#include <Adafruit_NeoPixel.h>

#define DATA_PIN 15    //data in pin

#define NUM_LED 12

int btnPin = 12; //touch pin
int hieuUng=0; 

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LED, DATA_PIN, NEO_GRB + NEO_KHZ800);

void setup()
{
  strip.begin();
  strip.setBrightness(150);   //setup brightness
  strip.show();
  pinMode(btnPin, INPUT);       // sets the digital pin as input
  pinMode(btnPin, INPUT_PULLUP); //pull-up button
  attachInterrupt(btnPin, led, FALLING);
  Serial.begin(115200);
}
void loop()
{
  if(hieuUng == 1)
  {
      colorWipe(strip.Color(255, 0, 0),strip.Color(0, 255, 0), 50);
  }
  else if(hieuUng ==2 )
  {
     colorWipe(strip.Color(255, 0, 0),strip.Color(255,0, 0), 50);
  }
  else if(hieuUng == 3)
  {
      colorWipe(strip.Color(255, 0, 255),strip.Color(255, 0, 255), 50);
  }
  else if(hieuUng == 4)
  {
      hieu_ung1(strip.Color(0, 255, 255),strip.Color(255, 0, 125));
  }
  else if(hieuUng == 5)
  {
      rainbow(1);
  }
  else if(hieuUng == 6)
  {
      rainbowCycle(1);
  }
}

void led()
{
  if(hieuUng < 7)
    hieuUng += 1;
  else
    hieuUng = 1;
  
}

void colorWipe(unsigned int c,unsigned int c2, unsigned int wait)
{
  for (int i = strip.numPixels(); i >= 0 ; i--) //numPixels = numLEDs
  {
    strip.setPixelColor(i*2, c); //set color for once LED
    strip.show();
  }
  for (int i = 0; i <= strip.numPixels() ; i++) //numPixels = numLEDs
  {
    strip.setPixelColor(i*2 + 1, c2); //set color for once LED
    strip.show();
  }

}

//rainbow led for all
void rainbow(unsigned int wait)
{
  int i, j;

  for (j = 0; j < 256; j++)
  {
    for (i = 0; i < strip.numPixels(); i++)
    {
      strip.setPixelColor(i, Wheel((i + j) & 255));
    }
   strip.show();
  }
}

// rainbow circle led
void rainbowCycle(unsigned int wait)
{
  uint16_t i, j;

  for (j = 0; j < 256 * 5; j++)
  { // 5 cycles of all colors on wheel
    for (i = 0; i < strip.numPixels(); i++)
    {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
  }
}
void hieu_ung1(unsigned int c1, unsigned int c2)
{
  for(int i=1;i<13;i++)
  {
        for(int j=1;j<13;j++)
        {
          if(j==i)
            strip.setPixelColor(j, c1);
          else
            strip.setPixelColor(j, c2);
        }
        strip.show();
        delay(100);
  }
}

//change LED from green -> red -> blue -> green (full color)
uint32_t Wheel(byte WheelPos)
{
  if (WheelPos < 85)
  {
    return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
  else if (WheelPos < 170)
  {
    WheelPos -= 85;
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  else
  {
    WheelPos -= 170;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}
