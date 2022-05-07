//20725728

int PWM_Pin1 = 2;
float RC = 700000;
float dutyCyclePercent = 0.09; // 9%
int DCInput = A0;
//int DCInput = A2;
float duty = 0;

void setup(){
  pinMode(22,OUTPUT);
  PMC->PMC_PCER1 |= PMC_PCER1_PID34;                     // TC7 power ON - Timer Counter 2 channel 1 IS TC7 - See page 38

  PIOC->PIO_PDR |= PIO_PDR_P28;                          // The pin is no more driven by GPIO
  PIOC->PIO_ABSR |= PIO_PC28B_TIOA7;                     // Periperal type B  - See page 859

  TC2->TC_CHANNEL[1].TC_CMR = TC_CMR_TCCLKS_TIMER_CLOCK1  // MCK/2, clk on rising edge
                              | TC_CMR_WAVE               // Waveform mode
                              | TC_CMR_WAVSEL_UP_RC        // UP mode with automatic trigger on RC Compare
                              | TC_CMR_ACPA_CLEAR          // Clear TIOA7 on RA compare match  -- See page 883
                              | TC_CMR_ACPC_SET;           // Set TIOA7 on RC compare match
                                                            /// Mck = 84 MHz
  TC2->TC_CHANNEL[1].TC_RC = RC;  //<*********************  Frequency = (Mck/2)/TC_RC  Hz = 60 KHz
  TC2->TC_CHANNEL[1].TC_RA = dutyCyclePercent*RC;  //<********************   Duty cycle = (TC_RA/TC_RC) * 100  % = 7.5% (full stop)

  TC2->TC_CHANNEL[1].TC_IER = TC_IER_CPCS;                 // Interrupt on RC compare match
  NVIC_EnableIRQ(TC7_IRQn);

  TC2->TC_CHANNEL[1].TC_CCR = TC_CCR_SWTRG | TC_CCR_CLKEN; // Software trigger TC7 counter and enable

  Serial.begin(9600);
}

void TC7_Handler() {
  TC2->TC_CHANNEL[1].TC_SR;
}

//frequency is 60Hz
//resolution 8 bits
//1 cycle = 16.667ms
//reverse 6%
//Stop 9%
//forward 12%

void loop() {
  // put your main code here, to run repeatedly: 
  duty = analogRead(DCInput);
  dutyCyclePercent = ((((duty/1022) * 0.06) + 0.06) * RC);
  //duty = analogRead(DCInput);
  
  TC2->TC_CHANNEL[1].TC_RA = dutyCyclePercent; 
  Serial.println(dutyCyclePercent);  
}
