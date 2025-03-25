// Licensed under a Creative Commons Attribution 4.0 International License.
/* MultiEffects.ino allows selection between delay, echo, distortion, fuzz and bit-crusher effects. */

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
int output, outputAL, outputBL, counter = 0;
unsigned int ADC_low, ADC_high, effect_type = 1;

//effect variables
#define MAX_DELAY 7500
#define MIN_DELAY 20
#define MAX_BIT 8
#define MIN_BIT 1
#define MAX_OUTPUT 32768
#define MIN_OUTPUT 20
byte DelayBuffer[MAX_DELAY];
unsigned int DelayCounter = 0;
unsigned int DelayDepth = 1000;
unsigned int EchoDepth = 4000;
int distortion_threshold = 6000;
int fuzz_threshold = 6000;
int bit_crush_variable = 4;
 
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
  ADCSRB = 0x00; // ADC free running mode
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
  /* selection mode */
  // program stays in this loop while the toggle switch is down, you can change effect by pushing buttons.
  while(digitalRead(TOGGLE) && digitalRead(FOOTSWITCH))
  { 
    digitalWrite(LED, HIGH);
    if(!digitalRead(PUSHBUTTON_2)) if(effect_type<5) effect_type++;
    if(!digitalRead(PUSHBUTTON_1)) if(effect_type>1) effect_type--;
    switch(effect_type)
    {
      case 1: //1st effect
      u8g.firstPage();
      do {
          u8g.setFont(u8g_font_helvR14r);
          u8g.drawStr( 10, 16, "SELECTION");    
          u8g.drawStr( 32, 50, "DELAY");
          } while( u8g.nextPage() );
      break;
      case 2: //2nd effect
      u8g.firstPage();
      do {
          u8g.setFont(u8g_font_helvR14r);
          u8g.drawStr( 10, 16, "SELECTION");    
          u8g.drawStr( 35, 50, "ECHO");
          } while( u8g.nextPage() );
      break;
      case 3: //3rd effect
      u8g.firstPage();
      do {
          u8g.setFont(u8g_font_helvR14r);
          u8g.drawStr( 10, 16, "SELECTION");    
          u8g.drawStr( 5, 50, "DISTORTION");
          } while( u8g.nextPage() );
      break;
      case 4: //4th effect
      u8g.firstPage();
      do {
          u8g.setFont(u8g_font_helvR14r);
          u8g.drawStr( 10, 16, "SELECTION");    
          u8g.drawStr( 38, 50, "FUZZ");
          } while( u8g.nextPage() );
      break;
      case 5: //5th effect
      u8g.firstPage();
      do {
          u8g.setFont(u8g_font_helvR14r);
          u8g.drawStr( 10, 16, "SELECTION");    
          u8g.drawStr( 0, 50, "BIT CRUSHER");
          } while( u8g.nextPage() );
      break;
    }
  }
  /* effect mode */
  if (digitalRead(FOOTSWITCH)) 
  {
    digitalWrite(LED, HIGH);
    switch(effect_type)
    {
      case 1: //1st effect
      u8g.firstPage();
      do {
          u8g.setFont(u8g_font_helvR14r);
          u8g.drawStr( 0, 16, "DELAY");    
          u8g.setPrintPos(10, 50); 
          u8g.setFont(u8g_font_helvR24r); 
          u8g.print(DelayDepth);
          } while( u8g.nextPage() );
      break;
      case 2: //2nd effect
      u8g.firstPage();
      do {
          u8g.setFont(u8g_font_helvR14r);
          u8g.drawStr( 0, 16, "ECHO");    
          u8g.setPrintPos(10, 50); 
          u8g.setFont(u8g_font_helvR24r); 
          u8g.print(EchoDepth);
          } while( u8g.nextPage() );
      break;
      case 3: //3rd effect
      u8g.firstPage();
      do {
          u8g.setFont(u8g_font_helvR14r);
          u8g.drawStr( 0, 16, "DISTORTION");    
          u8g.setPrintPos(10, 50); 
          u8g.setFont(u8g_font_helvR24r); 
          u8g.print(distortion_threshold);
          } while( u8g.nextPage() );
      break;
      case 4: //4th effect
      u8g.firstPage();
      do {
          u8g.setFont(u8g_font_helvR14r);
          u8g.drawStr( 0, 16, "FUZZ");    
          u8g.setPrintPos(10, 50); 
          u8g.setFont(u8g_font_helvR24r); 
          u8g.print(fuzz_threshold);
          } while( u8g.nextPage() );
      break;
      case 5: //5th effect
      u8g.firstPage();
      do {
          u8g.setFont(u8g_font_helvR14r);
          u8g.drawStr( 0, 16, "BIT CRUSHER");    
          u8g.setPrintPos(10, 50); 
          u8g.setFont(u8g_font_helvR24r); 
          u8g.print(bit_crush_variable);
          } while( u8g.nextPage() );
      break;
    } 
  }
  else  
  {  
   digitalWrite(LED, LOW);
    u8g.firstPage(); 
    do {
      u8g.setFont(u8g_font_helvR24r);
      u8g.drawStr( 0, 30, "EFFECT");    
      u8g.drawStr( 0, 60, "  OFF ");   
    } while( u8g.nextPage() );
  }
}
 
