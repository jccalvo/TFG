/** @file comandos.c
  * 
  * @brief  Módulo que se encarga de convertir la señal de control proporcionada
  * por el controlador en un comando legible para el sistema de refrigeración.
  * Este proceso consta de 2 etapas:
  *
  * 1. Se selecciona el comando de acuerdo al valor de la señal de control.
  * 2. Se modula el comando y se envía al bloque de transmisión.
  *
  * @author Juan Carlos Calvo Sansegundo.    
  * @date 08/03/2017.
  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/stat.h> 
#include "comandos.h"


/** @fn int seleccionar_setpoint (int temp_control)
  *
  * @brief Esta función determina la temperatura del aire acondicionado
  * a partir de la señal de control generada por el PID.
  * 
  * @param int temp_control Valor de la señal de control en milésimas.
  * @return int Un entero con el valor de la temperatura 
  * a la que debe funcionar el aire acondicionado (setpoint).
  *
  * @author Juan Carlos Calvo Sansegundo.    
  * @date 30/10/2016.
  */

int seleccionar_setpoint (int temp_control)
{
    int temp; 

    // Redondeo y conversión a unidades.
    temp = (temp_control + REDONDEO_MILESIMAS)/FACT_CONV;

    // Selección de la temperatura a aplicar al aire acondicionado.
    if (temp >= TEMP_MAX_AC) 
    {
        return TEMP_MAX_AC;
    }
    else if (temp<=TEMP_MIN_AC) 
    {
        return TEMP_MIN_AC;
    }
    else
    {
        return temp;  
    }
}

/** @fn int obtener_comando (int temp_setpoint, struct comando* cmd)
  *
  * Esta función busca el fichero que contiene el comando que hay que ejecutarse 
  * para que el aire acondicionado funcione a la temperatura de setpoint y 
  * copia su contenido en un estructura de tipo comando.
  *  
  * @param int temp_setpoint Valor del comando a seleccionar.
  * @param struct comando* cmd Puntero a la estructura donde se va a copiar el comando.
  * @return 0 si se ha copiado el comando. En caso contrario, devuelve -1.
  *
  * @author Juan Carlos Calvo Sansegundo.    
  * @date 08/03/2017.
  */

int obtener_comando (int temp_setpoint, struct comando* cmd)
{
    FILE * f; 
    struct stat est_fichero; 
    char url[FILE_PATH_SIZE];
    int fd, bytes_leidos, a;
    
    // Apertura del fichero
    a = snprintf(url, FILE_PATH_SIZE, FILE_PATH_FORMAT, temp_setpoint);
    f = fopen(url,"rb");
    if (f==NULL)
    {
        printf("xxxxx> Error: El comando seleccionado no existe.\n");
        return -1;
    }

    // Obtención del estado del fichero.
    fd = fileno(f);
    a = fstat(fd, &est_fichero);
    if (a!=0)
    {
        printf("xxxxx> Error: No se puede obtener el estado del fichero.\n");
        fclose(f);
        return -1;
    }
    
    // Copia del contenido del fichero en la estructura.       
    bytes_leidos = fread(cmd->datos, 1, sizeof(cmd->datos), f);
    cmd->pos = bytes_leidos;  
    fclose(f);
    if (bytes_leidos != est_fichero.st_size)
    {
        printf("xxxxx> Error: Copia no realizada.\n");
        return -1;
    } 
        
    return 0;    
}

/** @fn static void modular_bit (int value, int pos_init, int size_bit_mod, struct cmd_modulado* cmd)
  *
  * @brief Función estática que realiza la modulación 
  * de un bit de un comando.
  *  
  * @param value Valor del bit que se quiere modular, tipo int.
  * @param int pos_init Posición (dentro de una transferencia SPI) del 1º bit 
  *        de la secuencia de modulación.
  * @param int size_bit_mod Tamaño de la secuencia de modulación, sin los bits de parada.
  * @param struct cmd_modulado* cmd_mod Puntero a la estructura donde 
  *        se va a almacenar el comando ya modulado.               
  * @return void
  *
  * @author Juan Carlos Calvo Sansegundo.    
  * @date 08/03/2017.
  */

