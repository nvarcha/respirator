/**
 * Respirator project - GNU
 * 
 * Copyright (c) 2020 - Nicolas Varchavsky, Joel Ramos Pacheco, Sebastian Bourre - All rights reserved
 */

// Set to 1 to enable serial debug, 0 to disable it
#define DEBUG 1
#define OPEN true
#define CLOSED false

/********************************/
/************ PINS **************/
/********************************/
 
/**
 * Pressure sensors pin numbers
 * These are read through I2C
 */
const int ps1_o2_in = 0;
const int ps2_air_in = 0;
const int ps3_mixed_gas_in = 0;
const int ps4_input_gas_in = 0;

/**
 * Eletro-valves pin numbers
 * These are activated through relay boards
 */
const int ev1_o2_in = 11;
const int ev2_air_in = 12;
const int ev3_mixed_gas_out = 13;
const int ev4_input_gas = 14;
const int ev5_output_gas = 15;

/**
 * Electro-valves status
 */
bool ev1_status = CLOSED;

/**
 * Flow-control flowmeter or rotameter.
 * These are to control and sense flow and handled through I2C
 * TODO: I'm temporarily using pure analog until I can get something
 * that has I2C compatibility
 */
const int fc1_o2 = A5;
const int fc2_air = A4;
const int fc3_gas = A3;

/**
 * Flow-control read values
 * Stored mostly for debugging and feedback purposes
 */
int fc1_value = 0;

/**
 * Flow-control thresholds
 * These thresholds identified values that, when greater or equal, are 
 * considered a positive flow
 */
 const int fc1_active_threshold = 400;

/**
 * Possible pressure sensor used to sense if the patient is breathing
 * on her own
 */
const int s1_breathing = 0;

 /**
  * Actuator servo or stepper motor to inflate and deflate the AMBU bag
  */
const int act1_ambu = 0;

/*********************************************/
/************ GENERAL VARIABLES **************/
/*********************************************/

// Start/stop (probably temporary)
const int pin_start_stop_button = 2;
volatile bool poweredOn = false;

/**
 * Initial setup code
 * This will run only once
 */
void setup() {

  // Initialize serial object
  #ifdef DEBUG
  Serial.begin(9600);
  #endif

  #ifdef DEBUG
  Serial.println("Initializing Respirator Project - v0.0.1");
  Serial.println("Public Domain - As is - Use at your own risk");
  Serial.println("https://github.com/nvarcha/respirator");
  #endif
  
  // Configure the following pins as output pins
  pinMode(ev1_o2_in, OUTPUT);

  // Configure the following pins as input pins
  // INPUT_PULLUP inverts the behaviour so HIGH means off and LOW means on
  pinMode(pin_start_stop_button, INPUT_PULLUP);

  // Configure interrupt to read on/off button
  attachInterrupt(digitalPinToInterrupt(pin_start_stop_button), togglePower, CHANGE);

  // Run initial diagnostics
  startupDiagnostics();  
}

/**
 * Switches power on and off
 * This is not that important. You can always unplug it from the outlet
 * and make it start automatically, but during development it's easier
 */
void togglePower() {
 poweredOn = digitalRead(pin_start_stop_button) == LOW; 
}

/**
 * Main loop
 * This is repeated forever as long as the board has power
 */
void loop() {

  fc1Read();
  Serial.println("FC1 value");
  Serial.println(fc1_value);
  Serial.println("FC1 status");
  Serial.println(fc1Positive());

  // Early out if we're powered off
  if (!poweredOn) {
    return;
  }

  ev1Open();
  delay(1000);
  ev1Close();
  delay(1000);
}

/**
 * Run all initial diagnostics
 * The idea is to be able to test each element. Some will require human intervention
 * to asses the result. Others, a self-diagnosis may be possible.
 */
void startupDiagnostics() {
  #ifdef DEBUG
  Serial.println("Starting diagnostics");
  #endif

  // TEST O2 intake
  // Open O2 valve
  // Check O2 flowmeter
  // Are we getting a positive flow?
  // If not, alert (sound, email, whatever)
  // Close O2 valve

  // TEST Air intake
  // Open Air valve
  // Check Air flowmeter
  // Are we getting a positive flow?
  // If not, alert (sound, email, whatever)
  // Close Air valve

  
  
}

/*********************************/
/**** Electro vale operations ****/
/*********************************/
void ev1Open() {
  digitalWrite(ev1_o2_in,HIGH);
}

void ev1Close() {
  digitalWrite(ev1_o2_in,LOW);
}


/*********************************/
/**** Flow control operations ****/
/*********************************/

/**
 * Read the FC1 value
 */
void fc1Read() {
  fc1_value = analogRead(fc1_o2);
}

/**
 * Returns true when the value of the FC1 is
 * equal or greater than the specified threshold
 */
bool fc1Positive() {
  return (fc1_value >= fc1_active_threshold);
}
