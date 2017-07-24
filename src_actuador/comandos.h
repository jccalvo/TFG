/**  @file comandos.h
  * 
  *  @brief Fichero que contiene las macros, estructuras y cabeceras 
  *  de las funciones públicas definidas en el fichero comandos.c .
  *
  *  @author Juan Carlos Calvo Sansegundo.    
  *  @date 08/03/2017.
  */

#ifndef COMANDOS_H
#define COMANDOS_H 

/**
  * Factor de conversión de unidades a milésimas y viceversa.
  */

#define FACT_CONV 1000      

/**
  * Máxima temperatura aceptada por el aire acondicionado.
  */

#define TEMP_MAX_AC 32    

/**
  * Mínima temperatura aceptada por el aire acondicionado.
  */

#define TEMP_MIN_AC 18   

/**
  * Valor para establecer un redondeo con precisión de milésimas.
  */

#define REDONDEO_MILESIMAS 500  

/**
  * Formato de la ruta de acceso a los ficheros que contienen los comandos.
  */

#define FILE_PATH_FORMAT "ficheros_tramas/tramas_zulo/%dzulo.txt.bin"

/**
  * Tamaño máximo permitido de la ruta de acceso a los ficheros.
  */

#define FILE_PATH_SIZE 50

/**
  * Máscara utilizada para manejar bits.
  */  

#define BIT_MASK 0x80

/**
  * Factor de conversion de segundos a microsegundos.
  */
#define FACT_SEG_USEG 1000000

/**
  * 1º byte de datos del comando.
  */
#define BYTE1_DATOS 10

/**
  * Byte del comando que contiene el número de bytes de datos.
  */
#define BYTE_SIZE_CMD 6

/**
  * Posición del byte más significativo del periodo de bit.
  */
#define MSB_TBIT 7

/**
  * Posición del byte menos significativo del periodo de bit.
  */
#define LSB_TBIT 8

/**
  * Posición del byte con el periodo de la portadora.
  */
#define BYTE_TCARRIER 9

/**
  *Número de bits de una transferencia spi, incluyendo el bit de parada.
  */
#define NBITS_SPI 9

/**
  * Tamaño (en bits) de un byte.
  */
#define NBITS_BYTE 8

/**
  * Tamaño del array donde se almacena el comando sin modular.
  */
#define TAM_CMD (2*1024) // 2KB

/**
  * Tamaño del array donde se almacena el comando sin modular.
  */
#define TAM_CMDMOD (10*1024) // 10KB


/** @struct comando
  * 
  * @brief Estructura donde se va a almacenar el comando 
  * que hay que enviar pero sin estar modulado.
  *
  * @author Juan Carlos Calvo Sansegundo.    
  * @date 05/11/2016.
  */

struct comando {
    unsigned char datos [TAM_CMD]; /**< Array donde se almacena el comando */
    long pos; /**< Posición del array que se está usando */ 
};

/** @struct cmd_modulado
  *  
  * @brief Estructura que contiene el comando yamodulado.
  *
  * @author Juan Carlos Calvo Sansegundo.     
  * @date 04/12/2016.
  */

struct cmd_modulado {
    unsigned char datos [TAM_CMDMOD]; /**< Array donde se almacena el comando modulado */
    long pos; /**< posición del array que se está usando */ 
    int f_bit; /**< Frecuencia de bit usada para transmitir el comando por SPI */
};


/** @fn int seleccionar_setpoint (int temp_control)
  *
  * @brief Esta función determina la temperatura del aire acondicionado
  * a partir de la señal de control generada por el PID.
  * 
  * @param int temp_control. Valor de la señal de control en milésimas.
  * @return Un entero con el valor de la temperatura 
  * a la que debe funcionar el aire acondicionado (setpoint).
  *
  * @author Juan Carlos Calvo Sansegundo.    
  * @date 30/10/2016.
  */

int seleccionar_setpoint (int temp_control);


/** @fn int obtener_comando (int temp_setpoint, struct comando* cmd)
  *
  * Esta función busca el fichero que contiene el comando que hay que ejecutarse 
  * para que el aire acondicionado funcione a la temperatura de setpoint y 
  * copia su contenido en un estructura de tipo comando.
  *  
  * @param int temp_setpoint. Valor del comando a seleccionar.
  * @param struct comando* cmd. Puntero a la estructura donde se va a copiar el comando.
  * @return 0 si se ha copiado el comando. En caso contrario, devuelve -1.
  *
  * @author Juan Carlos Calvo Sansegundo.    
  * @date 08/03/2017.
  */

int obtener_comando (int temp_setpoint, struct comando* cmd);


/** @fn int modular_comando(struct comando* cmd, struct cmd_modulado* cmd_mod).
  *
  * @brief Función que modula el comando que se va a transmitir al aire acondicionado.
  * Extrae de la cabecera los parámetros de transmisión(frec bit, frec portadora...) y
  * modula el comando de acuerdo a esos parámetros. 
  *
  * @param struct comando* cmd. Puntero a la estructura que almacena el comando sin modular.
  * @param struct cmd_modulado* cmd_mod. Puntero a la estructura donde 
  *        se va almacenar el comando ya modulado.
  * @return Devuelve 0 si se ha hecho la modulación correctamente. 
  * En caso de error, devuelve -1.
  * 
  * @author Juan Carlos Calvo Sansegundo.    
  * @date 08/03/2017.
  */

int modular_comando (struct comando* cmd, struct cmd_modulado * cmd_mod);


#endif
