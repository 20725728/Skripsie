struct PWMControl {
  int analog = 505;             //the middle for both of the POTs
  float duty_Cycle = 75;      //75% is neutral.
};

//PWM ------------------------------------------
  float RC = 84000;
  int POT_LH_input = A1;
  int POT_RH_input = A0;
  PWMControl rightSide;
  PWMControl leftSide;


void setup() {
  // put your setup code here, to run once:
  Serial1.begin(9600);
  Serial.begin(9600);
  pwmPin5();
  pwmPin3();
  
}

void loop() {
  int rx = Serial.read();
  if(rx!=-1){
    
    Serial.println(rx);
    switch(rx){
      case 70:
        leftSide.duty_Cycle = 100;
        rightSide.duty_Cycle = 100;
        break;
      case 78:
        leftSide.duty_Cycle = 75;
        rightSide.duty_Cycle=75;
        break;
      case 82:
        leftSide.duty_Cycle=50;
        rightSide.duty_Cycle=50;
        break;
      case 79:
        leftSide.duty_Cycle=62;
        rightSide.duty_Cycle=88;
        break;
    }
  }
  power_thrusters();
//  receiveGPSdata();
//  printStruct();
//  delay(5000);
}
