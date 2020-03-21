/**
 * Respirator project - GNU
 * 
 * This is free and delivered AS IS. Use at your own risk.
 */

// Set to 1 to enable serial debug, 0 to disable it
#define DEBUG 1

// Start/stop (probably temporary)
const int pin_start_stop_button = 2;
volatile bool poweredOn = false;

// O2 valve relay control pin
const int pin_o2_in_valve = 11;

// O2 valve opened (false = closed, true = open)
bool o2_in_valve_opened = false;

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
  pinMode(pin_o2_in_valve, OUTPUT);

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

  // Early out if we're powered off
  if (!poweredOn) {
    return;
  }
  
  
  digitalWrite(pin_o2_in_valve,HIGH);
  delay(1000);
  digitalWrite(pin_o2_in_valve,LOW);
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
}
