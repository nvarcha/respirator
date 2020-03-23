/**
 * Respirator project - GNU
 * 
 * GNU GENERAL PUBLIC LICENSE - Version 3, 29 June 2007
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
bool ev2_status = CLOSED;
bool ev3_status = CLOSED;
bool ev4_status = CLOSED;
bool ev5_status = CLOSED;

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
int fc2_value = 0;

/**
 * Flow-control thresholds
 * These thresholds identified values that, when greater or equal, are 
 * considered a positive flow
 */
const int fc1_active_threshold = 400;
const int fc2_active_threshold = 400;

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
bool diagnostic_errors = false;

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

  debug("Initializing Respirator Project - v0.0.1");
  debug("https://github.com/nvarcha/respirator");
  
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

  // Skip look if we had diagnostic errors
  if (diagnostic_errors) {
    return;
  }
  
  fc1Read();
  debug("FC1 value");
  debug(fc1_value);

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
  debug("Starting diagnostics");

  // TEST O2 intake
  // Open O2 valve and wait
  ev1Open();
  delay(1000);
  
  // Check O2 flowmeter
  fc1Read();
    
  // Are we getting a positive flow?
  if (!fc1Positive()) {
    // If not, alert (sound, email, whatever)
    debug("****** ERROR: O2 FLOW CONTROL NOT POSITIVE ******");
    diagnostic_errors = true;
  }
  
  // Close O2 valve
  ev1Close();

  // TEST Air intake
  // Open Air valve
  ev2Open();
  delay(1000);
  
  // Check Air flowmeter
  fc2Read();
  
  // Are we getting a positive flow?
  if (!fc2Positive()) {
    // If not, alert (sound, email, whatever)
    debug("****** ERROR: AIR FLOW CONTROL NOT POSITIVE ******");
    diagnostic_errors = true;
  }
  
  // Close Air valve
  ev2Close();

  if (diagnostic_errors) {
    debug("Diagnostic errors detected. Cannot start. Read the log and fix");
  }
}

/**
 * Simple debug using Serial.println
 * It's wrapped inside an #ifdef DEBUG to easily remove outputs
 */
void debug(const char *text) {
  #ifdef DEBUG
  Serial.println(text);
  #endif
}

/*********************************/
/**** Electro vale operations ****/
/*********************************/
void ev1Open() {
  debug("Opening O2 valve");
  digitalWrite(ev1_o2_in, HIGH);
  ev1_status = OPEN;
}

void ev1Close() {
  debug("Closing O2 valve");
  digitalWrite(ev1_o2_in, LOW);
  ev1_status = CLOSED;
}

void ev2Open() {
  debug("Opening Air valve");
  digitalWrite(ev2_air_in, HIGH);
  ev2_status = OPEN;
}

void ev2Close() {
  debug("Closing Air valve");
  digitalWrite(ev2_air_in, LOW);
  ev2_status = CLOSED;
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
 * Read the FC2 value
 */
void fc2Read() {
  fc2_value = analogRead(fc2_air);
}

/**
 * Returns true when the value of the FC1 is
 * equal or greater than the specified threshold
 */
bool fc1Positive() {
  return (fc1_value >= fc1_active_threshold);
}

/**
 * Returns true when the value of the FC2 is
 * equal or greater than the specified threshold
 */
bool fc2Positive() {
  return (fc2_value >= fc2_active_threshold);
}
