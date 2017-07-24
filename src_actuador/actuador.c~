/** @file actuadorc
  *
  * @brief Fichero que contienr el programa principal. V1.2.
  *
  * Este programa coge y lee de la plataforma los valores de la señal de referencia y de medida. -- plaformDown.c .
  * Posteriormente calcula la diferencia entre esos dos valores(error)y genera la señal de control. -- comparador.c y PID.c .
  * A continuación selecciona el comando de acuerdo con el valor de la señal de error. -- comandos.c .
  * Envía los datos de la señal de control, la temperatura óptima y la temperatura de setpoint -- platformUp.c
  * Después extrae el comando y lo modula. -- comandos.c
  * Por último transmite el comando a través de la interfaz SPI. -- spi.c .
  *
  * @author Juan Carlos Calvo Sansegundo.  
  * @date 09/03/2017.
  */

#include <time.h>
#include <sys/time.h>
#include <string.h>
#include <stdio.h>
#include "platformDown.h"
#include "comparador.h"
#include "PID.h"
#include "platformUp.h"
#include "comandos.h"
#include "spi.h"

/**
  * Periodo de iteración del programa.
  */
#define PER_ITER 10


/** @fn int leer_fichero(FILE * p).
  *
  * @brief Esta función se encarga de leer de un fichero la señal de referencia
  *  	   Solo se usa para hacer los test de programa
  *  
  * @param  nombre. Puntero a un fichero.   
  * @return Devuelve el valor de temperatura leído. En caso de error devuelve un 0.
  *  
  * @author Juan Carlos Calvo Sansegundo.   
  * @date 09/03/2017.
  */

 int leer_fichero(FILE * p){
    int temp;
    if(fscanf(p,"%d",&temp)==EOF){
        printf("----> volviendo al inicio.\n"); 
        if(fseek(p,0,SEEK_SET)!=0){
           printf("xxxxx> Error: No se puede volver al inicio del fichero.\n\n"); 
           return -1; 
        }
        if(fscanf(p,"%d",&temp)==EOF){
           printf("xxxxx> Error: No se puede leer el dato.\n\n");
           return -1;
        } 
    }
    return temp;
 }


