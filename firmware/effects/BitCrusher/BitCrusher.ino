/*
  BitCrusher.ino reads the ADC and shift the bits creating a high distorted sound.

  Part of the pedalSHIELD-MEGA project: https://github.com/nablum/pedalSHIELD-MEGA

  Copyright (c) 2025 Nablum
  SPDX-License-Identifier: MIT

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/
 
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
#define MAX_BIT 8
#define MIN_BIT 0
int bit_crush_variable=MAX_BIT/2;
 
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
      u8g.setFont(u8g_font_helvR12r);
      u8g.drawStr( 0, 25, "  BIT CRUSHER");    
      u8g.setPrintPos(50, 60);
      u8g.setFont(u8g_font_helvR24r);
      u8g.print(bit_crush_variable);
      u8g.setFont(u8g_font_helvR12r);
      u8g.print(" Bits");
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
  //get ADC data
  ADC_low = ADCL;
  ADC_high = ADCH;
  
  //construct the input sumple summing the ADC low and high byte.
  output = ((ADC_high << 8) | ADC_low) + 0x8000; // make a signed 16b value

  //button status
  counter++;
  if (counter==10000)
  { 
    counter=0;
    if (!digitalRead(PUSHBUTTON_2)) if (bit_crush_variable<MAX_BIT) bit_crush_variable=bit_crush_variable+1;
    if (!digitalRead(PUSHBUTTON_1)) if (bit_crush_variable>MIN_BIT) bit_crush_variable=bit_crush_variable-1;
  }
 
  //bit_crush_variable goes from 0 to 8 and the input signal is crushed in the next instruction:
  output = output<<bit_crush_variable;
 
  //write the PWM signal
  OCR4AL = ((output + 0x8000) >> 8); // convert to unsigned, send out high byte
  OCR4BL = output; // send out low byte
}