#include <IRremote.h>

// pins
const int click_pin = 2;
const int motor_in3 = 4;
const int motor_in4 = 3;
const int motor_dir_pin = 10;
const int motor_step_pin = 9;
const int enable = 13;
const int steps_per_rev = 3000;
const int receiver_pin = 7;
const int brake_in1 = 1; 
const int brake_in2 = 2;
const int brake_enable = 12;

// setup receiver
IRrecv irrecv(receiver_pin);
decode_results results;

// remote variables
const int UP_VAL = -26521;
const int DOWN_VAL = 14535;
const int LEFT_VAL = 12495;
const int RIGHT_VAL = 31365;
int remote_val = 0;


// setup function
void setup() {
  pinMode(click_pin, INPUT_PULLUP);
  pinMode(motor_in3, OUTPUT);
  pinMode(motor_in4, OUTPUT);
  pinMode(enable, OUTPUT);
  pinMode(motor_dir_pin, OUTPUT);
  pinMode(motor_step_pin, OUTPUT);

  pinMode(brake_in1, OUTPUT);
  pinMode(brake_in2, OUTPUT);
  pinMode(brake_enable, OUTPUT);
  
  
  irrecv.enableIRIn();
  digitalWrite(motor_dir_pin, HIGH);
  
  Serial.begin(9600);
}


// loop
void loop() {

  // set actuator & brake motor speed
  analogWrite(enable, 500);
  analogWrite(brake_enable, 500);

  // turn actuator off
  digitalWrite(motor_in3, LOW);
  digitalWrite(motor_in4, LOW);

  // turn brake on
  digitalWrite(brake_in1, HIGH);
  digitalWrite(brake_in2, HIGH);
  
  // wait until any button is pressed
  while(!(irrecv.decode(&results)));
  irrecv.resume();
  
  print_remote_val(results.value);

  // if button being held, don't update remote_val
  if (int(results.value) != -1) {
    remote_val = results.value;
  }

  // change rotational motor direction based on button
  if (remote_val == LEFT_VAL) {
    digitalWrite(motor_dir_pin, HIGH);
  } else if (remote_val == RIGHT_VAL) {
    digitalWrite(motor_dir_pin, LOW);
  }

  // choose proper motor action based on remote value
  if (remote_val == LEFT_VAL || remote_val == RIGHT_VAL) {

      // turn brake off
      digitalWrite(brake_in1, LOW);
      digitalWrite(brake_in2, LOW);

      // complete N steps
      for(int i = 0; i < steps_per_rev; i++) {
        digitalWrite(motor_step_pin, HIGH);
        delayMicroseconds(100); 
        digitalWrite(motor_step_pin, LOW); 
        delayMicroseconds(100);
      }
    } else if (remote_val == UP_VAL) {
        digitalWrite(motor_in3, LOW);
        digitalWrite(motor_in4, HIGH);
        delay(500);
    } else if (remote_val == DOWN_VAL) {
        digitalWrite(motor_in3, HIGH);
        digitalWrite(motor_in4, LOW);
        delay(500);
    } 

}


// print remote val
void print_remote_val(int val) {
  Serial.println(" ");
  Serial.print("Code: ");
  Serial.println(val);
  Serial.println(" ");
}
