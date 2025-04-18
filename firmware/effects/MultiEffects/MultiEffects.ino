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
int output;
int counter = 0;
int effect = 1;
unsigned int ADC_low, ADC_high;

//effect variables
#define EFFECT_NUMBER 9
#define MAX_DELAY 5000
#define MIN_DELAY 20
#define MAX_BIT 8
#define MIN_BIT 1
#define MAX_LEVEL 32768
#define MIN_LEVEL 1000
#define MAX_octaveCrusher 50
#define MIN_octaveCrusher 1
#define MAX_VIBRATO 200
#define MIN_VIBRATO 10
#define MAX_CHORUS 200
#define MIN_CHORUS 10
byte audioBuffer[MAX_DELAY];
int audioCounter = 0;
int delayDepth = 1000;
int echoDepth = 4000;
int octaverDelay = 1000;
int vibratoDepth = 100;
int chorusDepth = 100;
int lfoDepth = 100;
int distortion = 6000;
int fuzz = 6000;
int bitCrush = 2;
int octaveCrusher = 15;
bool octaverMode = 0;
bool countUp = false;
bool writePWM = true;
 
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
    if(!digitalRead(PUSHBUTTON_2)) if(effect<EFFECT_NUMBER) effect++;
    if(!digitalRead(PUSHBUTTON_1)) if(effect>1) effect--;
    switch(effect)
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
      case 6: //6th effect
      u8g.firstPage();
      do {
          u8g.setFont(u8g_font_helvR14r);
          u8g.drawStr( 10, 16, "SELECTION");    
          u8g.drawStr( 18, 50, "OCTAVER");
          } while( u8g.nextPage() );
      break;
      case 7: //7th effect
      u8g.firstPage();
      do {
          u8g.setFont(u8g_font_helvR14r);
          u8g.drawStr( 10, 16, "SELECTION");    
          u8g.setFont(u8g_font_helvR10r);
          u8g.drawStr( 0, 50, "OCTAVE CRUSHER");
          } while( u8g.nextPage() );
      break;
      case 8: //8th effect
      u8g.firstPage();
      do {
          u8g.setFont(u8g_font_helvR14r);
          u8g.drawStr( 10, 16, "SELECTION");    
          u8g.drawStr( 22, 50, "VIBRATO");
          } while( u8g.nextPage() );
      break;
      case 9: //9th effect
      u8g.firstPage();
      do {
          u8g.setFont(u8g_font_helvR14r);
          u8g.drawStr( 10, 16, "SELECTION");    
          u8g.drawStr( 22, 50, "CHORUS");
          } while( u8g.nextPage() );
      break;
    }
  }
  /* effect mode */
  if (digitalRead(FOOTSWITCH)) 
  {
    digitalWrite(LED, HIGH);
    switch(effect)
    {
      case 1: //1st effect
      u8g.firstPage();
      do {
          u8g.setFont(u8g_font_helvR14r);
          u8g.drawStr( 0, 16, "DELAY");    
          u8g.setPrintPos(10, 50); 
          u8g.setFont(u8g_font_helvR24r); 
          u8g.print(delayDepth);
          } while( u8g.nextPage() );
      break;
      case 2: //2nd effect
      u8g.firstPage();
      do {
          u8g.setFont(u8g_font_helvR14r);
          u8g.drawStr( 0, 16, "ECHO");    
          u8g.setPrintPos(10, 50); 
          u8g.setFont(u8g_font_helvR24r); 
          u8g.print(echoDepth);
          } while( u8g.nextPage() );
      break;
      case 3: //3rd effect
      u8g.firstPage();
      do {
          u8g.setFont(u8g_font_helvR14r);
          u8g.drawStr( 0, 16, "DISTORTION");    
          u8g.setPrintPos(10, 50); 
          u8g.setFont(u8g_font_helvR24r); 
          u8g.print(distortion);
          } while( u8g.nextPage() );
      break;
      case 4: //4th effect
      u8g.firstPage();
      do {
          u8g.setFont(u8g_font_helvR14r);
          u8g.drawStr( 0, 16, "FUZZ");    
          u8g.setPrintPos(10, 50); 
          u8g.setFont(u8g_font_helvR24r); 
          u8g.print(fuzz);
          } while( u8g.nextPage() );
      break;
      case 5: //5th effect
      u8g.firstPage();
      do {
          u8g.setFont(u8g_font_helvR14r);
          u8g.drawStr( 0, 16, "BIT CRUSHER");    
          u8g.setPrintPos(10, 50); 
          u8g.setFont(u8g_font_helvR24r); 
          u8g.print(bitCrush);
          } while( u8g.nextPage() );
      break;
      case 6: //6th effect
      u8g.firstPage();
      do {
          u8g.setFont(u8g_font_helvR14r);
          u8g.drawStr( 18, 16, "OCTAVER");
          switch(octaverMode){
            case true: u8g.drawStr( 52, 50, "UP"); break;
            case false: u8g.drawStr( 35, 50, "DOWN"); break;
          } 
      } while( u8g.nextPage() );
      break;
      case 7: //7th effect
      u8g.firstPage();
      do {
        u8g.setFont(u8g_font_helvR10r);
        u8g.drawStr( 0, 16, "OCTAVE CRUSHER");
        u8g.setPrintPos(45, 50); 
        u8g.setFont(u8g_font_helvR24r); 
        u8g.print(octaveCrusher);
      } while( u8g.nextPage() );
      break;
      case 8: //8th effect
      u8g.firstPage();
      do {
        u8g.setFont(u8g_font_helvR14r);
        u8g.drawStr( 22, 16, "VIBRATO");
        u8g.setFont(u8g_font_helvR24r); 
        u8g.setPrintPos(10, 60); 
        u8g.print(vibratoDepth);
        u8g.setFont(u8g_font_helvR14r); 
        u8g.setPrintPos(100, 60); 
        u8g.print(countUp);

      } while( u8g.nextPage() );
      break;
      case 9: //9th effect
      u8g.firstPage();
      do {
        u8g.setFont(u8g_font_helvR14r);
        u8g.drawStr( 22, 16, "CHORUS");
        u8g.setFont(u8g_font_helvR24r); 
        u8g.setPrintPos(10, 60); 
        u8g.print(chorusDepth);
        u8g.setFont(u8g_font_helvR14r); 
        u8g.setPrintPos(100, 60); 
        u8g.print(countUp);

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
      u8g.drawStr( 32, 60, "OFF");   
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
      switch(effect)
      {
        case 1: //1st effect
        if (delayDepth<MAX_DELAY)delayDepth+=25; //increase delay
        break;
        case 2: //2nd effect
        if (echoDepth<MAX_DELAY)echoDepth+=25; //increase echo
        break;
        case 3: //3rd effect
        if (distortion<MAX_LEVEL)distortion+=25; //increase distortion threshold
        break;
        case 4: //4th effect
        if (fuzz<MAX_LEVEL)fuzz+=25; //increase fuzz threshold
        break;
        case 5: //5th effect
        if (bitCrush<MAX_BIT)bitCrush++; //increase the number of crushed bits
        break;
        case 6: //6th effect
        if(!octaverMode)octaverMode=true; //change the octave mode
        break;
        case 7: //7th effect
        if (octaveCrusher<MAX_octaveCrusher)octaveCrusher++; //increase octave crusher
        break;
        case 8: //8th effect
        if (vibratoDepth<MAX_VIBRATO)vibratoDepth++; //increase vibrato depth
        break;
        case 9: //9th effect
        if (chorusDepth<MAX_CHORUS)chorusDepth++; //increase chorus depth
        break;
      }
    }
    //decrease
    if (!digitalRead(PUSHBUTTON_1) && !digitalRead(TOGGLE)) 
    {
      switch(effect)
      {
        case 1: //1st effect
        if (delayDepth>MIN_DELAY)delayDepth-=25; //decrease delay
        break;
        case 2: //2nd effect
        if (echoDepth>MIN_DELAY)echoDepth-=25; //decrease echo
        break;
        case 3: //3rd effect
        if (distortion>MIN_LEVEL)distortion-=25; //decrease distortion threshold
        break;
        case 4: //4th effect
        if (fuzz>MIN_LEVEL)fuzz-=25; //decrease fuzz threshold
        break;
        case 5: //5th effect
        if (bitCrush>MIN_BIT)bitCrush--; //decrease the number of crushed bits
        break;
        case 6: //6th effect
        if (octaverMode)octaverMode=false; //change the octave mode
        break;
        case 7: //7th effect
        if (octaveCrusher>MIN_octaveCrusher)octaveCrusher--; //decrease octave crusher
        break;
        case 8: //8th effect
        if (vibratoDepth>MIN_VIBRATO)vibratoDepth--; //decrease vibrato depth
        break;
        case 9: //9th effect
        if (chorusDepth>MIN_CHORUS)chorusDepth--; //decrease chorus depth
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
  switch(effect)
  {
    case 1: //1st effect: delay
    audioBuffer[audioCounter] = ADC_high>>1 ; 
    audioCounter++;
    if(audioCounter >= delayDepth) audioCounter = 0; 
    output = ((((audioBuffer[audioCounter]+(ADC_high))>>1) << 8) | ADC_low) + 0x8000;
    break;

    case 2: //2nd effect: echo
    audioBuffer[audioCounter] = (ADC_high + audioBuffer[audioCounter])>>1 ; 
    audioCounter++;
    if(audioCounter >= echoDepth) audioCounter = 0; 
    output = ((((audioBuffer[audioCounter]+(ADC_high))>>1) << 8) | ADC_low) + 0x8000;
    break;

    case 3: //3rd effect: distortion
    if(output>distortion) output=distortion;
    if(output<-distortion) output=-distortion;
    break;

    case 4: //4th effect: fuzz
    if(output>fuzz) output=32768;
    if(output<-fuzz) output=-32768;
    break;

    case 5: //5th effect: bit crusher
    output = output<<bitCrush;
    break;

    case 6: //6th effect: octaver
    audioBuffer[audioCounter] = ADC_high>>1;
    audioCounter++;
    if(audioCounter >= octaverDelay) audioCounter = 0;
    switch(octaverMode){
      case true: output = ((audioBuffer[(audioCounter*2+octaverDelay/2)%octaverDelay] << 8) | ADC_low) + 0x8000; break;
      case false: output = ((audioBuffer[(int(audioCounter/2)+octaverDelay/2)%octaverDelay] << 8) | ADC_low) + 0x8000; break;}
    break;

    case 7: //7th effect: octave crusher
    audioCounter++;
    if(audioCounter >= MAX_DELAY) audioCounter = 0;
    if(audioCounter%octaveCrusher) writePWM = false;
    break;

    case 8: //8th effect: vibrato
    audioBuffer[audioCounter] = ADC_high>>1;
    audioCounter++;
    if(audioCounter>=lfoDepth){
      audioCounter=0;
      if(countUp){
        for(int p=0;p<10;p++) audioBuffer[lfoDepth+p]=audioBuffer[lfoDepth-1]; 
        lfoDepth++;
        if(lfoDepth>=vibratoDepth) countUp=false;
      }
      else {
        lfoDepth--;
        if(lfoDepth<=MIN_VIBRATO) countUp=true;
      }
    }
    output = ((audioBuffer[audioCounter] << 8) | ADC_low) + 0x8000;
    break;

    case 9: //9th effect: chorus
    audioBuffer[audioCounter] = ADC_high>>1;
    audioCounter++;
    if(audioCounter>=lfoDepth){
      audioCounter=0;
      if(countUp){
        for(int p=0;p<10;p++) audioBuffer[lfoDepth+p]=audioBuffer[lfoDepth-1]; 
        lfoDepth++;
        if(lfoDepth>=chorusDepth) countUp=false;
      }
      else {
        lfoDepth--;
        if(lfoDepth<=MIN_CHORUS) countUp=true;
      }
    }
    output = (((audioBuffer[audioCounter] << 8) | ADC_low) + 0x8000) + output;
    break;
  }

  //write the PWM signal
  if (writePWM){
    OCR4AL = ((output + 0x8000) >> 8);
    OCR4BL = output;
  }

  // Reset flag
  writePWM = true; 
  
}
