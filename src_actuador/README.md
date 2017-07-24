CARPETA SRC_ACTUADOR.
====================

Autor: Juan Carlos Calvo Sansegundo.
ültima actualización: 23/07/2017.

Esta carpeta contiene los archivos que conforman el programa de control. Estos son:

 -----> platformDown.c y PlatformDown.h: permite leer de la plataforma la temperatura de referencia y de medida.

 -----> comparador.c y comparador.h: realiza el cálculo de la señal de error (entrada del controlador PID).

 -----> PID.c y PID.h: implementa el controlador PID discreto y cálcula la acción de control.

 -----> comandos.c y comandos.h: selecciona el comando a transmitir por la interfaz SPI, de acuerdo a la señal	
				 de control obtenida.

 -----> spi.c y spi.h: se encarga de la modulación y transmisión por la interfaz SPI del comando.
 
 -----> platformUp.c y platformUp.h: se encarga de subir datos a la plataforma.

 -----> actuador.c: se encarga de la ejecución del programa principal.

 -----> signal_ref.txt: fichero desde donde se lee el valor de temperatura óptima.

 -----> make: fichero para compilar el programa y generar el ejecutable.

Esta carpeta también incluye la carpeta ficheros_tramas donde se almacenan los comandos del aire acondicionado.
Los comandos están almacenados en la carpeta tramas_zulo y están en formato .txt y .bin.


Historial de versiones:
-----------------------

 -----> Version 1.0 Fecha: 23/07/2017.
        
        Versión inicial del programa de control. Esta versión coincide con la presentada en la memoria del TFG.

 
