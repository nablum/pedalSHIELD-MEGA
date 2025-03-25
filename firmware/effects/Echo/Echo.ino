// Licensed under a Creative Commons Attribution 4.0 International License.
/* Echo.ino delays the signal and mixes it with the clean signal. */

//display parameters
#include "U8glib.h"
U8GLIB_SH1106_128X64 u8g(U8G_I2C_OPT_NO_ACK);
 
//defining harware resources
#define LED 13
#define FOOTSWITCH 12
#define TOGGLE 2
#define PUSHBUTTON_1 A5
#define PUSHBUTTON_2 A4

//defining the output PWM parameters
#define PWM_FREQ 0x00FF // pwm frequency - 31.3KHz
#define PWM_MODE 0 // Fast (1) or Phase Correct (0)
#define PWM_QTY 2 // 2 PWMs in parallel

//system variables
int output, counter=0;
unsigned int ADC_low, ADC_high;

//effect variables
#define DELAY_MAX 7500
#define DELAY_MIN 0
#define DELAY_STEP 10
#define GAIN_MAX 1000
#define GAIN_MIN 0
#define GAIN_STEP 1
byte DelayBuffer[DELAY_MAX];
unsigned int DelayCounter = 0;
unsigned int DelayDepth = DELAY_MAX;
unsigned int DelayGain = GAIN_MAX/2;
 
void setup() 
{
  //setup IO
  pinMode(FOOTSWITCH, INPUT_PULLUP);
  pinMode(TOGGLE, INPUT_PULLUP);
  pinMode(PUSHBUTTON_1, INPUT_PULLUP);
  pinMode(PUSHBUTTON_2, INPUT_PULLUP);
  pinMode(LED, OUTPUT);
  pinMode(6, OUTPUT); //PWM0 as output
  pinMode(7, OUTPUT); //PWM1 as output
 
  //setup ADC
  ADMUX = 0x60; // left adjust, adc0, internal vcc
  ADCSRA = 0xe5; // turn on adc, ck/32, auto trigger
  ADCSRB = 0x00; // free running mode
  DIDR0 = 0x01; // turn off digital inputs for adc0
 
  //setup PWM
  TCCR4A = (((PWM_QTY - 1) << 5) | 0x80 | (PWM_MODE << 1)); //
  TCCR4B = ((PWM_MODE << 3) | 0x11); // ck/1
  TIMSK4 = 0x20; // interrupt on capture interrupt
  ICR4H = (PWM_FREQ >> 8);
  ICR4L = (PWM_FREQ & 0xff);
  DDRB |= ((PWM_QTY << 1) | 0x02); // turn on outputs
  sei(); // turn on interrupts - not really necessary with arduino 
}
 
void loop() 
{
  if (digitalRead(FOOTSWITCH))
  {
    digitalWrite(LED, HIGH);
    if(digitalRead(TOGGLE))
    {
      u8g.firstPage(); 
      do
      {
        u8g.setFont(u8g_font_helvR24r);
        u8g.drawStr( 0, 32, "DELAY");  
        u8g.setFont(u8g_font_helvR14r); 
        u8g.drawStr( 0, 60, "LENGTH");
        u8g.setFont(u8g_font_helvR14r); 
        u8g.setPrintPos(85, 60); 
        u8g.print(DelayDepth);
      }
      while( u8g.nextPage() );
    }
    else
    {
      u8g.firstPage(); 
      do
      {
        u8g.setFont(u8g_font_helvR24r);
        u8g.drawStr( 0, 32, "DELAY");  
        u8g.setFont(u8g_font_helvR14r); 
        u8g.drawStr( 0, 60, "GAIN");
        u8g.setFont(u8g_font_helvR14r); 
        u8g.setPrintPos(85, 60); 
        u8g.print(DelayGain);
      }
      while( u8g.nextPage() );
    }
  }
  else{  
    digitalWrite(LED, LOW);
    u8g.firstPage(); 
    do {
      u8g.setFont(u8g_font_helvR24r);
      u8g.drawStr( 0, 32, "EFFECT ");    
      u8g.drawStr( 0, 64, "   OFF  ");    
    } while( u8g.nextPage() );
  }
}
 
ISR(TIMER4_CAPT_vect) 
{
  //get ADC data
  ADC_low = ADCL;
  ADC_high = ADCH;
 
  //store the high bit only for the delay buffer
  DelayBuffer[DelayCounter] = (ADC_high + DelayBuffer[DelayCounter])>>1 ; 
 
  //button status
  counter++;
  if(counter==100)
  { 
    counter=0;
    if(digitalRead(TOGGLE))
    {
      if(!digitalRead(PUSHBUTTON_2)) if(DelayDepth<DELAY_MAX) DelayDepth=DelayDepth+DELAY_STEP;
      if(!digitalRead(PUSHBUTTON_1)) if(DelayDepth>DELAY_MIN) DelayDepth=DelayDepth-DELAY_STEP;
    }
    else
    {
      if(!digitalRead(PUSHBUTTON_2)) if(DelayGain<GAIN_MAX) DelayGain=DelayGain+GAIN_STEP;
      if(!digitalRead(PUSHBUTTON_1)) if(DelayGain>GAIN_MIN) DelayGain=DelayGain-GAIN_STEP;
    }
  }
 
  //increse/reset delay counter.   
  DelayCounter++;
  if(DelayCounter >= DelayDepth) DelayCounter = 0; 
 
  //output = (delayed signal * gain + current signal)
  output = (DelayBuffer[DelayCounter]*DelayGain/GAIN_MAX +(ADC_high))>>1;
 
  //write the PWM signal
  OCR4AL = output;
  OCR4BL = output;
}