/**	@file PlataformaUp.h
  * 
  *	@brief Módulo que contiene las cabeceras de las estructuras, 
  * macros y funciones públicas definidas para	
  * poder subir un dato a la plataforma Graphite.
  *
  *	@author Juan Carlos Calvo Sansegundo
  * @date 25/05/2017 
  */

#ifndef PLATFORMUP_H
#define PLATFORMUP_H  

/** @fn void enviar_datos(int temp_control, int temp_setpoint, int temp_optimized, int timestamp).
  * 
  * @brief Función que se encarga de subir datos a Graphite.
  *
  * @param int temp_control. Temperatura de control(señal de control).
  * @param int temp_setpoint. Temperatura del aire acondicionado.
  * @param int temp_optimized. Temperatura óptima.
  * @param int ttimestamp. instante de lectura de los datos.
  *
  * @return void. No devuelve nada.
  *
  * @author Juan Carlos Calvo Sansegundo.
  * @date 25/05/2017.
  */

void enviar_datos(int temp_control, int temp_setpoint, int temp_optimized, int timestamp);  

#endif  