
/** Constants **/
const int STICK_DEADZONE = 2;
const double MOTOR_WEIGHT = 3.0 / 1023.0;
const int TEST_LIGHT = YELLOW_LED;


/** Motors **/
const int actuator_pulse = P2_4;
const int actuator_direction = P2_5;
const int actuator = P2_3;
const int rotational_motor = P2_7;

/** Controller Inputs **/
int light_btn = P1_6;
int lock_btn = P4_2;
int joystick_y = P4_1;
int joystick_x = P4_0;

/** Idk man **/
int valX = 0;           // easy to use data from joystick
int valY = 0;           // easy to use data from joystick

/** Brakes **/
bool at_min_height = false;
bool at_min_rotation = false;
bool lights_on = false;
int vertical_switch = P3_3;
int horizontal_switch = P3_2;

/** Who tf knows **/
int brake = 0;          // value used to indicate board light values
int brakeTR = 0;        // to indicate if brake is active or not
int brakeTL = 0;        // to indicate if brake is active or not
int brakeBL = 0;        // to indicate if brake is active or not
int brakeVR = 0;        // to indicate if brake is active or not
int brakeHR = 0;        // to indicate if brake is active or not
int lightA = 0;         // to indicate if light is on or not


/**  */
void update_lock() {
  at_min_height = !at_min_height;
  at_min_rotation = !at_min_rotation;
}

/**  */
void update_switches() {

  if (!digitalRead(vertical_switch)) {
    digitalWrite(actuator, HIGH);
    at_min_height = true;
  }

  if (!digitalRead(horizontal_switch)) {
    digitalWrite(rotational_motor, HIGH);
    at_min_rotation = true;
  }

}

/**  */
void update_light() {
  lights_on = !lights_on;
  digitalWrite(TEST_LIGHT, lights_on);
}



/** 
 * Run initially on power-on
 */
void setup() {

  Serial.begin(9600);
  
  // set outputs (for motors HIGH is off, LOW is on)
  pinMode(actuator_pulse, OUTPUT);     
  pinMode(actuator_direction, OUTPUT);    
  pinMode(actuator, OUTPUT);     
  pinMode(rotational_motor, OUTPUT);
  pinMode(TEST_LIGHT, OUTPUT);

  // HIGH - not pressed, LOW - pressed due to voltage issues
  pinMode(light_btn, INPUT_PULLUP); 
  pinMode(vertical_switch, INPUT_PULLUP);
  pinMode(horizontal_switch, INPUT_PULLUP);
  pinMode(lock_btn, INPUT_PULLUP);
  
  // stop control flow to immediately run controller instructions
  attachInterrupt(digitalPinToInterrupt(vertical_switch), update_switches, CHANGE);
  attachInterrupt(digitalPinToInterrupt(horizontal_switch), update_switches, CHANGE);
  attachInterrupt(digitalPinToInterrupt(light_btn), update_light, CHANGE); 
  attachInterrupt(digitalPinToInterrupt(lock_btn), update_lock, CHANGE);

}

void loop() {
 
  valY = analogRead(joystick_y) * MOTOR_WEIGHT;
  valX = analogRead(joystick_x) * MOTOR_WEIGHT;

  if(at_min_height && at_min_rotation) {
    digitalWrite(actuator, HIGH);
    digitalWrite(rotational_motor, HIGH);
  } else if(at_min_height) {
    digitalWrite(actuator_direction, LOW);
    VERTICAL_PULSE();
    digitalWrite()
  }
  

    //VERTICAL MOTION
  else if ((valY == 0) && (valX == 2)){ 
    digitalWrite(vertDir, HIGH);  // motor set to go down
    VERTICAL_PULSE();                         // moves vertical motors
  }
  else if ((valY == 3) && (valX == 2)){
    digitalWrite(vertDir, LOW);   // motor set to go up
    VERTICAL_PULSE();                         // moves vertical motors
  }
  //ROTATIONAL MOTION
     // setting direction of rotational motors
  else if ( (valX == 0) && (valY == 2) ){
     // motors rotate horizontal
  }
  else if ( (valX == 3) && (valY == 2) ){
     // motors rotate vertical
  }
  else{
    // if two directions are pressed do nothing
  }



}




/* THIS FUNCTION IS USED TO MOVE THE VERTICAL MOTORS*/
void VERTICAL_PULSE(){
  for(int x = 0; x < 50 ; x++){
    digitalWrite(vertPulse, HIGH);
    delay(1);
    digitalWrite(vertPulse, LOW);
    delay(1);
  }
}
