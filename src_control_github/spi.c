/** @file spi.c
  *
  * @brief Fichero que contiene las funciones necesarias para usar la interfaz SPI
  *        de la raspberry PI y poder enviar los comandos al aire acondicionado a través  
  *        de dicha interfaz.
  *
  * @author Juan Carlos Calvo Sansegundo.    
  * @date 08/03/2017.
  */

#include <pigpio.h>
#include <string.h>
#include <stdio.h>
#include "comandos.h"
#include "spi.h"

/** @fn transmitir_comando(struct comando_modulado* cmd_mod)
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

int transmitir_comando (struct cmd_modulado* cmd_mod)
{

    // Flags para comprobar la configuración de los puertos SPI.
    int set_SCLK, set_MOSI, set_MISO, set_CE;  
    int flag_pines;
 
    // Variables configuración dispositivo SPI.
    int device;
    unsigned int channel = 0;
    unsigned int flag_device = 1;
    unsigned int nbytes= 1;
    int i;
    

    // Carga de la librería para su uso. 

    if (gpioInitialise() < OK)
    {
        printf("xxxxx> Error: No se pudo cargar la librería.\n");
        return -1;
    }

    // Selección de los puertos GPIO a usar;
     
    set_SCLK = gpioSetMode(PORT_SCLK,OUTPUT);
    set_MOSI = gpioSetMode(PORT_MOSI,OUTPUT);
    set_MISO = gpioSetMode(PORT_MISO,INPUT);
    set_CE = gpioSetMode(PORT_CE,OUTPUT);

    flag_pines = (set_SCLK & set_MOSI & set_MISO & set_CE);
    if (flag_pines)
    {
        printf("xxxxx> Error: Pines no habilitados.\n");
        return -1;     
    }

    // Transmisión del comando.
    
    for (i=0; i<=cmd_mod->pos; i++)
    {
    	device = spiOpen(channel,cmd_mod->f_bit,flag_device);
        if (device < OK) 
        {
	    printf("xxxxx> Error: Dispositivo no creado correctamente.\n\n");
	    return -1;
        }
	spiWrite(device,&cmd_mod->datos[i],nbytes);
        spiClose(device);
    }

    gpioTerminate();
    return OK;
}
