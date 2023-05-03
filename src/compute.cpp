#include <Arduino.h>
#include <SPI.h>// SPI Library used to clock data out to the shift registers

#define latch_pin 4
#define blank_pin 22
#define data_pin 23
#define clock_pin 18

int shift_out;
byte anode[8];

byte red0[64], red1[64], red2[64], red3[64];
byte blue0[64], blue1[64], blue2[64], blue3[64];
byte green0[64], green1[64], green2[64], green3[64];


int level=0;
int anodelevel=0;
int BAM_Bit, BAM_Counter=0;

unsigned long start;

hw_timer_t *My_timer = NULL;

void LED(int level, int row, int column, byte red, byte green, byte blue){
    // Serial.println("pass3");
  if(level<0)
  level=0;
  if(level>7)
  level=7;
  if(row<0)
  row=0;
  if(row>7)
  row=7;
  if(column<0)
  column=0;
  if(column>7)
  column=7;  
  if(red<0)
  red=0;
  if(red>15)
  red=15;
  if(green<0)
  green=0;
  if(green>15)
  green=15;
  if(blue<0)
  blue=0;
  if(blue>15)
  blue=15;  

  int whichbyte = int(((level*64)+(row*8)+column)/8);

  int wholebyte=(level*64)+(row*8)+column;

  bitWrite(red0[whichbyte], wholebyte-(8*whichbyte), bitRead(red, 0));
  bitWrite(red1[whichbyte], wholebyte-(8*whichbyte), bitRead(red, 1));
  bitWrite(red2[whichbyte], wholebyte-(8*whichbyte), bitRead(red, 2)); 
  bitWrite(red3[whichbyte], wholebyte-(8*whichbyte), bitRead(red, 3)); 

  bitWrite(green0[whichbyte], wholebyte-(8*whichbyte), bitRead(green, 0));
  bitWrite(green1[whichbyte], wholebyte-(8*whichbyte), bitRead(green, 1));
  bitWrite(green2[whichbyte], wholebyte-(8*whichbyte), bitRead(green, 2)); 
  bitWrite(green3[whichbyte], wholebyte-(8*whichbyte), bitRead(green, 3));

  bitWrite(blue0[whichbyte], wholebyte-(8*whichbyte), bitRead(blue, 0));
  bitWrite(blue1[whichbyte], wholebyte-(8*whichbyte), bitRead(blue, 1));
  bitWrite(blue2[whichbyte], wholebyte-(8*whichbyte), bitRead(blue, 2)); 
  bitWrite(blue3[whichbyte], wholebyte-(8*whichbyte), bitRead(blue, 3));
//   Serial.println("pass4");
}

void IRAM_ATTR onTimer(){
    // Serial.println("pass 5");
  digitalWrite(blank_pin, HIGH);

  if(BAM_Counter==8){
  BAM_Bit++;
  }

  else if(BAM_Counter==24){
  BAM_Bit++;
  }

  else if(BAM_Counter==56){
  BAM_Bit++;
  }

  BAM_Counter++;

  switch (BAM_Bit){
    case 0:
        for(shift_out=level; shift_out<level+8; shift_out++)
        SPI.transfer(red0[shift_out]);
        for(shift_out=level; shift_out<level+8; shift_out++)
        SPI.transfer(green0[shift_out]); 
        for(shift_out=level; shift_out<level+8; shift_out++)
        SPI.transfer(blue0[shift_out]);
        break;
    case 1:
        for(shift_out=level; shift_out<level+8; shift_out++)
        SPI.transfer(red1[shift_out]);
        for(shift_out=level; shift_out<level+8; shift_out++)
        SPI.transfer(green1[shift_out]); 
        for(shift_out=level; shift_out<level+8; shift_out++)
        SPI.transfer(blue1[shift_out]);
        break;
    case 2:
        for(shift_out=level; shift_out<level+8; shift_out++)
        SPI.transfer(red2[shift_out]);
        for(shift_out=level; shift_out<level+8; shift_out++)
        SPI.transfer(green2[shift_out]); 
        for(shift_out=level; shift_out<level+8; shift_out++)
        SPI.transfer(blue2[shift_out]);
        break;
    case 3:
        for(shift_out=level; shift_out<level+8; shift_out++)
        SPI.transfer(red3[shift_out]);
        for(shift_out=level; shift_out<level+8; shift_out++)
        SPI.transfer(green3[shift_out]); 
        for(shift_out=level; shift_out<level+8; shift_out++)
        SPI.transfer(blue3[shift_out]);

        if(BAM_Counter==120){
            BAM_Counter=0;
            BAM_Bit=0;
        }
        break;
  }
  SPI.transfer(anode[anodelevel]);

  digitalWrite(latch_pin, HIGH);
  digitalWrite(latch_pin,LOW);
  digitalWrite(blank_pin,LOW);

  anodelevel++;
  level = level + 8;

  if(anodelevel==8){
    anodelevel=0;
  }
  if(level==64){
    level=0;
  }
  pinMode(blank_pin, OUTPUT);
//   Serial.println("pass 6");
}




void clean(){
  int ii, jj, kk;
    for(ii=0; ii<8; ii++)
  for(jj=0; jj<8; jj++)
  for(kk=0; kk<8; kk++)
  LED(ii,jj,kk,0,0,0);
  
}

void setup() {

  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE0);
  // SPI.setClockDivider(SPI_CLOCK_DIV2);
  // SPI.setFrequency(16000000);
  SPI.setFrequency(16000);
    
  Serial.begin(115200);
    // Serial.println("pass1");



  anode[7]=B00000001;
  anode[6]=B00000010;
  anode[5]=B00000100;
  anode[4]=B00001000;
  anode[3]=B00010000;
  anode[2]=B00100000;
  anode[1]=B01000000;
  anode[0]=B10000000;

  pinMode(latch_pin, OUTPUT);//Latch
  pinMode(data_pin, OUTPUT);//MOSI DATA
  pinMode(clock_pin, OUTPUT);//SPI Clock
  pinMode(blank_pin, OUTPUT);//Output Enable  important to do this last, so LEDs do not flash on boot up
  SPI.begin();//start up the SPI library
//   Serial.println("pass2");
//   interrupts();//let the show begin, this lets the multiplexing start
//   My_timer = timerBegin(0, 320, true);
//   timerAttachInterrupt(My_timer, &onTimer, true);
//   timerAlarmWrite(My_timer, 30, true);
//   timerAlarmEnable(My_timer);
}

void loop() {
    // Serial.println("loop start");
    int start = micros();
    for (int i = 0; i < 16; i++)
    {
        for (int j = 0; j < 16; j++)
        {
            for (int k = 0; k < 16; k++)
            {
                LED(i, j, k, 6, 15, 15);
            }
            
        }
        
    }
    int end = micros();
    int calctime = end-start;
    Serial.println(calctime);
    
    // Serial.println("loop end");
    // delay(500);
    // clean();
    // delay(500);
}