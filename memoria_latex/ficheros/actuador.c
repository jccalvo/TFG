 int main(){

    // Definicición e inicialización de variables
    while(1)
    {
        // Obtención del instante de inicio.
        //Limpieza de estructuras.
        // obtención de la temperatura de referencia.
        solicitar_objeto (URL_FORMAT, &obj);
        flag = extraer_temperatura (&obj, &temp_ref);  
        if (flag) {
            //Error: No se ha podido leer la temperatura.
            return -1;
        }   

        // obtención de la temperatura de medida.
        solicitar_objeto (URL_FORMAT, &obj);
        flag = extraer_temperatura (&obj, &temp_med);  
        if (flag) {
            //Error: No se ha podido leer la temperatura.
            return -1; 
        } 

        // Cálculo de la señal de control 
        error = calcular_error (temp_ref.valor, temp_med.valor);
        control = calcular_tempcontrol (error, error_1, error_2, control_1);
    
        // Selección del comando y envío de datos a la plataforma.
        temp_setpoint = seleccionar_setpoint (control);
        enviar_datos(temp_setpoint,control,temp_ref.valor,current_iteration->tv.sec);
            
        // Obtención del comando y modulación de éste.
        flag = obtener_comando (temp_setpoint, &cmd_setpoint);        
        if (flag) {
            // Error: No se ha podido obtener el comando.
            return -1;
        }

        flag = modular_comando (&cmd_setpoint, &cmd_mod_setpoint);
        if (flag) {
            // Error: No se ha podido modular el comando.
            return -1;
        }

        //Transmisión del comando por la interfaz SPI.
        flag = transmitir_comando (&cmd_mod_setpoint);
        if (flag) {
            // Error: No se ha podido transmitir el comando.
            return -1;
        } 

        // Calcular intervalo de la siguiente iteración.
        iteracion(&current_iteration, &next_iteration, &timeout);
    }      
    return 0;
 }