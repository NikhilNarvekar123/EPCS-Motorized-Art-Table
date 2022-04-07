#include <IRremote.h>

const int click_pin = 2;
const int motor_in3 = 4;
const int motor_in4 = 3;
const int enable = 13;
int receiver_pin = 7;

// setup receiver
IRrecv irrecv(receiver_pin);
decode_results results;

// remote variables
const unsigned int U_MAX_INT = 4294967295;
const int UP_VAL = 16750695;
const int DOWN_VAL = 16726215;
const int LEFT_VAL = 10; //temp
const int RIGHT_VAL = 11; //temp
int remote_val = 0;


// setup function
void setup() {
  pinMode(click_pin, INPUT_PULLUP);
  pinMode(motor_in3, OUTPUT);
  pinMode(motor_in4, OUTPUT);
  pinMode(enable, OUTPUT);
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
        break;
      case DOWN_VAL:
        digitalWrite(motor_in3, LOW);
        digitalWrite(motor_in4, HIGH);
        break;
      case LEFT_VAL:
        break;
      case RIGHT_VAL:
        break;
    }
    
  } else {
    // no remote value so all motors off
    digitalWrite(motor_in3, LOW);
    digitalWrite(motor_in4, LOW);
  }

  // increment/decrement to adjust motor smoothness
  delay(500);
}


// print remote val
void print_remote_val(int val) {
  Serial.println(" ");
  Serial.print("Code: ");
  Serial.println(val);
  Serial.println(" ");
}
