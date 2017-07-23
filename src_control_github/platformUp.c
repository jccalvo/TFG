/**	@file PlataformaUp.c
  * 
  *	@brief Módulo que contiene las funciones necesarias para	
  * poder subir un dato a la plataforma Graphite.
  *
  *	@author Juan Carlos Calvo Sansegundo
  * @date 25/05/2017 
  */


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>


void enviar_datos(int a, int b, int c, int timestamp){

	struct sockaddr_in cliente; //Declaración de la estructura con información para la conexión
    struct hostent *servidor; //Declaración de la estructura con información del host

    
    int puerto, conexion;
    int fd;
    FILE *f;

    servidor = gethostbyname("visualizee.die.upm.es");

    if (servidor == NULL)
    {
    	printf("Host erróneo\n");
	return ;
    }

  	puerto = 3003;
	conexion = socket(AF_INET, SOCK_STREAM, 0); //Asignación del socket
  	bzero((char *)&cliente, sizeof((char *)&cliente)); //Rellena toda la estructura de 0's
        
	cliente.sin_family = AF_INET; //asignacion del protocolo
	cliente.sin_port = htons(puerto); //asignacion del puerto

	bcopy((char *)servidor->h_addr, (char *)&cliente.sin_addr.s_addr, sizeof(servidor->h_length));
  	//bcopy(); copia los datos del primer elemendo en el segundo con el tamaño máximo 
  	//del tercer argumento.

    fd = connect(conexion,(struct sockaddr *)&cliente, sizeof(cliente)); 
	if(fd < 0)
  	{ //conectando con el host
  		printf("Error conectando con el host\n");
  		close(conexion);
		return ;
  	}

  	f = fdopen(fd,"w");

  	if (f == NULL) 
    	{
    	    printf("Error conectando con el host\n");
	    return ;
    	}

    


    fprintf(f,"visualizee.greencpd.b039.cooler.temperature.supply.setpoint.2 %d %d\n",a, timestamp);
    fprintf(f,"visualizee.greencpd.b039.cooler.temperature.supply.control.2 %d.%2d %d\n",b/1000, abs(b%1000), timestamp);
    fprintf(f,"visualizee.greencpd.b039.cooler.temperature.supply.optimized.2 %d.%d %d\n",c/1000, abs(c%1000), timestamp);
   
    fclose(f);

}

/*int main(){
    
    int a = 22.0;
    int b = 25.0;
    int c = 30.0;
    
    int d = 0;
    int timestamp = 1495728765;

    while (d<=5){
 
       enviar_datos(a, b, c, timestamp);
       timestamp += 10;
       d++;
    }
}  */