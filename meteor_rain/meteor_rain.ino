#define NeoPin     A3 // NeoPixel Pin
#define MAXPIXELS 120 // Number of Pixels

//-------------------------------------------------------------------------------------
#include <npMeteor.h>
npNeoPixel pixels = npNeoPixel(MAXPIXELS, NeoPin, NEO_GRB + NEO_KHZ800, 8.0);
//-------------------------------------------------------------------------------------
unsigned int Delay = 100;
// define 3 virtual strips running from 0-23, 47-24 (reverse direction) and 48-71
npVirtualNeo vNeo1(&pixels, 0, 19);
npVirtualNeo vNeo2(&pixels, 39, 20);
npVirtualNeo vNeo3(&pixels, 40, 59);
npVirtualNeo vNeo4(&pixels, 79, 60);
npVirtualNeo vNeo5(&pixels, 80, 99);
npVirtualNeo vNeo6(&pixels, 119, 100);
// define three meteors
// npMeteor(red, green, blue, length, decay, random, delay, vneostrip, keepBackground)
npMeteor Meteor1(0x30, 0xAA, 0x33, 1, 184, true, Delay, vNeo1, false);
npMeteor Meteor2(0x30, 0xAA, 0x33, 1, 184, true, Delay, vNeo2, false);
npMeteor Meteor3(0x30, 0xAA, 0x33, 1, 184, true, Delay, vNeo3, false);
npMeteor Meteor4(0x30, 0xAA, 0x33, 1, 184, true, Delay, vNeo4, false);
npMeteor Meteor5(0x30, 0xAA, 0x33, 1, 184, true, Delay, vNeo5, false);
npMeteor Meteor6(0x30, 0xAA, 0x33, 1, 184, true, Delay, vNeo6, false);
byte counter=1;
byte *c;
//-------------------------------------------------------------------------------------
void setup ()
{
  Serial.begin (115200); // Serial output with 115200 bps
  pixels.begin();
  pixels.clear();
  pixels.npShow();
}
//-------------------------------------------------------------------------------------
void loop ()
{
  Meteor1.update();
  Meteor2.update();
  Meteor3.update();
  Meteor4.update();
  Meteor5.update();
  Meteor6.update();
  
  if (counter%2==0)
  {
    c=Wheel(counter);
    Meteor1.changeColor(*c, *(c+1), *(c+2));
    Meteor2.changeColor(*c, *(c+1), *(c+2));
    Meteor3.changeColor(*c, *(c+1), *(c+2));
    Meteor4.changeColor(*c, *(c+1), *(c+2));
    Meteor5.changeColor(*c, *(c+1), *(c+2));
    Meteor6.changeColor(*c, *(c+1), *(c+2));
    counter=counter+5;
  }
  
  if (Meteor1.hasFinished()) {
    Meteor1.restart();
    counter++;
  }
  if (Meteor2.hasFinished()) Meteor2.restart();   
  if (Meteor3.hasFinished()) Meteor3.restart();   
  if (Meteor4.hasFinished()) Meteor4.restart();
  if (Meteor5.hasFinished()) Meteor5.restart();   
  if (Meteor6.hasFinished()) Meteor6.restart();  
}

byte * Wheel(byte WheelPos) {
  static byte c[3];
 
  if(WheelPos < 85) {
   c[0]=WheelPos * 3;
   c[1]=255 - WheelPos * 3;
   c[2]=0;
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   c[0]=255 - WheelPos * 3;
   c[1]=0;
   c[2]=WheelPos * 3;
  } else {
   WheelPos -= 170;
   c[0]=0;
   c[1]=WheelPos * 3;
   c[2]=255 - WheelPos * 3;
  }

  return c;
}
