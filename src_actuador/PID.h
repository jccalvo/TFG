/**  @file PID.h
  *
  *  @brief Fichero que contiene las macros, estructuras y cabeceras de las 
  *         funciones públicas definidas en el fichero PID.c .
  *
  *  @author Juan Carlos Calvo Sansegundo.   
  *  @date 09/03/2017.
  */

#ifndef PID_H
#define PID_H

/** 
  * Constante proporcional expresado en milésimas.
  */
#define KP 28000

/** 
  * Constante integral expresado en milésimas.
  */  
#define KI 37
   
/** 
  * Constante derivativa expresado en milésimas.
  */   
#define KD 300000

/** 
  * Periodo de muestreo expresado en milésimas.
  */      
#define PERIOD 10000  

/** 
  * Factor de conversion de unidades a milésimas y viceversa.
  */      
//#define FACT_CONV 1000 

/** 
  * Constante del efecto windup en milésimas.
  */      
#define KT 200

/** @fn int calcular_tempcontrol (int err, int err_1, int err_2, int control_1, int* e_s1).
  *
  * @brief Esta función implementa el algoritmo de un controlador PID discreto,
  * y genera el valor de la señal de control en cada instante.
  *
  * @param err Valor de la señal de error en el instante n, tipo int.
  * @param err_1 Valor de la señal de error en el instante n-1, tipo int.
  * @param err_2 Valor de la señal de error en el instante n-2, tipo int.
  * @param control_1 Valor de la señal de control en el instante n-1,tipo int.
  * @return int Valor de la señal de control en el instante n.
  *
  * @author Juan Carlos Calvo Sansegundo.    
  * @date 24/02/2017.
  */

int calcular_tempcontrol (int err, int err_1, int err_2, int control_1, int* e_s1);

#endif
