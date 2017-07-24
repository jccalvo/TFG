/**  @file platformDown.h
  *
  *  @brief Fichero que contiene las cabeceras de las funciones necesarias para realizar la
  *  solicitud de petición HTTP al servidor y la obtención del objeto JSON.
  *
  *  @author Juan Carlos Calvo Sansegundo.   
  *  @date 26/10/2016.
  */

#ifndef PLATFORMDOWN_H
#define PLATFORMDOWN_H

/** 
  * Factor para convertir la temperatura en un entero.
  */

#define FACT_CONV_TEMP 1000  

/** 
  * Código que indica que la petición http ha sido satisfactoria.
  */

#define PET_HTTP_OK 200

/** 
  * Código que indica que la conexión con la url es satisfactoria.
  */  

#define CONEXION_OK 0   

/**
  * Tamaño máximo del buffer que almacena el objeto JSON.
  */

#define SIZE_MEM (5*1024)  /**< Equivale a un tamaño de 5 KB */

/**
  * Formato de la URL del objeto JSON.
  */

#define URL_FORMAT   "http://visualizee.die.upm.es:8000/render?format=json&target=visualizee.greencpd.b039.rack.r0001.server.host2.temperature.ambient.1.1000&from=-1min"    


/**  @struct objJSON.
  *  
  *  @brief Estructura donde se almacena el objeto JSON
  *  descargado de la plataforma.
  *
  *  @author Juan Carlos Calvo Sansegundo.  
  *  @date 20/10/2016.
  */
 
struct objJSON{
     char datos[SIZE_MEM];  /**< Array reservado para almacenar el objeto */
     int size;  /**< Tamaño en bytes del objeto (Número de posiciones usadas) */ 
};


/**  @struct temp_leida
  *  
  *  @brief Estructura donde se almacena el dato de temperatura solicitada
  *  y que ha sido extraído previamente de un objeto JSON.
  *
  *  @author Juan Carlos Calvo Sansegundo.  
  *  @date 20/10/2016.
  */ 
 
struct temp_leida{
     int valor;       /**< valor de la temperatura */
     int timestamp;   /**< Instante de medida de la temperatura */
};


/**  @fn int solicitar_objeto (const char* url, struct objJSON* obj)
  *  
  *  @brief Función que realiza la petición http para obtener 
  *  objeto JSON y copia el objeto en la estructura objJSON.
  *  Usa funcionalidades de la librería curl para realizar la petición http
  *  y la función estática escribir_objeto () para copiar el objeto en la estructura. 
  *
  *  @param const char* url. Puntero a un array de caracteres con la URL del objeto JSON solicitado.
  *  @param struct objJSON* obj. Puntero a la estructura donde se va a escribir el objeto.
  *  @return void.
  *  
  *  @author Juan Carlos Calvo Sansegundo.  
  *  @date 20/10/2016.
  */

void solicitar_objeto (const char * url, struct objJSON* obj);
 

/**  @fn int extraer_temperatura (struct objJSON* obj, struct temp_leida* temperatura)
  *  
  *  @brief Función que procesa el objeto JSON y extrae el dato de temperatura solicitado.
  *
  *  @param struct objJSON* obj. Puntero a la estructura que almacena el objeto a procesar.
  *  @param struct temp_leida* temperatura. Puntero a la estructura donde se va a escribir 
  *  el dato de temperatura extraído del objeto JSON.
  *  @return 0 si se ha podido extraer el dato. En caso contrario, devuelve -1.
  *
  *  @author Juan Carlos Calvo Sansegundo.  
  *  @date 25/10/2016.
  */

int extraer_temperatura (struct objJSON* obj, struct temp_leida* temperatura);

#endif
