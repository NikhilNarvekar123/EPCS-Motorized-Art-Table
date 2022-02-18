


// joystick -- butttons 
// connect functions to motors


// Constants
const int vertPulse = P2_4;
const int vertDir = P2_5;
const int vertML = P2_3;
const int vertMR = P2_7;

// Variable
int vertMotion = P4_1 ; // Y-axis from joystick
int rotMotion = P4_0;   // x-axis from joystick
int lock = P4_2;        // joystick button locks motion    
int lightB = P1_6;      // light button        
int valX = 0;           // easy to use data from joystick
int valY = 0;           // easy to use data from joystick
int valL = 0;           // indicates whether motion is locked or not

int VBR = P3_3;         // vertical rotation brake
int HBR = P3_2;         // horizontal rotation brake

int brake = 0;          // value used to indicate board light values
int brakeTR = 0;        // to indicate if brake is active or not
int brakeTL = 0;        // to indicate if brake is active or not
int brakeBL = 0;        // to indicate if brake is active or not
int brakeVR = 0;        // to indicate if brake is active or not
int brakeHR = 0;        // to indicate if brake is active or not
int lightA = 0;         // to indicate if light is on or not


void setup() {
  
  Serial.begin(9600);
  pinMode(light, OUTPUT);         //  lamp
  pinMode(lightB, INPUT_PULLUP);  // lamp button
  attachInterrupt(digitalPinToInterrupt(lightB),LIGHT,RISING); 

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

  pinMode(lock, INPUT_PULLUP);  // setup lock
  attachInterrupt(digitalPinToInterrupt(lock),LOCK, RISING); // lock is interrupt
  
  
}





void loop() {

  if(valL == 1) {
    
  }

  
}



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

