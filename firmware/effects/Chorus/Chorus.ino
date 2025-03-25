// Licensed under a Creative Commons Attribution 4.0 International License.
/* Chorus.ino delayed the signal with a triangular LFO and mixed it with the clean signal. */

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
int output, output, counter, count_up=0;
unsigned int ADC_low, ADC_high;

//effect variables
#define MAX_LENGTH 300
#define MIN_LENGTH 20
unsigned int DelayMax=MAX_LENGTH/2;
unsigned int DelayMin=10;
unsigned int DelayBuffer[MAX_LENGTH];
unsigned int DelayCounter = 0;
unsigned int DelayDepth = 100;
 
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
    do {
      u8g.setFont(u8g_font_helvR14r);
      u8g.drawStr( 0, 20, "  CHORUS  ");    
      u8g.setFont(u8g_font_helvR24r); 
      u8g.setPrintPos(10, 60); 
      u8g.print(DelayMax);
      u8g.setFont(u8g_font_helvR14r); 
      u8g.setPrintPos(100, 60); 
      u8g.print(count_up);
    } while( u8g.nextPage() );
  }
  else
  {  
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
 
  //store the current reading
  DelayBuffer[DelayCounter] = (((ADC_high << 8) | ADC_low) + 0x8000);
 
  //button status
  counter++;
  if(counter==1000)
  { 
      counter=0;
      if (!digitalRead(PUSHBUTTON_2)) if (DelayMax<MAX_LENGTH)DelayMax++; //increase the maximum delay
      if (!digitalRead(PUSHBUTTON_1)) if (DelayMax>MIN_LENGTH)DelayMax--; //decrease the maximum delay
  }
 
  //chorus effect
  DelayCounter++;
  if(DelayCounter >= DelayDepth) 
  {
    DelayCounter = 0; 
    if(count_up)
    {
       for(int p=0;p<10;p++) DelayBuffer[DelayDepth+p]=DelayBuffer[DelayDepth-1]; 
       DelayDepth++;
       if(DelayDepth>=DelayMax) count_up=0;
    }
    else
    {
      DelayDepth=DelayDepth-1;
      if(DelayDepth<=DelayMin) count_up=1;
    }
  }

  //output signal
  output = (DelayBuffer[DelayCounter] + (((ADC_high << 8) | ADC_low) + 0x8000))>>1 ;

  //write the PWM signal
  OCR4AL = ((output + 0x8000) >> 8);
  OCR4BL = output;
}