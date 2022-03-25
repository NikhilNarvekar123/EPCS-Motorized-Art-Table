#include "nunchuk.h"
#include <Wire.h>


void setup() {
  Serial.begin(9600);
  Wire.begin();
  nunchuk_init();
}

void loop() {
  if(nunchuk_read()) {
    nunchuk_print();
    Serial.print(1);
  }
  delay(10);
}
