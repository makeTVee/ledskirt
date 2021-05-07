#include <Arduino.h>
#include <Adafruit_LSM6DSO32.h>
#include <npMeteor.h>
#include <npBouncingBall.h>

#define NeoPin     A3 // NeoPixel Pin
#define MAXPIXELS 120 // Number of Pixels
#define NSTRIPES 6 //Number of Stripes 

npNeoPixel pixels = npNeoPixel(MAXPIXELS, NeoPin, NEO_GRB + NEO_KHZ800, 8.0);
//-------------------------------------------------------------------------------------

// define 6 virtual strips 
// reversed for bouncing ball effect (counting from bottom to top)
npVirtualNeo vNeo1(&pixels, 19, 0);
npVirtualNeo vNeo2(&pixels, 20, 39);
npVirtualNeo vNeo3(&pixels, 59, 40);
npVirtualNeo vNeo4(&pixels, 60, 79);
npVirtualNeo vNeo5(&pixels, 99, 80);
npVirtualNeo vNeo6(&pixels, 100, 119);

//array of bouncing balls for easier access, different initial delay
npBouncingBall bballs[NSTRIPES] = {npBouncingBall(50,vNeo1),npBouncingBall(60,vNeo2),
                                    npBouncingBall(40,vNeo3),npBouncingBall(20,vNeo4),
                                    npBouncingBall(40,vNeo5),npBouncingBall(70,vNeo6)};


sensors_event_t accel_old;
Adafruit_LSM6DSO32 dso32;
float diff[3] = {0,0,0};
float diffsum;
byte *c;
float Motion_Theshold = 10.0;

//-------------------------------------------------------------------------------------
void setup ()
{
  // setup LSM6DS032 sensor
  if (!dso32.begin_I2C()) {
    while (1) {
      delay(10);
    }
  }
  dso32.setAccelRange(LSM6DSO32_ACCEL_RANGE_8_G);
  dso32.setAccelDataRate(LSM6DS_RATE_12_5_HZ);

  pixels.begin();
  pixels.clear();
  pixels.npShow();
}
//-------------------------------------------------------------------------------------
void loop ()
{
  sensors_event_t accel;
  sensors_event_t gyro;
  sensors_event_t temp;
  dso32.getEvent(&accel, &gyro, &temp);

  //calculating the diff to previous value for x,y and z axis
  diff[0]=abs(accel.acceleration.x - accel_old.acceleration.x);
  diff[1]=abs(accel.acceleration.y - accel_old.acceleration.y);
  diff[2]=abs(accel.acceleration.z - accel_old.acceleration.z);
  
  accel_old=accel;
  //react to all axis
  diffsum=diff[0]+diff[1]+diff[2];
  
  for(int i=0;i<NSTRIPES;i++) bballs[i].update();

  if (bballs[3].hasFinished() && (diffsum>Motion_Theshold))
  {
    //c=Wheel(random(0,255));

    for(int i=0;i<NSTRIPES;i++) 
    {
      //uncomment following line for fixed collor for the balls
      //and the upper line c=Wheel... 
      //bballs[i].changeColor(*c, *(c+1), *(c+2));
      bballs[i].restart();
    }
  }
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
