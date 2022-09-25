//20725728
//<Includes>
  #include <SD.h>
  #include <SPI.h>
  #include <Time.h>
  #include <math.h>
  #include "MyStructures.h"
  #include "MyHeaders.h"
//</Includes>

//<Global Variables>
  int state = 1; //1-wait for GPS; 2-Navigate; 3-Standyby
  int Manual_Control = 0; // 1=manual Control enabled;
  int Debug = 0;
  bool runProgram = true;

  /* D13 - S2 (manual/auto)
   * D8 - S3 (green)
   * D4 - S1 (Power)
   * D12 - led on
   * D11 - led active
   * D9 - led M
   * D7 - led GPs
   * D6 - led A
  */
  int manualS = 13;
  int greenS = 8;
  int powerS = 4;
  int LED_ON = 12;
  int LED_ACTIVE = 11;
  int LED_MAN = 9;
  int LED_GPS = 7;
  int LED_AUTO = 6;

  //Navigation ----------------------------------
  int throttle_down_distance = 10; //(m) distance at which the vessel reduces below full throttle speed.
  int full_steer_angle = 20;//(degrees) the angle beyond which the vessel will use full steering.
  int throttle_percentage = 100; //percentage full throttle applied (full throttle doesnt seem to work)
  int steering_percentage = 100; //percentage steering lock applied.
  int arrival_threshold = 2; //distance at which it is considered that the vessel reached the point.
  
  //GPS ------------------------------------------
  GPSrx pos;
  bool GPS_connected = false;
  char validGPS = 'V';
  int targetIndex = 0;
  
  //PWM ------------------------------------------
  float RC = 84000;
  int POT_LH_input = A1;
  int POT_RH_input = A0;
  PWMControl rightSide;
  PWMControl leftSide;
  
  //SDCard -----------------------------------------
  // CS pin is pin 53,
  const int chipSelect = 53; //Set CS interface
  File root;
  String fileName = "datalog.txt";
  
  //SD Read --------------------------------------
  GPSpt points[20];

  //Debug ----------------------------------------
  int addPointsCnt = 0;
//</Global Variables>

void setup(){
  pwmPin3();
  pwmPin5();
  Serial.begin(9600);
  Serial1.begin(9600);
  initialize_SD();
  SetupIOs();
  readGPSPoints();
  //receiveAllGPSPoints();
}


void loop() {
  // put your main code here, to run repeatedly: 
  SwitchesAndLEDs();
  if(runProgram){
    if(Manual_Control == 1){
      updatePWM();
      writeThrottle();
    }else{
      switch(state){
        case 1:
            receiveGPSdata();
            if(GPS_connected){
              state = 2;
            }
          break;
        case 2:
            receiveGPSdata();
            navigate();
            delay(1000);
          break;
        case 3:
            delay(500);
          break;
      }
    }
  }
  delay(1000);
}

void Listen(){
    int byteRX = Serial.read();
    if(byteRX != -1){
      switch(byteRX){
        case 'A'://switch to automatic control
          Manual_Control = 0;
          break;
        case 'M'://switch to manual control
          Manual_Control = 1;
          break;
      }
    }
}

void SetupIOs(){
  
  pinMode(manualS, INPUT);
  pinMode(powerS, INPUT);
  pinMode(greenS, INPUT);
  pinMode(LED_ON, OUTPUT);
  pinMode(LED_ACTIVE, OUTPUT);
  pinMode(LED_MAN, OUTPUT);
  pinMode(LED_GPS, OUTPUT);
  pinMode(LED_AUTO, OUTPUT);
  /* D13 - S2 (manual/auto)
   * D8 - S3 (green)
   * D4 - S1 (Power)
   * D12 - led on
   * D11 - led active
   * D9 - led M
   * D7 - led GPs
   * D6 - led A
  */
}

void SwitchesAndLEDs(){
  if(digitalRead(powerS)){
    digitalWrite(LED_ON, HIGH);
    runProgram = true;
  }else{//stop the main loop running and set the motors to neutral
    digitalWrite(LED_ON, LOW);
    runProgram = false;
    leftSide.duty_Cycle = 75;
    rightSide.duty_Cycle = 75;
    power_thrusters();
  }
  if(digitalRead(manualS)){
    digitalWrite(LED_MAN, HIGH);
    digitalWrite(LED_AUTO, LOW);
    Manual_Control = 1;
  }else{
    digitalWrite(LED_MAN, LOW);
    digitalWrite(LED_AUTO, HIGH);
    Manual_Control = 0;
  }
  if(digitalRead(LED_ACTIVE)){
    digitalWrite(LED_ACTIVE, LOW);
  }else{
    digitalWrite(LED_ACTIVE, HIGH);
  }
  if(GPS_connected){
    digitalWrite(LED_GPS,HIGH);
  }else{
    digitalWrite(LED_GPS,LOW);
  }
}
