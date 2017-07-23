#define KP 28000 // Constante proporcional expresado en milésimas.

#define KI 37 // Constante integral expresado en milésimas.  
   
#define KD 300000 // Constante derivativa expresado en milésimas. 

#define PERIOD 10000  // Periodo de muestreo expresado en milésimas.  

#define FACT_CONV 1000 // Factor de conversion de unidades a milésimas. 

int calcular_tempcontrol (int err, int err_1, int err_2, int control_1).

// Esta función implementa el algoritmo de un controlador PID discreto,
// y genera el valor de la señal de control en cada instante.

  @param int err // Valor de la señal de error en n.

  @param int err_1 // Valor de la señal de error en n-1.

  @param int err_2 // Valor de la señal de error en n-2.

  @param int control_1 // Valor de la señal de control en n-1.

  @return int // Valor de la señal de control en n.