static void modular_bit (int value, int pos_init, int size_bit_mod, struct cmd_modulado * cmd)
{
    int i;
    int j = cmd->pos;

    int mask = BIT_MASK >> pos_init;
    if (!mask) {   
        mask = BIT_MASK; 
    }

    for (i = 0; i < size_bit_mod; i++)
    {
         if (value == 0) {
             cmd->datos[j] &= (~mask);
         }
         else
         {
             if ((i%2)!=0) {
                 cmd->datos[j] &= (~mask);
             }
	     else 
             { 
                 cmd->datos[j] |= (mask);  
             }
         }

         mask >>=1;
         if (!mask)
         {   
             mask = BIT_MASK; 
             j++;
             cmd->pos = j;
         }
    }
}


/** @fn int modular_comando(struct comando* cmd, struct cmd_modulado* cmd_mod).
  *
  * @brief Función que modula el comando que se va a transmitir al aire acondicionado.
  * Extrae de la cabecera los parámetros de transmisión(frec bit, frec portadora...) y
  * modula el comando de acuerdo a esos parámetros. 
  *
  * @param struct comando* cmd Puntero a la estructura que almacena el comando sin modular.
  * @param struct cmd_modulado* cmd_mod Puntero a la estructura donde 
  *        se va almacenar el comando ya modulado.
  * @return Devuelve 0 si se ha hecho la modulación correctamente. 
  * En caso de error, devuelve -1.
  * 
  * @author Juan Carlos Calvo Sansegundo.    
  * @date 08/03/2017.
  */

int modular_comando (struct comando* cmd, struct cmd_modulado * cmd_mod)
{ 
    int pos_inicio, bit, i; 
    int bits_parada, long_secuencia; 
    int nbits_cmd_mod, nbits_exceso, nbytes_cmd_mod;

    unsigned int long_bit_mod; 
    unsigned int t_bit, t_carrier, f_carrier;
    unsigned char mask_bit = BIT_MASK;
    
    //Cálculo de parámetros para realizar la modulación. 

    t_carrier = cmd->datos[BYTE_TCARRIER];
    f_carrier = FACT_SEG_USEG / t_carrier;
    t_bit = (cmd->datos[MSB_TBIT] << NBITS_BYTE) + cmd->datos[LSB_TBIT];
    
    cmd_mod->f_bit = 2 * f_carrier; // se usa el doble para generar la portadora deseada
    
    long_bit_mod = (2 * t_bit) / t_carrier;   
    if ((2 * t_bit) % t_carrier != 0)
    {
        long_bit_mod += 1;
    }
    
    // Modulación del comando.
  
    pos_inicio = 0;
 
    for (i = BYTE1_DATOS; i < (cmd->datos[BYTE_SIZE_CMD] + BYTE1_DATOS); i++){

        while(mask_bit)
        {      
              bit = cmd->datos[i] & mask_bit;  
              bits_parada = (long_bit_mod + pos_inicio) / NBITS_SPI;
              long_secuencia = long_bit_mod - bits_parada;
              modular_bit (bit, pos_inicio, long_secuencia, cmd_mod); 
              pos_inicio = (long_bit_mod + pos_inicio) % NBITS_SPI;
              mask_bit >>= 1;               
        }
        mask_bit = BIT_MASK;     
    }

    if (pos_inicio == 8 || pos_inicio == 0) {
	cmd_mod->pos -= 1;
    }
    
    // Comprobación de que se ha hecho bien la modulación.

    nbits_cmd_mod = cmd->datos[BYTE_SIZE_CMD] * NBITS_BYTE * long_bit_mod;
    nbits_exceso = (nbits_cmd_mod / NBITS_SPI) + ((nbits_cmd_mod % NBITS_BYTE)!=0);   
  
    nbytes_cmd_mod = (nbits_cmd_mod - nbits_exceso) / NBITS_BYTE + (((nbits_cmd_mod - nbits_exceso) % NBITS_BYTE)!=0);
   
    if (nbytes_cmd_mod == (cmd_mod->pos + 1))
    {
        return 0;
    }
    else
    {
        return -1;
    }
}

/*

int main ()
{
    struct comando cmd;
    struct cmd_modulado cmd_mod;
    int j,a,b,c;
    int temperatura = 24000;
    j = 0;
    char p = BIT_MASK>>8;
    //while(j<=5){
	 
	memset(&(cmd.datos),p,2000);
        cmd.pos = 0;

	memset(&(cmd_mod.datos),p,10000);
        cmd_mod.pos = 0;

    	a = seleccionar_comando(temperatura);
        b = leer_comando(33,&cmd);
        c = modular_comando(&cmd,&cmd_mod);
	j++; 
    //}

}
*/
