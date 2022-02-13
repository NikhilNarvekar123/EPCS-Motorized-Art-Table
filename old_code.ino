 /*constants*/
const int ledr = YELLOW_LED;  // red led 1
const int ledR = RED_LED;     // red led 2
const int ledG = GREEN_LED;   // green led
const int ledB = BLUE_LED;    // blue led
const int light = P1_7;       // lamp light
const int vertPulse = P2_4;   // moves vertical motors
const int vertDir = P2_5;     // sets vertical motor direction
const int vertML = P2_3;      // enables vertical motor left
const int vertMR = P2_7;      // enables vertical motor right

/*Variable*/
int vertMotion = P4_1 ; // Y-axis from joystick
int rotMotion = P4_0;   // x-axis from joystick
int lock = P4_2;        // joystick button locks motion    
int voltageY = 0;       // data from the joystick
int voltageX = 0;       // data from the joystick        
int valX = 0;           // easy to use data from joystick
int valY = 0;           // easy to use data from joystick
int valL = 0;           // indicates whether motion is locked or not
int VBR = P3_3;         // vertical rotation brake
int HBR = P3_2;         // horizontal rotation brake
int TBL = P3_5;         // top left brake
int TBR = P3_6;         // top right brake
int BBL = P3_7;         // bottom brake
int lightB = P1_6;      // light button
int brake = 0;          // value used to indicate board light values
int brakeTR = 0;        // to indicate if brake is active or not
int brakeTL = 0;        // to indicate if brake is active or not
int brakeBL = 0;        // to indicate if brake is active or not
int brakeVR = 0;        // to indicate if brake is active or not
int brakeHR = 0;        // to indicate if brake is active or not
int lightA = 0;         // to indicate if light is on or not

void setup() {
  Serial.begin(9600);           //  setup serial
  
  /*Color setup*/
  pinMode(ledr, OUTPUT);          //  red1
  pinMode(ledR, OUTPUT);          //  red2
  pinMode(ledG, OUTPUT);          //  green
  pinMode(ledB, OUTPUT);          //  blue
  pinMode(light, OUTPUT);         //  lamp
  pinMode(lightB, INPUT_PULLUP);  // lamp button
  attachInterrupt(digitalPinToInterrupt(lightB),LIGHT,RISING); 
  
  /* VERTICAL MOTOR Setup*/
  pinMode(vertPulse, OUTPUT);     
  pinMode(vertDir, OUTPUT);    
  pinMode(vertML, OUTPUT);     
  pinMode(vertMR, OUTPUT);
  
  /*BRAKES*/
  pinMode(TBL, INPUT_PULLUP);
  pinMode(TBR, INPUT_PULLUP);
  pinMode(BBL, INPUT_PULLUP);
  pinMode(VBR, INPUT_PULLUP);
  pinMode(HBR, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(TBL),SWITCHES, LOW);
  attachInterrupt(digitalPinToInterrupt(TBR),SWITCHES, LOW);
  attachInterrupt(digitalPinToInterrupt(BBL),SWITCHES, LOW);
  attachInterrupt(digitalPinToInterrupt(VBR),SWITCHES, CHANGE);
  attachInterrupt(digitalPinToInterrupt(HBR),SWITCHES, CHANGE);

  /*MOTION LOCK*/
  pinMode(lock, INPUT_PULLUP);  // setup lock
  attachInterrupt(digitalPinToInterrupt(lock),LOCK, RISING); // lock is interrupt
}

