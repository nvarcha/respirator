#include <LiquidCrystal.h>
//Pines de LCD
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);        //Pines de salida para el LCD


//Pines Digitales
const int dirPin = 2;                       // Pin de Direccion Motor
const int stepPin = 3;                      // Pin de Pasos Motor
const int ZeroCal = 11;                     // Pin para el autocal
int ValveOut = 12;                          // Pin Salida de Relay para ElectroValvula
const int ValveSensor = 13;                 // Sensor para llenado de la bolsa


//Pines Analogicos
int lcd_key     = 0;                        // Entrada de Botones
int adc_key_in  = 0;                        // Entrada ADC botones

//Variables Botones
bool btnRIGHT = false;                      // Boton de Derecha
bool btnUP    = false;                      // Boton de Arriba
bool btnDOWN  = false;                      // Boton Abajo
bool btnLEFT  = false;                      // Boton Izquierda
bool btnSELECT = false;                     // Boton Seleccionar
bool btnNONE   = false;                     // Sin apretar nada

//FLAGS de funcionamiento

bool inicio = false; 

// Frecuencia del motor y resolucion
int freq = 200;                             // Frecuencia
const int stepsPerRevolution = 1600;        // Resolucion de pasos


//Funciones
void motor();                               // Funcion para control de motor
void electrovalvula();                      // Funcion para control de la electrovalvula
void LCD_Display();                         // Funcion para control del display
void autocal();                             // Funcion para la calibracion y control del motor 
void botones();                             // Comportamiento de los botones

void setup() {

//LCD Begin
lcd.begin(16, 2);              // Inicializar el LCD
      lcd.setCursor(0,0);

// Seteo de caracteristicas de Entradas/Salidas

  pinMode(ZeroCal, INPUT); 
  pinMode(ValveOut,OUTPUT);
  pinMode(ValveSensor, INPUT);
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
}

void loop() {
  
LCD_Display();
botones();
autocal();
motor();

}
