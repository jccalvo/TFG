#define PORT_SCLK 11 // Puerto SCLK en Raspberry PI, según esquema GPIO.

#define PORT_MOSI 10 // Puerto MOSI en Raspberry PI, según esquema GPIO.

#define PORT_MISO 9 // Puerto MISO en Raspberry PI, según esquema GPIO.

#define PORT_CE 8 // Puerto CE en Raspberry PI, según esquema GPIO.

#define OUTPUT 1 // Valor para configurar un puerto como salida.

#define INPUT 0 // Valor para configurar un puerto como entrada.

#define OK 0 // El puerto es configurado como salida.


int transmitir_comando (struct cmd_modulado* com)

// Función que realiza la transmisión del comando por la interfaz SPI.

  @param struct cmd_modulado* mod. // Puntero al comando a transmitir.

  @return int // Devuelve 0 si todo ha ido bien. En caso de error, da -1.