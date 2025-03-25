// Licensed under a Creative Commons Attribution 4.0 International License.
/* OctaveCrusher.ino reads the ADC and write the PWMs every nth sample generating an harsh octave-down. */
 
//display parameters
#include "U8glib.h"
U8GLIB_SH1106_128X64 u8g(U8G_I2C_OPT_NO_ACK);
 
//defining hardware resources
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
#define MAX_THRESHOLD 50
#define MIN_THRESHOLD 0
int octaver_threshold = 15;
int octaver_counter = 0;
 
void setup() {
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
  ADCSRB = 0x00; //free running mode
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
      u8g.setFont(u8g_font_helvR10r);
      u8g.drawStr( 0, 25, "OCTAVE CRUSHER");    
      u8g.setPrintPos(45, 60);
      u8g.setFont(u8g_font_helvR24r);
      u8g.print(octaver_threshold);
    } while( u8g.nextPage() );
  }
  else  
  {  
    digitalWrite(LED, LOW);
    u8g.firstPage(); 
    do {
      u8g.setFont(u8g_font_helvR24r);
      u8g.drawStr( 0, 30, "EFFECT");    
      u8g.drawStr( 0, 60, "   OFF ");   
    } while( u8g.nextPage() );
  }
}
 
ISR(TIMER4_CAPT_vect) 
{
  //button status
  counter++;
  if (counter==10000)
  { 
    counter=0;
    if (!digitalRead(PUSHBUTTON_2)) if (octaver_threshold<MAX_THRESHOLD) octaver_threshold++;
    if (!digitalRead(PUSHBUTTON_1)) if (octaver_threshold>MIN_THRESHOLD) octaver_threshold--;
  }
 
  //octave crusher
  octaver_counter++;
  if(octaver_counter>octaver_threshold)
  { 
    //reset counter
    octaver_counter=0;

    //get ADC data
    ADC_low = ADCL;
    ADC_high = ADCH;
    
    //construct the input sample summing the ADC low and high byte.
    output = ((ADC_high << 8) | ADC_low) + 0x8000;
  
    //write PWMs
    OCR4AL = ((output + 0x8000) >> 8);
    OCR4BL = output;
  }

}