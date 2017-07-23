static  size_t escribir_objeto (void *ptr, size_t  size, size_t  nmemb, struct objJSON * obj )
{
  // Declaracion de variables.
  curl_global_init (CURL_GLOBAL_ALL); // Inicializar librería.
  curl = curl_easy_init(); // Inicializa y crea el curl.
  if (Error) {
	// Error: curl no inicializado.
  }
  // Configuracion del curl.
  curl_easy_setopt(curl, CURLOPT_URL, url); 
  curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION, escribir_objeto); 
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, obj->datos); 

  // Obtencion del estado del curl y resultado de la petición.
  estado = curl_easy_perform(curl);
  curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &codigo);
  if (estado = OK & codigo = PET_HTTP_OK) {
	  // Todo correcto.
  }
  else{
  	  curl_easy_strerror(estado);   // Informar del error.
  }	
  curl_easy_cleanup(curl);   // Cierre del curl.
  curl_global_cleanup();   // Cierre de la librería.
}