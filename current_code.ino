#define joy_x A8
#define joy_y A9

// define digital pins
const int click_pin = 2;
const int motor_in3 = 4;
const int motor_in4 = 3;
const int enable = 13;

// setup function
void setup() {
  pinMode(click_pin, INPUT_PULLUP);
  pinMode(motor_in3, OUTPUT);
  pinMode(motor_in4, OUTPUT);
  pinMode(enable, OUTPUT);

  // serial monitor
  Serial.begin(9600);
}

// loop every 100 ms
void loop() {

  // read joystick input
  float x_val = analogRead(joy_x);
  float y_val = analogRead(joy_y);
  int click_val = digitalRead(click_pin);

  // set actuator motor speed
  analogWrite(enable, 500);

  // based on joystick x, set motor direction
  if (x_val > 360.00) {
    digitalWrite(motor_in3, HIGH);
    digitalWrite(motor_in4, LOW);
    Serial.print("UP\n");
  } else if (x_val < 340.00) {
    digitalWrite(motor_in3, LOW);
    digitalWrite(motor_in4, HIGH);    
    Serial.print("DOWN\n");
  } else {
    digitalWrite(motor_in3, LOW);
    digitalWrite(motor_in4, LOW);
    Serial.print("ZERO\n");
  }
  
  // for testing
  print_vals(x_val, y_val, click_val);
  
  delay(100);
}

void print_vals(float val1, float val2, int val3) {
  Serial.print("x:");
  Serial.print(val1, 2);
  Serial.print("\t y:");
  Serial.print(val2, 2);
  Serial.print("\t c:");
  Serial.print(val3);
  Serial.println("\n");
}
