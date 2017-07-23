int extraer_temperatura (struct objJSON* obj, struct temp_leida* temperatura) 
{
  // Declaración de manejadores del objeto JSON, campos y errores.
  json_t* obj_handler, datapoints, valor, inst_medida;
  json_error_t errores; 
          
  obj_handler = json_loads(); // Carga del objeto en el manejador.  
  if (!obj_handler) {
	  // Error:No se puede cargar;
	  return -1;
  } 
  // Extracción del datapoints.
  if (error) {
	  // Error: No existe el datapoints
	  return -1;
  }
  // Extracción del dato mas reciente.
  for(i=0; i< json_array_size(obj_handler); i++) {
	  // Extracción del datapoint i;
	  // Extracción de los atributos valor e inst\_medida;
	  if (OK) {  
		  // Copia del dato en la estructura temperatura;
	  }
  }
  return 0;
}