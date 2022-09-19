//20725728
//<Includes>
  #include <SD.h>
  #include <Time.h>
  #include <math.h>
  #include "MyStructures.h"
  #include "MyHeaders.h"
//</Includes>

//<Global Variables>
  int state = 1; //1-wait for GPS; 2-Navigate; 3-Standyby
  int Manual_Control = 0; // 1=manual Control enabled;
  int Debug = 0;

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
  receiveAllGPSPoints();
}


void loop() {
  // put your main code here, to run repeatedly: 
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
          delay(2000);
        break;
      case 3:
          delay(1000);
        break;
    }
  }
//  int rx = Serial1.read();
//  if(rx!=-1){
//    Serial.write(rx);
//  }
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
