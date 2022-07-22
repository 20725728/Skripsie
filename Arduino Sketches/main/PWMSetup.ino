float RC = 84000;
int POT_LH_input = A0;
int POT_RH_input = A1;

struct PWMControl {
  int ADCRange = 1022;
  float PWMRange = 0.06;
  float reversePoint = 0.06;
  float analog;
  float timer;
};

PWMControl rightSide;
PWMControl leftSide;

void updatePWM() {
  leftSide.analog = analogRead(POT_LH_input);
  rightSide.analog = analogRead(POT_RH_input);
//  leftSide.timer = ((((leftSide.analogDuty / leftSide.ADCRange) * leftSide.PWMRange) + leftSide.reversePoint) * RC);
//  rightSide.timer = ((((leftSide.analogDuty / rightSide.ADCRange) * rightSide.PWMRange) + rightSide.reversePoint) * RC);
  //TC2->TC_CHANNEL[1].TC_RA = leftSide.timer;
  //TC2->TC_CHANNEL[2].TC_RA = rightSide.timer;
readAnalogPins();
}
/**
   left                 * right
   max ~677             * max ~700
   min ~321             * min ~340
   mid~510              * mid ~520
   490 - 530 stop       * 500 - 540 stop

*/
void readAnalogPins() {
  rightSide.analog = analogRead(POT_RH_input);
  Serial.write("Right Input: ");
  Serial.print(rightSide.analog);
  if(rightSide.analog < 500){
    rightSide.timer = 63000 - (((520 - rightSide.analog)/(520 - 340)) * (63000-42000));
    Serial.write("\tRight Timer: ");
    Serial.println(rightSide.timer);
  }else if(rightSide.analog > 540){
    rightSide.timer = 84000 - (((700 - rightSide.analog)/(700 - 520)) * (84000-63000));
    Serial.write("\tRight Timer: ");
    Serial.println(rightSide.timer);
  }else{
    Serial.write("Right Timer: 63000 \tNeutral\n");
    rightSide.timer = 63000;
  }
  TC2->TC_CHANNEL[1].TC_RA = rightSide.timer;
}

void pwmPin3() {
  //frequency is 60Hz
  //resolution 8 bits
  //1 cycle = 16.667ms
  //reverse 6%
  //Stop 9%
  //forward 12%
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
  TC2->TC_CHANNEL[1].TC_RA = 63000; //<********************   Duty cycle = (TC_RA/TC_RC) * 100

  TC2->TC_CHANNEL[1].TC_IER = TC_IER_CPCS;                 // Interrupt on RC compare match
  NVIC_EnableIRQ(TC7_IRQn);

  TC2->TC_CHANNEL[1].TC_CCR = TC_CCR_SWTRG | TC_CCR_CLKEN; // Software trigger TC7 counter and enable
}

void pwmPin5() {
  /*Set power management controller register
   * PMC - power controller register                                
   * PMC_PCER1 - Peripheral clock enable register 1                 Pg 563
   * PMC_PCER1_PID33 - Enable peripheral clock PID33 - Timer Channel 6
  */
  PMC->PMC_PCER1 |= PMC_PCER1_PID33;                     // TC6 power ON - Timer Counter 2 channel 2 IS TC6 - See page 38

  /*Disable pin input/output
   * PIOC - parallel input output controller C
   * PIO_PDR - PIO Disable Register                                 Pg 634
   * PIO_PDR_P25 - Disable P25 (PC25 = TIOA6)
   * PIO_ABSR - Peripheral AB select register                       Pg 656
   * PIO_PC25B_TIOA6 - set PC25 = peripheral B function (TIOA6)
   */
  PIOC->PIO_PDR |= PIO_PDR_P25;                          // The pin is no more driven by GPIO
  PIOC->PIO_ABSR |= PIO_PC25B_TIOA6;                     // Periperal type B  - See page 859

  /* Set Timer counter settings                 TC UI               Pg 879
   * TC_CMR - Channel mode register                                 Pg 883
   * TC_CMR_TCCLKS_TIMER_CLOCK1 - Set timer clock 1 (Mck/2) & counter incremented on rising edge
   * TC_CMR_WAVE - Enable waveform mode
   * TC_CMR_WAVSEL_UP_RC - UP mode with automatic trigger on RC compare
   * TC_CMR_ACPA_CLEAR - Clear effect on TIOA on RA compare
   * TC_CMR_ACPC_SET - Set effect on TIOA on RC compare
   * 
   * TC_RC - Register C                                             Pg 891
   * TC_RA - Register A                                             Pg 889
   * 
   * TC_IER - Interrupt enable register                             Pg 894
   * TC_IER_CPCS - enable interrupt on RC Compare match
   * 
   * TC_CCR - Channel Control Register                              Pg 880
   * TC_CCR_SWTRG - A software trigger is performed: the counter is reset and the clock is started.
   * TC_CCR_CLKEN - enable clock
   */
  TC2->TC_CHANNEL[2].TC_CMR = TC_CMR_TCCLKS_TIMER_CLOCK1  // MCK/2, clk on rising edge
                              | TC_CMR_WAVE               // Waveform mode
                              | TC_CMR_WAVSEL_UP_RC        // UP mode with automatic trigger on RC Compare
                              | TC_CMR_ACPA_CLEAR          // Clear TIOA6 on RA compare match  -- See page 883
                              | TC_CMR_ACPC_SET;           // Set TIOA6 on RC compare match
  /// Mck = 84 MHz
  TC2->TC_CHANNEL[2].TC_RC = RC;  //<*********************  Frequency = (Mck/2)/TC_RC  Hz = 60 KHz
  TC2->TC_CHANNEL[2].TC_RA = 63000; //<********************   Duty cycle = (TC_RA/TC_RC) * 100

  TC2->TC_CHANNEL[2].TC_IER = TC_IER_CPCS;                 // Interrupt on RC compare match
  NVIC_EnableIRQ(TC6_IRQn);

  TC2->TC_CHANNEL[2].TC_CCR = TC_CCR_SWTRG | TC_CCR_CLKEN; // Software trigger TC6 counter and enable
}

void  TC6_Handler() {
  /*Interrupt triggered on RC compare match
  * TC_SR - Status Register                                         Pg - 892        
  * Read the status registe to clear the RC compare and RA compare match flags.
   */
  TC2->TC_CHANNEL[2].TC_SR;
}


void TC7_Handler() {
  TC2->TC_CHANNEL[1].TC_SR;
}