/** @fn void* escribir_fichero(char *nombre, int diferencia).
  *
  * @brief Esta función se encarga de escribir en un fichero la diferencia 
  *  	   de temperatura que se ha calculado previamente.
  *  
  * @param  nombre. Nombre del fichero donde se va a escribir.
  * @param  diferencia. Valor que se va a escribir.   
  * @return No se devuelve nada si la operación se ha realizado correctamente
  *         En caso contrario se devuelve null;
  *  
  * @author Juan Carlos Calvo Sansegundo.   
  * @date 26/10/2016.
  */

 void* escribir_fichero(char *nombre,int iteracion, struct timeval instante, int temp_ref, int temp_med, int error, int error_1, int error_2, int control, int control_1, int comando){
   FILE * f;
   if((f=fopen(nombre,"a+"))==NULL){
        printf("xxxxx> No se puede escribir en el fichero  %s \n\n",nombre);
        return NULL;
   }else{
   	fprintf(f,"-----> Iteracion: %d\n",iteracion); 
	fprintf(f,"-----> Instante de la iteracion: %11.0f seg y %11.0f mseg\n", (double)instante.tv_sec, (double) instante.tv_usec); 
	fprintf(f,"-----> Temperatura de referencia: %d\n",temp_ref); 
	fprintf(f,"-----> Temperatura del sensor: %d\n",temp_med); 
	fprintf(f,"-----> Error, error_1 y error_2: %d, %d y %d\n",error,error_1,error_2); 
	fprintf(f,"-----> Control y control_1: %d y %d\n",control, control_1); 
        fprintf(f,"-----> Comando elegido: %d\n\n",comando);
        fclose(f);    
   }
   if((f=fopen("numeros.txt","a+"))==NULL){
        printf("xxxxx> No se puede escribir en el fichero  %s \n\n",nombre);
        return NULL;
   }else{
   	fprintf(f,"%d\n",iteracion); 
	fprintf(f,"%11.2f %11.2f\n",(double)(instante.tv_sec),(double)(instante.tv_usec)); 
	fprintf(f,"%d\n",temp_ref); 
	fprintf(f,"%d\n",temp_med); 
	fprintf(f,"%d %d %d\n",error,error_1,error_2); 
	fprintf(f,"%d %d\n",control, control_1); 
        fprintf(f,"%d\n\n",comando);
        fclose(f);    
   }
 }
 


 /** @fn void iteracion(struct timeval* current, struct timeval * next, struct timeval * timeout).
   *
   * @brief Esta función se encarga de controlar el periodo en el que se realiza la lectura de las 
   *         temperaturas de cada fichero.
   * 
   * @param current. Estructura timeval que almacena el instante actual.
   * @param next.    Estructura timeval que almacena el instante de la siguiente iteración. 
   * @param timeout. Estructura timeval que almacena el tiempo que falta hasta la siguiente iteración.
   *
   * @author Juan Carlos Calvo Sansegundo.   
   * @date 26/10/2016.
   */

 void iteracion(struct timeval* current, struct timeval * next, struct timeval * timeout){

    gettimeofday(current,NULL);
    next->tv_sec = next->tv_sec + PER_ITER;

    timeout->tv_sec = next->tv_sec - current->tv_sec;
    timeout->tv_usec = next->tv_usec - current->tv_usec;
    if(timeout->tv_usec <0){
       	timeout->tv_usec = timeout->tv_usec + 1000000;
        timeout->tv_sec = timeout->tv_sec -1;
    }
       
    select(0,NULL,NULL,NULL,timeout); 
 }


 /** fn main()
   *
   * @brief Método principal de ejecución del programa.
   *
   * @author Juan Carlos Calvo Sansegundo.   
   * @date 09/03/2017.
   */

 int main(){

    struct objJSON obj;   
    struct temp_leida temp_ref;   
    struct temp_leida temp_med;   

    struct comando cmd_setpoint;  
    struct cmd_modulado cmd_mod_setpoint;

    struct timeval current_iteration = {0,0};
    struct timeval next_iteration = {0,0};
    struct timeval timeout = {0,0};

    int error, error_1, error_2 = 0;                
    int control, control_1 = 0;       
    int temp_setpoint = 0;  
    int temp_med_1 = 0; 

    int e_s1 = 0; // Efecto windup 

    int a, flag = 0;   

    FILE *p = fopen("signal_ref.txt","r");
    if (p == NULL){
	printf("xxxxx> Error: No se puede abrir el fichero.\n\n");
        return -1;
    }

    gettimeofday(&current_iteration,NULL);
    gettimeofday(&next_iteration,NULL);

    while(1)
    {
         gettimeofday(&current_iteration,NULL);
         printf("--->Inst entrada: %10.2f seg y %10.2f mseg:\n\n",(double)current_iteration.tv_sec,(double)current_iteration.tv_usec);

         // Limpiar estructuras.
         memset(&(obj.datos),'\0',SIZE_MEM);
         obj.size = 0;

         memset(&(cmd_setpoint.datos),'\0',SIZE_MEM);
         cmd_setpoint.pos = 0;

         memset(&(cmd_mod_setpoint.datos),'\0',SIZE_MEM);
         cmd_mod_setpoint.pos = 0;
         cmd_mod_setpoint.f_bit = 0;

         // obtención de la temperatura de referencia.
         /*solicitar_objeto (URL_FORMAT, &obj);
         flag = extraer_temperatura (&obj, &temp_ref);  
         if (flag) {
            printf ("xxxxx> Error: No se ha podido obtener la temperatura de referencia.\n");
            return -1;
         }*/  

	 temp_ref.valor = leer_fichero(p); 
         printf("-----> Temperatura optima: %d\n",temp_ref.valor);

         //obtención de la temperatura de medida.
         solicitar_objeto (URL_FORMAT, &obj);
         flag = extraer_temperatura (&obj, &temp_med);  
         if (flag) {
	     printf ("xxxxx> Error: No se ha podido extraer la temperatura medida.\n");
             temp_med.valor = temp_med_1;
            // return -1;
         }else{ 
	     temp_med_1 = temp_med.valor;
	 }
         printf("-----> Temperatura medida sensor: %d\n",temp_med.valor);        
        // Cálculo de la señal de control 
        error = calcular_error (temp_ref.valor, temp_med.valor);
        control = calcular_tempcontrol (error, error_1, error_2, control_1, &e_s1);
        error_2 = error_1;
        error_1 = error;
        control_1 = control; 

        // Selección del comando y envío de datos a la plataforma.
        temp_setpoint = seleccionar_setpoint (control);
        printf("-----> Temperatura setpoint: %d\n",temp_setpoint);
        enviar_datos(temp_setpoint,control,temp_ref.valor,current_iteration.tv_sec);
            
        // Obtención del comando y modulación de éste.
        flag = obtener_comando (temp_setpoint, &cmd_setpoint);        
        if (flag) {
            printf ("xxxxx> Error: No se ha podido obtener el comando.\n");
            //return -1;
        }

        flag = modular_comando (&cmd_setpoint, &cmd_mod_setpoint);
        if (flag) {
            printf ("xxxxx> Error: No se ha podido modular el comando.\n");
            //return -1;
        }

        // Transmisión del comando por la interfaz SPI.
        flag = transmitir_comando (&cmd_mod_setpoint);
        if (flag) {
        printf ("xxxxx> Error: No se ha podido transmitir el comando.\n");
          //  return -1;
        }

        // Calcular intervalo de la siguiente iteración.
        iteracion(&current_iteration, &next_iteration, &timeout);
        a++;
    }      
    
    fclose(p);
    return 0;
 }


