//20725728

int Manual_Control = 0;//manual Control enabled;

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
  }else{
    receiveGPSdata();
  }
}

void Listen(){
  while(Serial.available() > 0){
    int byteRX = Serial.read();
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
