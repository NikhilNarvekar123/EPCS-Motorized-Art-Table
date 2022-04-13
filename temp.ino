#include <IRremote.h>

const int click_pin = 2;
const int motor_in3 = 4;
const int motor_in4 = 3;
const int motor_dir_pin = 10;
const int motor_step_pin = 9;
const int enable = 13;
const int steps_per_rev = 1000;
const int receiver_pin = 7;

// setup receiver
IRrecv irrecv(receiver_pin);
decode_results results;

// remote variables
const unsigned int U_MAX_INT = 4294967295;
const int UP_VAL = 16750695;
const int DOWN_VAL = 16726215;
const int LEFT_VAL = 16743045; //temp
const int RIGHT_VAL = 16724175; //temp
int remote_val = 0;


// setup function
void setup() {
  pinMode(click_pin, INPUT_PULLUP);
  pinMode(motor_in3, OUTPUT);
  pinMode(motor_in4, OUTPUT);
  pinMode(enable, OUTPUT);
  pinMode(motor_dir_pin, OUTPUT);
  pinMode(motor_step_pin, OUTPUT);
  irrecv.enableIRIn();
  
  Serial.begin(9600);
}


// loop every 500 ms
// delay incremented for smoother IR operation
void loop() {

  // set actuator motor speed
  analogWrite(enable, 500);

  // 1 if button pressed, 0 otherwise
  if (irrecv.decode(&results)) {

    // using ints has lot smaller values, use instead??
    Serial.println(results.value);
    print_remote_val(results.value);
    irrecv.resume();

    // update remote variable only if button is not
    // being pressed down
    if (results.value != U_MAX_INT) {
      remote_val = results.value;
    }
    
    // based on remote_val set motor direction
    switch (remote_val) {
      case UP_VAL:
        digitalWrite(motor_in3, HIGH);
        digitalWrite(motor_in4, LOW);
        delay(500);
        break;
      case DOWN_VAL:
        digitalWrite(motor_in3, LOW);
        digitalWrite(motor_in4, HIGH);
        delay(500);
        break;
      case LEFT_VAL:
        digitalWrite(motor_dir_pin, HIGH);
        for(int x = 0; x < steps_per_rev; x++) {
          digitalWrite(motor_step_pin, HIGH);
          delayMicroseconds(10); 
          digitalWrite(motor_step_pin, LOW); 
          delayMicroseconds(10);
        }
      case RIGHT_VAL:
        digitalWrite(motor_dir_pin, LOW);
        for(int x = 0; x < steps_per_rev; x++) {
          digitalWrite(motor_step_pin, HIGH);
          delayMicroseconds(10); 
          digitalWrite(motor_step_pin, LOW); 
          delayMicroseconds(10);
        }
    }
    
  } else {
    // no remote value so all motors off
    digitalWrite(motor_in3, LOW);
    digitalWrite(motor_in4, LOW);
  }

}


// print remote val
void print_remote_val(int val) {
  Serial.println(" ");
  Serial.print("Code: ");
  Serial.println(val);
  Serial.println(" ");
}
