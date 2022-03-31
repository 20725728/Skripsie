//20725728

int PWM_Pin1 = 2;

void setup(){
    Serial.begin(9600);
   
  PMC->PMC_PCER1 |= PMC_PCER1_PID34;                     // TC7 power ON - Timer Counter 2 channel 1 IS TC7 - See page 38

  PIOC->PIO_PDR |= PIO_PDR_P28;                          // The pin is no more driven by GPIO
  PIOC->PIO_ABSR |= PIO_PC28B_TIOA7;                     // Periperal type B  - See page 859

  TC2->TC_CHANNEL[1].TC_CMR = TC_CMR_TCCLKS_TIMER_CLOCK1  // MCK/2, clk on rising edge
                              | TC_CMR_WAVE               // Waveform mode
                              | TC_CMR_WAVSEL_UP_RC        // UP mode with automatic trigger on RC Compare
                              | TC_CMR_ACPA_CLEAR          // Clear TIOA7 on RA compare match  -- See page 883
                              | TC_CMR_ACPC_SET;           // Set TIOA7 on RC compare match

  TC2->TC_CHANNEL[1].TC_RC = 840000;  //<*********************  Frequency = (Mck/2)/TC_RC  Hz = 20 KHz
  TC2->TC_CHANNEL[1].TC_RA = 84000;  //<********************   Duty cycle = (TC_RA/TC_RC) * 100  %

  TC2->TC_CHANNEL[1].TC_IER = TC_IER_CPCS;                 // Interrupt on RC compare match
  NVIC_EnableIRQ(TC7_IRQn);

  TC2->TC_CHANNEL[1].TC_CCR = TC_CCR_SWTRG | TC_CCR_CLKEN; // Software trigger TC7 counter and enable
  
  

  analogWrite(PWM_Pin1,250);
}

void TC7_Handler() {
  TC2->TC_CHANNEL[1].TC_SR;
}

//frequency is 50Hz
//resolution 8 bits
//1 cycle = 20ms

void loop() {
  // put your main code here, to run repeatedly:
  int readValue = Serial.read();
  if(readValue != -1){
    Serial.println(readValue);
  }

  /* 0-0
   * 1 - 50
   * 2 - 100
   * 3 - 150
   * 4 - 200
   * 5 - 255
   */
  if(readValue == 48){//0
    analogWrite(PWM_Pin1,0);
    Serial.println(0);
  }else if(readValue  == 49){//1
    analogWrite(PWM_Pin1,50);
    Serial.println(50);
  }else if(readValue  == 50){//2
    analogWrite(PWM_Pin1,100);
    Serial.println(100);
  }else if(readValue  == 51){//3
    analogWrite(PWM_Pin1,150);
    Serial.println(150);
  }else if(readValue  == 52){//4
    analogWrite(PWM_Pin1,200);
    Serial.println(200);
  }else if(readValue  == 53){//5
    analogWrite(PWM_Pin1,510);
    Serial.println(255); 
  }else if(readValue == 100){//d
    Serial.println("Drive");
    TC2->TC_CHANNEL[1].TC_RA = 84000; 
  }
  else if(readValue == 110){//n
    Serial.println("Neutral");
    TC2->TC_CHANNEL[1].TC_RA = 63000; 
  }
  else if(readValue == 114){//r
    Serial.println("Reverse");
    TC2->TC_CHANNEL[1].TC_RA = 42000; 
  }

  /*for(int i = 0;i<255;i++){
    analogWrite(PWM_Pin1,i);
    delay(5);
  }
  for(int i=255;i>0;i--){
    analogWrite(PWM_Pin1,i);
    delay(5);
  }*/
}
