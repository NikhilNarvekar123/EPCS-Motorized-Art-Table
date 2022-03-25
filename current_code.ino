#define joyX A8
#define joyY A9


int clickPin = 2;

void setup() {
  pinMode(clickPin, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() {
  float xValue = analogRead(joyX);
  float yValue = analogRead(joyY);
  int clickValue = digitalRead(clickPin);

  Serial.print("x:");
  Serial.print(xValue, 2);
  Serial.print("\t y:");
  Serial.print(yValue, 2);
  Serial.print("\t c:");
  Serial.print(clickValue);
  Serial.println("\n");
  delay(100);
}
