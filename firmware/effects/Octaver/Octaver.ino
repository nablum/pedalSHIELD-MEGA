// Licensed under a Creative Commons Attribution 4.0 International License.
/* Octaver.ino creates an octave-up or octave-down signal from the original one. */

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
int output;
int counter = 0;

//effect variables
#define DELAY_MAX 1000
byte DelayBufferL[DELAY_MAX];
byte DelayBufferH[DELAY_MAX];
unsigned int DelayCounter = 0;
unsigned int divider = 0;
unsigned int mode = 0;
unsigned int DelayCounterL = 0;
unsigned int DelayCounterH = DELAY_MAX/2;
 
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
    u8g.firstPage(); 
    do{
      u8g.setFont(u8g_font_helvR14r);
      u8g.drawStr( 17, 30, "OCTAVER");    
      if(mode) u8g.drawStr( 42, 60, "LOW");
      else u8g.drawStr( 40, 60, "HIGH");
    } while( u8g.nextPage() );
  }
  else
  {  
    digitalWrite(LED, LOW);
    u8g.firstPage(); 
    do{
      u8g.setFont(u8g_font_helvR24r);
      u8g.drawStr( 0, 32, "EFFECT ");    
      u8g.drawStr( 0, 64, "   OFF  ");    
    }while( u8g.nextPage() );
  }
}
 
ISR(TIMER4_CAPT_vect) 
{
  //button status
  counter++;
  if(counter==100)
  { 
    counter=0;
    if (!digitalRead(PUSHBUTTON_1)) if (!mode) mode++;
    if (!digitalRead(PUSHBUTTON_2)) if (mode) mode--; 
  }
  
  //store ADC data
  DelayBufferL[DelayCounter] = ADCL;
  DelayBufferH[DelayCounter] = ADCH;

  //octaver
  switch(mode)
  {
    case 0: //octave-up 
    DelayCounterL += 2;
    DelayCounterH += 2;
    break;
    case 1: //octave-down
    divider++;
    if(divider>=2) {DelayCounterL++;DelayCounterH++;divider=0;}
    break;
  }

  //increse/reset delay counter
  DelayCounter++;
  if(DelayCounter >= DELAY_MAX) DelayCounter = 0;
  if(DelayCounterL >= DELAY_MAX) DelayCounterL = 0; 
  if(DelayCounterH >= DELAY_MAX) DelayCounterH = 0; 
 
  //construct the output sample
  output = ((DelayBufferH[DelayCounterH] << 8) | DelayBufferL[DelayCounterL]) + 0x8000;

  //write the PWM signal
  OCR4AL = ((output + 0x8000) >> 8);
  OCR4BL = output;
}