//20725728
//<Includes>
  #include <SD.h>
  #include <Time.h>
  #include <math.h>
  #include "MyStructures.h"
  #include "MyHeaders.h"
//</Includes>

//<Global Variables>
  int Manual_Control = 1; // 1=manual Control enabled;
  char validGPS = 'V';
  //GPS
  GPSrx pos;
  bool GPS_connected = false;
  //PWM
  float RC = 84000;
  int POT_LH_input = A1;
  int POT_RH_input = A0;
  PWMControl rightSide;
  PWMControl leftSide;
  //SDCard
  // CS pin is pin 53,
  const int chipSelect = 53; //Set CS interface
  File root;
  String fileName = "datalog.txt";
  //SD Read
  GPSpt points[20];
//</Global Variables>

void setup(){
  pwmPin3();
  pwmPin5();
  Serial.begin(9600);
  Serial1.begin(9600);
  initialize_SD();
}


void loop() {
  // put your main code here, to run repeatedly: 
  if(Manual_Control == 1){
    updatePWM();
    writeThrottle();
  }else{
    receiveGPSdata();
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
