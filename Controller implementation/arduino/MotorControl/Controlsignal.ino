// Funcion asociada a la señal de control del sistema 
void controlSignal(int cSignal, String side){
  if (cSignal > 0){
    if (side == "right"){
      analogWrite(pwmPulsesRightcw,cSignal);
      analogWrite(pwmPulsesRightuw,0);
    }
    if (side == "left"){
      analogWrite(pwmPulsesLeftcw,cSignal);
      analogWrite(pwmPulsesLeftuw,0);
    }
  }
  if (cSignal < 0){
    if (side == "right"){
      analogWrite(pwmPulsesRightcw,0);
      analogWrite(pwmPulsesRightuw,-cSignal);
    }
    if (side == "left"){
      analogWrite(pwmPulsesLeftcw,0);
      analogWrite(pwmPulsesLeftuw,-cSignal);
    }
    
  }
  if (cSignal == 0){
    if (side == "right"){
      analogWrite(pwmPulsesRightcw,0);
      analogWrite(pwmPulsesRightuw,0);
    }
    if (side == "left"){
      analogWrite(pwmPulsesLeftcw,0);
      analogWrite(pwmPulsesLeftuw,0);
    }
    
  }
}
