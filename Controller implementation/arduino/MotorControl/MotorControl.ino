/**
 * Created on Thu Oct 22 10:50 2020
 * @author: Elkin Javier Guerra Galeano
 * MotorControl.ino
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
//float right_angle = 0.0;

// Variables relacionadas al actuador del sistema 
const int pwmPulsesLeftcw = 5;
const int pwmPulsesLeftuw = 6; 
const int pwmPulsesRightcw = 9;
const int pwmPulsesRightuw = 10; 

// Variables relacionadas a los registros de desplazamiento para el algoritmo de control
double setpoint_k_i[] = {0.0,0.0,0.0};
double prefilter_k_i[] = {0.0,0.0,0.0};
double error_k_i[] = {0.0,0.0,0.0};
double controlsignal_k_i[] = {0.0,0.0};
double Ks = 0.4; 

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
    lastTime = millis(); // Almacenamos el tiempo actual 
    left_angle = (_LeftEncoderTicks*6.283185307179586)/ENC_COUNT_REV; // Calculamos el argulo en radianes

      // Datos recibidos
    if (stringComplete){
      setpoint_k_i[0] = Ks*inputString.toInt();
      inputString = "";
      stringComplete = false;  
    }

    // Control algorithm
    controlAlgorithm();

    // Mandamos los datos para graficarlos
    Serial.println(left_angle);
    //Serial.println(controlsignal_k_i[0]);
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
