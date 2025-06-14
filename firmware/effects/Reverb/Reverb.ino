/*
  Reverb.ino uses 3 Delay Buffers to mimic the reflexion from 3 walls.

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

//other variables
int output;
int counter = 0;
unsigned int ADC_low, ADC_high;

//effect variables
#define MAX_DELAY1  4000
#define MAX_DELAY2  2000
#define MAX_DELAY3  1000
#define MIN_DELAY1  3000
#define MIN_DELAY2  1000
#define MIN_DELAY3     0
byte DelayBuffer1[MAX_DELAY1];
byte DelayBuffer2[MAX_DELAY2];
byte DelayBuffer3[MAX_DELAY3];
unsigned int DelayCounter1 = 0;
unsigned int DelayCounter2 = 0;
unsigned int DelayCounter3 = 0;
unsigned int Delay_Depth1 = MAX_DELAY1;
unsigned int Delay_Depth2 = MAX_DELAY2;
unsigned int Delay_Depth3 = MAX_DELAY3;
 
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
      u8g.drawStr( 0, 14, "REVERB"); 
      u8g.drawStr( 0, 32, "ECHO 1: ");   
      u8g.setPrintPos(80, 32);
      u8g.print(Delay_Depth1);
      u8g.drawStr( 0, 48, "ECHO 2: "); 
      u8g.setPrintPos(80, 48);
      u8g.print(Delay_Depth2);
      u8g.drawStr( 0, 64, "ECHO 3: "); 
      u8g.setPrintPos(80, 64);
      u8g.print(Delay_Depth3);
    } while(u8g.nextPage());
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
 
  //store the high bit only 
  DelayBuffer1[DelayCounter1] = (ADC_high+DelayBuffer1[DelayCounter1])>>1;
  DelayBuffer2[DelayCounter2] = (ADC_high+DelayBuffer1[DelayCounter2])>>1;
  DelayBuffer3[DelayCounter3] = (ADC_high+DelayBuffer1[DelayCounter3])>>1;

  //button status
  counter++;
  if(counter==100)
  { 
  counter=0;
    if (!digitalRead(PUSHBUTTON_2)) {
      if (Delay_Depth1<MAX_DELAY1)Delay_Depth1=Delay_Depth1+1; 
      if (Delay_Depth2<MAX_DELAY2)Delay_Depth2=Delay_Depth2+1; 
      if (Delay_Depth3<MAX_DELAY3)Delay_Depth3=Delay_Depth3+1; 
    }
   
    if (!digitalRead(PUSHBUTTON_1)) {
      if (Delay_Depth1>MIN_DELAY1)Delay_Depth1=Delay_Depth1-1; 
      if (Delay_Depth2>MIN_DELAY2)Delay_Depth2=Delay_Depth2-1; 
      if (Delay_Depth3>MIN_DELAY3)Delay_Depth3=Delay_Depth3-1; 
    }
  }
 
  //increse/reset delay counter.   
  DelayCounter1++;
  DelayCounter2++;
  DelayCounter3++;
  if(DelayCounter1 >= Delay_Depth1) DelayCounter1 = 0; 
  if(DelayCounter2 >= Delay_Depth2) DelayCounter2 = 0; 
  if(DelayCounter3 >= Delay_Depth3) DelayCounter3 = 0; 
 
  //output = delayed signal + current signal.
  output = (DelayBuffer1[DelayCounter1]+DelayBuffer2[DelayCounter2]+DelayBuffer3[DelayCounter3]+ADC_high)>>2;
 
  //write the PWM signal
  OCR4AL = output;
  OCR4BL = output;
}