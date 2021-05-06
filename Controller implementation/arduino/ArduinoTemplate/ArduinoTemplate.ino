/**
 * Created on Thu Oct 22 10:50 2020
 * @author: Elkin Javier Guerra Galeano
 * ArduinoTemplate.ino
 */

// Importamos librerias importantes a utilizar 
#include <digitalWriteFast.h> // Libreria encargada de la lectura rapida de los pines

// Definimos los pines y variables a utilizar 
#define ENC_COUNT_REV 300;//256;  // Numero de pulsos por revolucion del enconder de cuadratura 
unsigned long lastTime = 0 , sampleTime = 2; // Variables temporales importantes 

// Encoders de cuadratura
// Encoder izquierdo
#define c_LeftEncoderInterrupt 0
#define c_LeftEncoderPinA 2
#define c_LeftEncoderPinB 4
#define LeftEncoderIsReversed
volatile bool _LeftEncoderBSet;
volatile long _LeftEncoderTicks = 0;
double left_angle = 0.0;

// Encoder derecho
//#define c_RightEncoderInterrupt 5
//#define c_RightEncoderPinA 18
//#define c_RightEncoderPinB 24
//volatile bool _RightEncoderBSet;
//volatile long _RightEncoderTicks = 0;
//float right_angle = 0;

// Variables relacionadas al actuador del sistema 
double pwm = 0; // Ciclo de trabajo
double error = 0.0;
const int pwmPulsesLeftcw = 5;
const int pwmPulsesLeftuw = 6; 
const int pwmPulsesRightcw = 9;
const int pwmPulsesRightuw = 10; 

// Variables de comunicacion serial 
String inputString = ""; // String para contener datos entrantes 
bool stringComplete = false; // Bandera que indica la recepcion de un dato nuevo 


void setup() {
  
  // Configuramos la comunicacion serial 
  Serial.begin(115200);

  // Salidas de pwm
  pinMode(pwmPulsesLeftcw,OUTPUT);
  pinMode(pwmPulsesLeftuw,OUTPUT);
  pinMode(pwmPulsesRightcw,OUTPUT);
  pinMode(pwmPulsesRightuw,OUTPUT);
 
  // Encoders de cuadratura
  // Encoder izquierdo
  pinMode(c_LeftEncoderPinA, INPUT);      // Definimos pin A como entrada
  digitalWrite(c_LeftEncoderPinA, LOW);  // Habilitamos el pullup interno
  pinMode(c_LeftEncoderPinB, INPUT);      // Definimos el pin B como entrada
  digitalWrite(c_LeftEncoderPinB, LOW);  // Habilitamos el pullup interno
  attachInterrupt(c_LeftEncoderInterrupt, HandleLeftMotorInterruptA, RISING);
 
  // Encoder derecho
//  pinMode(c_RightEncoderPinA, INPUT);      // Definimos pin A como entrada
//  digitalWrite(c_RightEncoderPinA, LOW);  // Habilitamos el pullup interno
//  pinMode(c_RightEncoderPinB, INPUT);      // Definimos el pin B como entrada
//  digitalWrite(c_RightEncoderPinB, LOW);  // Habilitamos el pullup interno
//  attachInterrupt(c_RightEncoderInterrupt, HandleRightMotorInterruptA, RISING);

}

void loop() {

  // Control loop
  if (millis() - lastTime >= sampleTime){
    noInterrupts(); // Desconectamos la interrupcion para que no actue en esta parte del programa
    //rpm = 0.1*((_LeftEncoderTicks*6283.185307179586)/((millis() - lastTime)*ENC_COUNT_REV)) + 0.9*rpm; // Calculamos las revoluciones por minuto
    //rpm = (_LeftEncoderTicks*6283.185307179586)/((millis() - lastTime)*ENC_COUNT_REV);
    lastTime = millis(); // Almacenamos el tiempo actual 
    //_LeftEncoderTicks = 0; // Reset pulses 
    //Serial.println(rpm);
    left_angle = (_LeftEncoderTicks*6.283185307179586)/ENC_COUNT_REV;

      // Datos recibidos
    if (stringComplete){
      pwm = inputString.toInt();
      inputString = "";
      stringComplete = false;  
    }

    // Control action
    error = pwm - left_angle;
    controlSignal(error,"left");
    
    Serial.println(left_angle);
    interrupts(); // Restart the interrupt precesing  
  }
}

// Funcion para recibir los datos que llegan por serial 
void serialEvent(){
  while (Serial.available()){
    char inChar = (char)Serial.read();
    inputString += inChar;
    if (inChar == '\n'){
      stringComplete = true;
    }  
  }  
}

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

// // Interrupciones asociadas el encoder de cuadratura del motor izquierdo 
void HandleLeftMotorInterruptA()
{
  // Test transition; since the interrupt will only fire on 'rising' we don't need to read pin A
  _LeftEncoderBSet = digitalReadFast(c_LeftEncoderPinB);   // read the input pin
 
  // and adjust counter + if A leads B
  #ifdef LeftEncoderIsReversed
    _LeftEncoderTicks -= _LeftEncoderBSet ? -1 : +1;
  #else
    _LeftEncoderTicks += _LeftEncoderBSet ? -1 : +1;
  #endif
}
 
// // Interrupciones asociadas el encoder de cuadratura del motor derecho
//void HandleRightMotorInterruptA()
//{
//  // Test transition; since the interrupt will only fire on 'rising' we don't need to read pin A
//  _RightEncoderBSet = digitalReadFast(c_RightEncoderPinB);   // read the input pin
// 
//  // and adjust counter + if A leads B
//  #ifdef RightEncoderIsReversed
//    _RightEncoderTicks -= _RightEncoderBSet ? -1 : +1;
//  #else
//    _RightEncoderTicks += _RightEncoderBSet ? -1 : +1;
//  #endif
//}
