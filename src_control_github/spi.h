/** @file spi.h
  *
  * @brief Fichero que contiene las cabecaeras de las funciones para usar la interfaz SPI
  *  	   de la raspberry PI y poder enviar los comandos al aire acondicionado a través  
  *        de dicha interfaz.
  *
  * @author Juan Carlos Calvo Sansegundo.    
  * @date 08/03/0217.
  */

#ifndef SPI_H
#define SPI_H

/**
  * Puerto SCLK en Raspberry PI, según esquema GPIO.
  */
#define PORT_SCLK 11

/**
  * Puerto MOSI en Raspberry PI, según esquema GPIO.
  */
#define PORT_MOSI 10

/**
  * Puerto MISO en Raspberry PI, según esquema GPIO.
  */
#define PORT_MISO 9

/**
  * Puerto CE en Raspberry PI, según esquema GPIO.
  */
#define PORT_CE 8

/**
  * El puerto es configurado como salida.
  */
#define OUTPUT 1

/**
  * El puerto es configurado como entrada.
  */
#define INPUT  0

/**
  * Todo correcto.
  */ 
#define OK 0


/** @fn transmitir_comando (struct cmd_modulado* cmd_mod)
  *
  * @brief Función que se encarga de inicializar la librería PIGPIO, abrir los pines 
  *        de la interfaz SPI y realizar la transmisión de dicho comando.
  *
  * @param com. Estructura que almacena el comando a transmitir, tipo comando_modulado.   
  * @return Devuelve 0 si se ha realizado correctamente. En caso contrario, devuelve -1.
  *
  * @author Juan Carlos Calvo Sansegundo.    
  * @date 04/12/2016.
  */

int transmitir_comando (struct cmd_modulado * cmd_mod);

#endif
