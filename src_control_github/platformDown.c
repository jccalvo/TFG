
/** @file plataforma.c
  *
  * @brief Módulo encargado de adquirir un valor de temperatura que está
  * almacenado en una plataforma. El dato está almacenado en formato JSON.
  * Este proceso se divide en 4 etapas:
  *    
  * 1. Se crea un buffer donde almacenar el objeto que contiene el dato.
  * 2. Se realiza una petición http para obtener el objeto.
  * 3. Se escribe el objeto en el buffer.
  * 4. Se procesa el objeto para poder usar el dato solicitado.
  *
  * @author Juan Carlos Calvo Sansegundo.   
  * @date 20/10/2016.
  */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <jansson.h>
#include <curl/curl.h>
#include "platformDown.h"


/** @fn static size_t escribir_objeto (void * ptr, size_t size, size_t nmemb, struct objJSON* obj)
  *
  * @brief Función estática que escribe el objeto descargado
  * en una estructura objJSON creada previamente para almacenarlo.
  * 
  * @param void* ptr Puntero al objeto JSON que se quiere escribir en la estructura.
  * @param size_t size Posiciones de memoria que ocupa el objeto JSON.
  * @param size_t nmemb Número de bytes que ocupa una posición de memoria del objeto.
  * @param struct objJSON* obj Puntero a la estructura donde se va almacenar el objeto.
  * @return Estructura size_t con el número de bytes copiados,si todo es correcto. 
  * En caso contrario de error, devuelve -1.
  *
  * @author Juan Carlos Calvo Sansegundo.  
  * @date 20/10/2016.
  *
  */

  static size_t escribir_objeto (void *ptr, size_t size, size_t nmemb, struct objJSON * obj)
  { 
 
      if (obj->size + size * nmemb >= SIZE_MEM - 1)
      {
          return -1;
      }
      memcpy(obj->datos + obj->size, ptr, size * nmemb);
      obj->size += size * nmemb;

      return size * nmemb;
  }


/**  @fn int solicitar_objeto (const char* url, struct objJSON* obj)
  *  
  *  @brief Función que realiza la petición http para obtener 
  *  objeto JSON y copia el objeto en la estructura objJSON.
  *  Usa funcionalidades de la librería curl para realizar la petición http.
  *  y la función estática escribir_objeto () para copiar el objeto en la estructura. 
  *  @param const char* url Puntero a un array de caracteres con la URL del objeto JSON solicitado.
  *  @param struct objJSON* obj Puntero a la estructura donde se va a escribir el objeto.
  *  @return void.
  *	 
  *  @author Juan Carlos Calvo Sansegundo.  
  *  @date 20/10/2016.
  */

  void solicitar_objeto (const char * url, struct objJSON* obj)
  {
      /* Declaración e inicialización de un manejador tipo curl
         para realizar la petición http*/
      CURL * curl = NULL;   
      CURLcode estado;      
      long codigo;          
      curl_global_init(CURL_GLOBAL_ALL);      
      curl= curl_easy_init();               
      
      // Se comprueba que el manejador creado es válido.
      if (!curl)
      {     
          fprintf(stderr,"xxxxx> Error: No se ha podido inicializar el curl\n");
          curl_global_cleanup();
          return ;
      }
    
      /* Se configura el curl para realizar la petición del objeto y 
         copiarlo en el buffer.se obtiene el estado de la operación */
      curl_easy_setopt(curl,CURLOPT_URL,url);
      curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,escribir_objeto); 
      curl_easy_setopt(curl,CURLOPT_WRITEDATA,obj->datos);
      estado = curl_easy_perform(curl);
      curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &codigo); 
     
      if ((estado == CONEXION_OK) && (codigo == PET_HTTP_OK))
      {
          curl_easy_cleanup(curl);
          curl_global_cleanup();
          obj->datos[obj->size]='\0'; // Indica el final del objeto JSON.
          obj->size +=1;
          printf("-----> Conexión correcta \n\n");
          return ;
      }
      else
      {
          fprintf(stderr,"xxxxx> %s\n",curl_easy_strerror(estado));
          curl_easy_cleanup(curl);
          curl_global_cleanup();
          return ;
      }    
  }  


/**  @fn int extraer_temperatura (struct objJSON* obj, struct temp_leida* temperatura)
  *  
  *  @brief Función que procesa el objeto JSON y extrae el dato de temperatura solicitado.
  *
  *  @param struct objJSON* obj Puntero a la estructura que almacena el objeto a procesar.
  *  @param struct temp_leida* temperatura Puntero a la estructura donde se va a escribir 
  *  el dato de temperatura extraído del objeto JSON.
  *  @return 0 si se ha podido extraer el dato. En caso contrario, devuelve -1.
  *
  *  @author Juan Carlos Calvo Sansegundo.  
  *  @date 25/10/2016.
  */

  int extraer_temperatura (struct objJSON* obj, struct temp_leida* temperatura)
  {
      size_t i = 0;

      json_t* obj_handler;   // Manejador del objeto json completo.
      json_error_t errores;  // Manejador de los errores relacionados con el objeto.
      json_t* datapoints;    // Manejador del array de datapoints.
      json_t* valor;         // Manejador del valor de temperatura del datapoint.
      json_t* inst_medida;   // Manejador del instante de medida del datapoint.

     
      // Carga del objeto en el manejador json
      obj_handler = json_loads(obj->datos,0,&errores);    
      if (!obj_handler)
      {
          fprintf(stderr, "xxxxx> Error: en la línea %d: %s\n", errores.line, errores.text);
          return -1;
      }

      // Extracción del array de datapoints.
      obj_handler = json_array_get(obj_handler,0);
      obj_handler = json_object_get(obj_handler,"datapoints");
      if (!json_is_array(obj_handler))
      {
          fprintf(stderr, "xxxxx> Error: No existen el array de datapoints\n");
          return -1;
      }
     
      // Extracción del dato más reciente.
      for (i=0;i<json_array_size(obj_handler);i++)
      {
          datapoints = json_array_get(obj_handler,i);
          valor = json_array_get(datapoints,0);
          inst_medida = json_array_get(datapoints,1);

          if (json_is_number(valor) && json_is_number(inst_medida))
          {  
              temperatura->valor = json_real_value(valor)*FACT_CONV_TEMP;
              temperatura->timestamp = (int) json_integer_value(inst_medida);
          }
      }
      return 0;
  }

/*
  int main(){

      struct objJSON obj;
      struct temp_leida dato;
      int a = 0;
      int flag = 0;

      while (a<=10) 
      {
          memset(&(obj.datos),'X',SIZE_MEM);
          obj.size = 0;
          flag = solicitar_objeto(URL_FORMAT,&obj);
          if (flag!=0) 
          {
              printf("Error: No se ha podido extraer el objeto de la plataforma\n");
              return -1;
    	  }

	  flag = extraer_temperatura(&obj,&dato);
	  if (flag!=0) 
          {
              printf("Error: No se ha podido extraer el dato\n");
              return -1;
    	  }          
          a++;
      }
      return 0;
  }
*/