ISR(TIMER4_CAPT_vect) 
{
  //button status
  counter++; 
  if(counter==2000)
  {
    counter=0;
    //increase
    if (!digitalRead(PUSHBUTTON_2) && !digitalRead(TOGGLE)) 
    {
      switch(effect_type)
      {
        case 1: //1st effect
        if (DelayDepth<MAX_DELAY)DelayDepth=DelayDepth+25; //increase delay
        break;
        case 2: //2nd effect
        if (EchoDepth<MAX_DELAY)EchoDepth=EchoDepth+25; //increase echo
        break;
        case 3: //3rd effect
        if (distortion_threshold<MAX_OUTPUT)distortion_threshold=distortion_threshold+25; //increase distortion threshold
        break;
        case 4: //4th effect
        if (fuzz_threshold<MAX_OUTPUT)fuzz_threshold=fuzz_threshold+25; //increase fuzz threshold
        break;
        case 5: //5th effect
        if (bit_crush_variable<MAX_BIT)bit_crush_variable=bit_crush_variable+1; //increase the number of crushed bits
        break;
      }
    }
    //decrease
    if (!digitalRead(PUSHBUTTON_1) && !digitalRead(TOGGLE)) 
    {
      switch(effect_type)
      {
        case 1: //1st effect
        if (DelayDepth>MIN_DELAY)DelayDepth=DelayDepth-25; //decrease delay
        break;
        case 2: //2nd effect
        if (EchoDepth>MIN_DELAY)EchoDepth=EchoDepth-25; //decrease echo
        break;
        case 3: //3rd effect
        if (distortion_threshold>MIN_OUTPUT)distortion_threshold=distortion_threshold-25; //decrease distortion threshold
        break;
        case 4: //4th effect
        if (fuzz_threshold>MIN_OUTPUT)fuzz_threshold=fuzz_threshold-25; //decrease fuzz threshold
        break;
        case 5: //5th effect
        if (bit_crush_variable>MIN_BIT)bit_crush_variable=bit_crush_variable-1; //decrease the number of crushed bits
        break;
      }
    }
  }

  //get ADC data
  ADC_low = ADCL;
  ADC_high = ADCH;
  
  //construct the input sample summing the ADC low and high byte.
  output = ((ADC_high << 8) | ADC_low) + 0x8000;

  //audio processing
  switch(effect_type)
  {
    case 1: //1st effect: delay
    DelayBuffer[DelayCounter] = (ADC_high)>>1 ; 
    DelayCounter++;
    if(DelayCounter >= DelayDepth) DelayCounter = 0; 
    output = ((((DelayBuffer[DelayCounter]+(ADC_high))>>1) << 8) | ADC_low) + 0x8000;
    break;

    case 2: //2nd effect: echo
    DelayBuffer[DelayCounter] = (ADC_high + DelayBuffer[DelayCounter])>>1 ; 
    DelayCounter++;
    if(DelayCounter >= EchoDepth) DelayCounter = 0; 
    output = ((((DelayBuffer[DelayCounter]+(ADC_high))>>1) << 8) | ADC_low) + 0x8000;
    break;

    case 3: //3rd effect: distortion
    if(output>distortion_threshold) output=distortion_threshold;
    if(output<-distortion_threshold) output=-distortion_threshold;
    break;

    case 4: //4th effect: fuzz
    if(output>fuzz_threshold) output=32768;
    if(output<-fuzz_threshold) output=-32768;
    break;

    case 5: //5th effect: bit crusher
    output = output<<bit_crush_variable;
    break;
  }
 
  //write the PWM signal
  OCR4AL = ((output + 0x8000) >> 8);;
  OCR4BL = output;
}