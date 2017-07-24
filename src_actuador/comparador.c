/** @file comparador.c
  *
  * @brief Módulo que realiza la comparación entre la señal
  * de referencia y la señal medida, generando la señal de error.
  *
  * @author Juan Carlos Calvo Sansegundo.     
  * @date 24/02/2017.
  */

#include "comparador.h"

/** @fn int calcular_error (int temp_ref, int temp_med)
  *
  * @brief Calcula el error como la diferencia de 
  * la señal de referencia con la señal medida.
  *
  * @param int temp_ref. Temperatura de referencia.
  * @param int temp_med. Temperatura medida.
  * @return Entero con la diferencia entre ambas señales (señal de error).
  *
  * @author Juan Carlos Calvo Sansegundo.     
  * @date 24/02/2017.
  */

int calcular_error (int temp_ref, int temp_med)
{
    int temp_err = temp_ref - temp_med;
    return temp_err;
}
