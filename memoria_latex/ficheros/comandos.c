#define FACT_CONV 1000 // Factor de conversión de unidades a milésimas. 

#define TEMP_MAX_AC 32 // Temperatura máxima del aire acondicionado.

#define TEMP_MIN_AC 18 // Temperatura mínima del aire acondicionado. 

#define REDONDEO_MILESIMAS 500 // Valor para redondear las milésimas.

// Formato de la ruta de acceso a los ficheros que contienen los comandos.
#define FILE_PATH_FORMAT "ficheros_tramas/tramas_zulo/%dzulo.txt.bin"

//Tamaño máximo permitido de la ruta de acceso a los ficheros.
#define FILE_PATH_SIZE 50

#define BIT_MASK 0x80 //Máscara utilizada para manejar bits.

#define FACT_SEG_USEG 1000000 //Factor de conversion de seg a microseg.

#define BYTE1_DATOS 10 //1º byte de datos del comando.

// Byte del comando que contiene el número de bytes de datos.
#define BYTE_SIZE_CMD 6 

#define MSB_TBIT 7 // Byte más significativo del periodo de bit.

#define LSB_TBIT 8 // Byte menos significativo del periodo de bit.

// Byte del comando que contiene el periodo de la portadora
#define BYTE_TCARRIER 9

// Número de bits de una transferencia spi, incluyendo el bit de parada.
#define NBITS_SPI 9

#define NBITS_BYTE 8 // Byte menos significativo del periodo de bit.

// Tamaño del array donde se almacena el comando sin modular.
#define TAM_CMD (2 x 1024) // 2KB

// Tamaño del array donde se almacena el comando sin modular.
#define TAM_CMDMOD (10 x 1024) // 10KB


// struct comando. Estructura donde se va a almacenar 
// el comando que hay que enviar pero sin estar modulado.
  
struct comando {
    unsigned char datos [TAM_CMD]; //Array donde se guarda el comando.
    long pos; // Posición del array que se está usando. 
};

// struct cmd\_modulado. Estructura que contiene 
// el comando ya modulado.

struct cmd_modulado {
    unsigned char datos [TAM_CMDMOD]; // Array donde se almacena 
                                      // el comando modulado. 
    long pos; // Posición del array que se está usando. 
    int f_bit; // Frecuencia de bit usada para enviar el comando por SPI.
};


int seleccionar_setpoint (int temp_control)

// Esta función determina la temperatura del aire acondicionado
// a partir de la señal de control generada por el PID.
 
  @param int temp_control // Valor de la señal de control en milésimas.

  @return int // Devuelve un entero con el setpoint a fijar.


int obtener_comando (int temp_setpoint, struct comando* cmd)

// Esta función busca el fichero asociado al setpoint pasado como
// argumento y copia su contenido en un estructura de tipo comando.

  @param int temp_setpoint // Valor del comando a seleccionar.

  @param struct comando* cmd // Estructura que almacenará el comando.

  @return int // Devuelve 0 si todo ha ido bien. Si hay error, da -1.


static void modular_bit (int value, int pos_init, int size_bit_mod, struct cmd_modulado* cmd)

// Función estática que realiza la modulación de un bit de un comando.

  @param int value // Valor del bit que se quiere modular.

  @param int pos_init // Posición (en una trans SPI) del 1º bit.

  @param int size_bit_mod // Tamaño de la secuencia sin bits de parada.

  @param struct cmd_modulado* cmd_mod // Puntero hacia el cmd mod.             

  @return void // No devuelve nada.


int modular_comando(struct comando* cmd, struct cmd_modulado* cmd_mod)
 
// Función que modula el comando a transmitir. 

  @param struct comando* cmd // Puntero hacia el comando sin modular.

  @param struct cmd_modulado* cmd_mod // Puntero hacia el cmd mod.  

  @return int // Devuelve 0 si todo ha ido bien. En caso de error, da -1.
