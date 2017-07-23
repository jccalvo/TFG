#define FACT_CONV_TEMP 1000 // Factor de conversión a milésimas.

#define PET_HTTP_OK 200 // Petición http satisfactoria.

#define CONEXION_OK 0 // Conexión satisfactoria con la url .

#define SIZE_MEM (5*1024) // Tam máx del buffer para el objeto JSON. 

// Formato de la URL del objeto JSON. 
#define URL_FORMAT "http://visualizee.die.upm.es:8000/render?format=json&target=visualizee.greencpd.b039.rack.r0001.server.host2.temperature.ambient.1.1000&from=-1min"   


// struct objJSON.  

// Estructura donde se almacena el objeto JSON 
// descargado de la plataforma.

struct objJSON{
    char datos[SIZE_MEM]; // Array reservado para almacenar el objeto.
    int size; //Tamaño en bytes del objeto (Número de posiciones usadas).
};


// struct temp\_leida.
  
// Estructura donde se almacena el dato de temperatura solicitado
// y que ha sido extraído previamente de un objeto JSON.
 
struct temp_leida{
    int valor;     // valor de la temperatura 
    int timestamp; // instante de medida de la temperatura
};

static size_t escribir_objeto (void * ptr, size_t size, size_t nmemb, struct objJSON* obj)

//  Funcion estática que escribe el objeto descargado
//  en una estructura objJSON creada previamente para almacenarlo.

  @param void* ptr // Puntero al objeto JSON que se quiere escribir. 

  @param size_t size // Posiciones de memoria que ocupa el obj JSON. 

  @param size_t nmemb // Bytes que ocupa una pos de memoria del obj.

  @param struct objJSON* obj // Puntero a la estr que guardará el obj.

  @return size_t // Devuelve el nºbytes copiados. Si hay error, da -1.


void solicitar_objeto (const char * url, struct objJSON* obj);    

//  Función que realiza la petición http para obtener objeto JSON y copia 
//  objeto en la estructura objJSON. Usa la librería curl para la pet http
//  y la función estática escribir\_objeto() para copiar el objeto

  @param const char* url // Puntero a un array con la URL del obj JSON.

  @param struct objJSON* obj // Puntero a la estr que guardará el obj.

  @return void // No devuelve nada.
  

int extraer_temperatura (struct objJSON* obj, struct temp_leida* temperatura)

// Función que procesa el objeto JSON y le extrae el dato de temperatura.
// Utiliza la librería jansson para extraer el dato del objeto JSON.

   @param struct objJSON* obj // Puntero a la estr que contiene el obj.

   @param struct temp_leida* temperatura 
   // Puntero a la estructura donde se va a escribir el dato extraído. 

   @return int // Devuelve 0 si se ha extraído el dato. Sino, devuelve -1.
  

