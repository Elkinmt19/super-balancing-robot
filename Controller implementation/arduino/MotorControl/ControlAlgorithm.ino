// Aqui realizamos el algoritmo de control a implementar en el sistema
void controlAlgorithm(){
  // Control action

  // Definimos el prefiltro
  prefilter_k_i[0] = 1.989436724795*prefilter_k_i[1] - 0.9894904866541*prefilter_k_i[2] 
  + 0.00005376185959871*setpoint_k_i[2];

  // Calculamos el error 
  error_k_i[0] = prefilter_k_i[0] - left_angle;

  // Definimos el controlador PID a implementar en el sistema 
  controlsignal_k_i[0] = 10748.23362068*error_k_i[0] - 21382.93069165*error_k_i[1]
  + 10635.274916*error_k_i[2] + controlsignal_k_i[1];
  // Ejecutamos la accion de control 
  controlSignal(controlsignal_k_i[0],"left");
  
  // Actualizamos los registros de desplazamiento
  setpoint_k_i[2] = setpoint_k_i[1];
  setpoint_k_i[1] = setpoint_k_i[0];
  prefilter_k_i[2] = prefilter_k_i[1];
  prefilter_k_i[1] = prefilter_k_i[0];
  error_k_i[2] = error_k_i[1];
  error_k_i[1] = error_k_i[0];
  controlsignal_k_i[1] = controlsignal_k_i[0];
  
}
