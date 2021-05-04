#include "FastLED.h"
#define NUM_STRIPS 6  // The number of strips
#define NUM_LEDS 20   // The number of LEDs per strip
const int TOTAL_LEDS = NUM_LEDS*NUM_STRIPS;
CRGB leds[TOTAL_LEDS];
#define PIN A3 
// Fire Base Color 
// RED = 0, GREEN = 1, BLUE = 2, PURPLE = 3
#define FIRE_BASE_COLOR 0

void setup()
{
  FastLED.addLeds<WS2811, PIN, GRB>(leds, TOTAL_LEDS).setCorrection( TypicalLEDStrip );
  
  setAll(0,0,0);
}
// *** REPLACE FROM HERE ***
void loop() { 
  // Flame characteristics - Original values  55, 120, 15, 7
  int COOLING = 55;
  int SPARKING = 100;  // 255 max
  int SPARK_DELAY = 7;
  int SPARKING_LEDS = 3; //number of led which can start a new spark
  Fire(COOLING, SPARKING, SPARK_DELAY, SPARKING_LEDS);
  delay(10);
}

void Fire(int Cooling, int Sparking, int SpeedDelay, int SparkingLeds) {
  static byte heat[TOTAL_LEDS];
  int cooldown;
  
  // Step 1.  Cool down every cell a little
  for( int i = 0; i < TOTAL_LEDS; i++) {
    cooldown = random(0, ((Cooling * 10) / NUM_LEDS) + 2);
    
    if(cooldown>heat[i]) {
      heat[i]=0;
    } else {
      heat[i]=heat[i]-cooldown;
    }
  }
  
  // Step 2.  Heat from each cell drifts 'up' and diffuses a little
  for( int k= (TOTAL_LEDS) - 1; k >= 2; k--) {
    heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2]) / 3;
  }
    
  // Step 3.  Randomly ignite new 'sparks' near the bottom
  for (int stripNo=0; stripNo < NUM_STRIPS; stripNo++) {
    if( random(255) < Sparking ) {
      int y = (stripNo * NUM_LEDS) + random(SparkingLeds);
      heat[y] = heat[y] + random(160,255);
      //heat[y] = random(160,255);
    }
  }
  // Step 4.  Convert heat to LED colors
  for( int j = 0; j < TOTAL_LEDS; j++) {
    setPixelHeatColor(j, heat[j] );
  }
  showStrip();
  delay(SpeedDelay);
}
void setPixelHeatColor (int Pixel, byte temperature) {
  // Scale 'heat' down from 0-255 to 0-191
  byte t192 = round((temperature/255.0)*191);
 
  // calculate ramp up from
  byte heatramp = t192 & 0x3F; // 0..63
  heatramp <<= 2; // scale up to 0..252
  if (FIRE_BASE_COLOR == 1) { // GREEN
    
    // figure out which third of the spectrum we're in:
    if( t192 > 0x80) {                     // hottest
      setPixel(Pixel, heatramp, heatramp, 255);
    } else if( t192 > 0x40 ) {             // middle
      setPixel(Pixel, 128, 128, heatramp);
    } else {                               // coolest
      setPixel(Pixel, 0, heatramp, 0);
    }    
  } else if (FIRE_BASE_COLOR == 2) { // BLUE
    
    // figure out which third of the spectrum we're in:
    if( t192 > 0x80) {                     // hottest
      setPixel(Pixel, heatramp, 255, heatramp);
    } else if( t192 > 0x40 ) {             // middle
      setPixel(Pixel, 128, heatramp, 128 );
    } else {                               // coolest
      setPixel(Pixel, 0, 0, heatramp);
    }  
  } else if (FIRE_BASE_COLOR == 3) { // PURPLE
    
    // figure out which third of the spectrum we're in:
    if( t192 > 0x80) {                     // hottest
      setPixel(Pixel, heatramp, 255, heatramp);
    } else if( t192 > 0x40 ) {             // middle
      setPixel(Pixel, 128, heatramp, 128 );
    } else {                               // coolest
      setPixel(Pixel, heatramp, 0, heatramp);
    }
    
  } else { // RED
    
    // figure out which third of the spectrum we're in:
    if( t192 > 0x80) {                     // hottest
      setPixel(Pixel, 255, 255, heatramp);
    } else if( t192 > 0x40 ) {             // middle
      setPixel(Pixel, 255, heatramp, 0);
    } else {                               // coolest
      setPixel(Pixel, heatramp, 0, 0);
    }
  }
  
}
// *** REPLACE TO HERE ***
void showStrip() {
 #ifdef ADAFRUIT_NEOPIXEL_H 
   // NeoPixel
   strip.show();
 #endif
 #ifndef ADAFRUIT_NEOPIXEL_H
   // FastLED
   FastLED.show();
 #endif
}
void setPixel(int Pixel, byte red, byte green, byte blue) {
 #ifdef ADAFRUIT_NEOPIXEL_H 
   // NeoPixel
   strip.setPixelColor(Pixel, strip.Color(red, green, blue));
 #endif
 #ifndef ADAFRUIT_NEOPIXEL_H 
   // FastLED
   if (Pixel>=0 && Pixel<20) 
   {
    leds[19-Pixel].r = red;
    leds[19-Pixel].g = green;
    leds[19-Pixel].b = blue;
   }
   else
   if (Pixel>=40 && Pixel<60) 
   {
    leds[(59-Pixel)+40].r = red;
    leds[(59-Pixel)+40].g = green;
    leds[(59-Pixel)+40].b = blue;
   }
   else
   if (Pixel>=80 && Pixel<100) 
   {
    leds[(99-Pixel)+80].r = red;
    leds[(99-Pixel)+80].g = green;
    leds[(99-Pixel)+80].b = blue;
   }
   else
   {
   leds[Pixel].r = red;
   leds[Pixel].g = green;
   leds[Pixel].b = blue;
   }
 #endif
}
void setAll(byte red, byte green, byte blue) {
  for(int i = 0; i < 60; i++ ) {
    setPixel(i, red, green, blue); 
  }
  showStrip();
}