void loop() {
 

  // if MOVEMENT CONTROL DISABLED
  if (valL == 1){
    valY = 2;     // 2 is deadzone for joystick
    valX = 2;     // 2 is deadzone for joystick
    digitalWrite(vertMR, HIGH);
    digitalWrite(vertML, HIGH);
    // needs rotational motors
    digitalWrite(ledR, HIGH); 
  }
  else if (valL == 0){
     // GETTING DATA FROM JOYSTICK AND MAKING USABLE VALUES OUT OF THE DATA
    voltageY = analogRead(vertMotion);
    voltageX = analogRead(rotMotion);
    valY = voltageY * (3.0 / 1023.0);  
    valX = voltageX * (3.0 / 1023.0);  
    digitalWrite(vertMR, LOW);
    digitalWrite(vertML, LOW);
  }

  STOPS(brakeTR, brakeTL, brakeBL, brakeVR, brakeHR, valX, valY);   // checking break switches
  
    // when nothing is happening
  if ((valY == 2) && (valX == 2)){
    //turn off all motors
    digitalWrite(vertMR, HIGH);
    digitalWrite(vertML, HIGH);
    // needs rotational motors
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

  B_LIGHT(valX,valY,valL,brake);            // board Lights

  // DISPLAYING VALUES
  Serial.println("vertMotion:");            
  Serial.println(valY);
  //Serial.println("rotMotion:");
  //Serial.println(valX);
  //Serial.println("lock:");
  //Serial.println(valL);
  //Serial.println("topbrakeright:");
  //Serial.println(brakeTR);
  //Serial.println("topbrakeleft:");
  //Serial.println(brakeTL);
  //Serial.println("botbrakeleft:");
  //Serial.println(brakeBL);
  //Serial.println("verticalrotationbrake:");
  //Serial.println(brakeVR);
  //Serial.println("horizontalrotationbrake:");
  //Serial.println(brakeHR);
  //Serial.println("lighttoggle:");
  //Serial.println(lightB);
}

/* THIS FUNCTION IS USED TO DISABLE OR ENABLE MOTION OF THE TABLE*/
void LOCK(){
    // DISABLING MOTION
  if ( (digitalRead(lock) == 1)&&(valL == 0)){
    valL = 1;
  }
    // ENABLING MOTION
  else if ( (digitalRead(lock) == 1)&&(valL == 1)){
    valL = 0;
  }
}

/* THIS FUNCTION IS USED TO OBTAIN DATA FROM THE LIMIT SWITCHES WHEN ANY OF THEM ARE PRESSED*/
void SWITCHES(){
  brakeTR = digitalRead(TBR); // top right brake
  brakeTL = digitalRead(TBL); // top left brake
  brakeBL = digitalRead(BBL); // bottom brake
  brakeVR = digitalRead(VBR); // vertical rotation brake
  brakeHR = digitalRead(HBR); // horizontal rotation brake
}

/* THIS FUCNTION IS USED TO INDICATE WHAT HAPPENS WHEN ONE OR MOR LIMIT SWITCHES ARE PRESSED*/
void STOPS(int brakeTR, int brakeTL, int brakeBL, int brakeVR, int brakeHR, int valX, int valY ){
    // top right brake
  if ( (brakeTR == 0) && (valY == 3) && (valX == 2)){
    digitalWrite(vertMR, HIGH);
    brake = 1;
  }
    // top left brake
  else if ( (brakeTL == 0) && (valY == 3) && (valX == 2) ){
    digitalWrite(vertML, HIGH);
    brake = 2;
  }
    // bottom brake
  else if ( (brakeBL == 0) && (valY == 0) && (valX == 2)){
    digitalWrite(vertMR, HIGH);
    digitalWrite(vertML, HIGH);
    brake = 3;
  }
    // vertical rotation brake
  else if ( (brakeVR == 0) && (valX == 3) && (valY == 2)){
    brake = 4;
  }
    // horizontal rotation brake
  else if ( (brakeHR == 0) && (valX == 0) && (valY == 2)){
    brake = 5;
  }
    // if none of the breaks are active
  else{
    digitalWrite(vertMR, LOW);
    digitalWrite(vertML, LOW);
    brake = 0;
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

/* THIS FUNCTION IS USED TO TOGGLE THE LAMP LIGHT ON AND OFF*/
void LIGHT(){
      // turning light on
    if ( (digitalRead(lightB) == 1) && (lightA == 0)){
      lightA = 1;
      digitalWrite(light, HIGH);
    }
      // turning light off
    else if ( (digitalRead(lock) == 1) && (lightA == 1)){
      lightA = 0;
      digitalWrite(light, LOW);
    }
}

/* THIS FUNCTION IS USED TO DISPLAY THE LIGHTS ON THE MICROCONTROLLER BASED ON THE MOVEMENT OF MOTORS AND BRAKES*/
void B_LIGHT (int valX, int valY, int valL, int brake){
   // while nothing is happening
  digitalWrite(ledr, HIGH);
  if (valL == 0){
     digitalWrite(ledR, LOW);
     digitalWrite(ledG, LOW);
     digitalWrite(ledB, LOW);
        // if none of the brakes are enabled
      if (brake == 0){
          //linear up
        if ( (valY == 3) && (valX == 2)){
          digitalWrite(ledr, LOW);
          digitalWrite(ledG, HIGH);
        }
          //linear down
        if ( (valY == 0) && (valX == 2)){
          digitalWrite(ledr, LOW);
          digitalWrite(ledB, HIGH);
        }
          //rotate vertical
        if ( (valY == 2) && (valX == 3)){
          digitalWrite(ledr, LOW);
          digitalWrite(ledR, HIGH);
          digitalWrite(ledG, HIGH);
        }
          //rotate horizontal
        if ( (valY == 2) && (valX == 0)){
          digitalWrite(ledr, LOW);
          digitalWrite(ledR, HIGH);
          digitalWrite(ledB, HIGH);
        }
      }
      //top right brake
    else if ((brake == 1) /*&& (valY == 3) && (valX == 2)*/){
      digitalWrite(ledr, HIGH);
      digitalWrite(ledG, HIGH);
    }
      //top left brake
    else if ((brake == 2) /*&& (valY == 3) && (valX == 2)*/){
      digitalWrite(ledr, HIGH);
      digitalWrite(ledB, HIGH);
      digitalWrite(ledG, LOW);
    }
      //bottom brake
    else if ((brake == 3) /*&& (valY == 0) && (valX == 2)*/){
      digitalWrite(ledr, HIGH);
      digitalWrite(ledR, HIGH);
      digitalWrite(ledG, HIGH);
      digitalWrite(ledB, HIGH);
    }
      //rotate vertical brake
    else if ((brake == 4) /*&& (valY == 2) && (valX == 3)*/){
      digitalWrite(ledr, HIGH);
      digitalWrite(ledR, HIGH);
      digitalWrite(ledG, HIGH);
    }
      //rotate horizontal brake
    else if ((brake == 5) /*&& (valY == 2) && (valX == 0)*/){
      digitalWrite(ledr, HIGH);
      digitalWrite(ledR, HIGH);
      digitalWrite(ledB, HIGH);
    }
  }
 
}
