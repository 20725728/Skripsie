//20725728



void setup(){
  pwmPin3();
  //pwmPin5();
  Serial.begin(9600);
  Serial1.begin(9600);
}


void loop() {
  // put your main code here, to run repeatedly: 
  updatePWM();
  delay(10);
  //receiveGPSdata();
  //printStruct();
}
