/** @file PID.c
  *
  * @brief Módulo encargado de implementar un controlador PID.
  *
  * @author Juan Carlos Calvo Sansegundo.   
  * @date 24/02/2017.
  */

#include <stdio.h>
#include <stdlib.h>
#include "PID.h"
#include "comandos.h"

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

int calcular_tempcontrol (int err, int err_1, int err_2, int control_1, int* e_s1)
{   
    //Parte entera y decimal de KI
    int ki_ent = KI/FACT_CONV;
    int ki_dec = KI%FACT_CONV;

    // Parte entera y decimal del periodo.
    int per_ent = PERIOD/FACT_CONV;
    int per_dec = PERIOD%FACT_CONV;

    // Variables auxiliares
    int a,b;
    int c1,c2,c3;
    int sum1, sum2, sum3;
   
    // Parte entera y decimal de los coeficientes
    int q0_ent, q0_dec;
    int q1_ent, q1_dec;
    int q2_ent, q2_dec;

    // Valor de la señal de control
    int total_control;

    // Efecto windup
    int e_s, sum4, windup;

   /* La salida es u(k) = u(k-1) + q0*e(k) + q1*e(k-1) + q2*e(k-2) + q3*(e_s(k) + e_s(k-1)); 
    *
    *  q0 = Kp(1 + td/Tm + Tm/(2*ti)) = (Kp + Kd/Tm + Tm*Ki/2); 
    *  q1 = Kp(-1 - 2*td/Tm + Tm/(2*ti)) = (-Kp - 2*Kd/Tm + Tm*Ki/2); 
    *  q2 = Kp*td/Tm = Kd/Tm; 
    *  q3 = Tm/2*Tt = Tm*Kt/2; // Corregir el efecto windup. 
    */

    // 1º: Cálculo de Kd/Tm;
    c1 = (KD*FACT_CONV)/PERIOD;

    // 2º: Cálculo de 2*Kd/Tm;
    c2 = (2*KD*FACT_CONV)/PERIOD;

    // 3º: Cálculo de Tm*Ki/2;
    a =(ki_ent*per_ent);
    b =(ki_ent*per_dec) + (ki_dec*per_ent) + ((ki_dec*per_dec)/FACT_CONV);
    c3 = (a*FACT_CONV + b)/2;

    // 4º: Cálculo de q0;
    a = KP + c1 + c3;
    q0_ent = a/FACT_CONV;
    q0_dec = a%FACT_CONV;

    // 4º: Cálculo de q1;
    a = -KP - c2 + c3;
    q1_ent = a/FACT_CONV;
    q1_dec = a%FACT_CONV;

    // 5º: Cálculo de q2;
    q2_ent = c1/FACT_CONV;
    q2_dec = c1%FACT_CONV;

    printf("-----> q0: %d.%d q1: %d.%d q2: %d.%d \n",q0_ent,q0_dec,q1_ent,abs(q1_dec),q2_ent,q2_dec);

    // 6º: Cálculo de q0(e(k));
    a = q0_ent*(err/FACT_CONV);
    b = (q0_ent*(err%FACT_CONV)) + (q0_dec*(err/FACT_CONV)) + (q0_dec*(err%FACT_CONV))/FACT_CONV;
    sum1 = a*FACT_CONV + b;

    // 7º: Cálculo de q1(e(k-1));
    a = q1_ent*(err_1/FACT_CONV);
    b = (q1_ent*(err_1%FACT_CONV)) + (q1_dec*(err_1/FACT_CONV)) + (q1_dec*(err_1%FACT_CONV))/FACT_CONV;
    sum2 = a*FACT_CONV + b;

    // 8º: Cálculo de q2(e(k-2));
    a = q2_ent*(err_2/FACT_CONV);
    b = (q2_ent*(err_2%FACT_CONV)) + (q2_dec*(err_2/FACT_CONV)) + (q2_dec*(err_2%FACT_CONV))/FACT_CONV;
    sum3 = a*FACT_CONV + b;

    // 9º: Salida total
    total_control = control_1 + sum1 + sum2 + sum3;
    printf("-----> Total_control sin windup: %d.%d\n",total_control/FACT_CONV,abs(total_control%FACT_CONV));

    /*
    // 10º: Calculo de (Kt*Tm/2) (efecto windup)
    a =((KT/FACT_CONV)*per_ent);
    b =((KT/FACT_CONV)*per_dec) + ((KT%FACT_CONV)*per_ent) + (((KT%FACT_CONV)*per_dec)/FACT_CONV);
    c3 = (a*FACT_CONV + b)/2;

    
    // 11º: Cálculo de e_s(k) + e_s(k-1);
    if(total_control < (TEMP_MIN_AC*FACT_CONV) || total_control > (TEMP_MAX_AC*FACT_CONV))
    {
        e_s = ((seleccionar_setpoint(total_control)*FACT_CONV) - total_control);
    }
    else
    {
	e_s = 0;
    }
    sum4 = e_s; + (*e_s1);

    // 12º: Cálculo del (Kt*Tm/2)*(e_s(k) + e_s(k-1))
    a = (c3/FACT_CONV)*(sum4/FACT_CONV);
    b = ((c3/FACT_CONV)*(sum4%FACT_CONV) + (c3%FACT_CONV)*(sum4/FACT_CONV) + ((c3%FACT_CONV)*(sum4%FACT_CONV))/FACT_CONV);
    windup = a*FACT_CONV + b;

    // Control con efecto windup

    total_control = total_control + windup;  
    (*e_s1) = e_s;  
    */
    printf("-----> sum_1: %d.%d sum_2: %d.%d sum_3: %d.%d \n",sum1/FACT_CONV,abs(sum1%FACT_CONV),sum2/FACT_CONV,abs(sum2%FACT_CONV),sum3/FACT_CONV,abs(sum3%FACT_CONV));
    //printf("-----> Windup: %d.%d\n",(windup/FACT_CONV),(abs)(windup%FACT_CONV));
    //printf("-----> Total_control con windup: %d.%d\n\n",total_control/FACT_CONV,abs(total_control%FACT_CONV));
    printf("-----> Error en n: %d.%d ,n-1: %d.%d, n-2: %d.%d \n",err/FACT_CONV,err%FACT_CONV,err_1/FACT_CONV,err_1%FACT_CONV,err_2/FACT_CONV,err_2%FACT_CONV);

    return total_control;
}

/*
int main () 
{
    
    int err = 4500;
    int err_1 = 0;
    int err_2 = 0;
    int control_1 = 0;
    int control = 0;
    int es_1 = 0;

    int a = 0;

    while (a<=27) {

	control = calcular_tempcontrol (err, err_1, err_2, control_1,&es_1);
        err_2 = err_1;
        err_1 = err;
        control_1 = control;
        a++;
    }

}*/
