#include <Adafruit_NeoPixel.h>

#define PIN 15

int state1,state2,hieu_ung=1,bufstate1=0,bufstate2=0,so_led=0;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(12, PIN, NEO_GRB + NEO_KHZ800);

void setup()
{
  strip.begin();
  strip.setBrightness(30);
  strip.show();
  //attachInterrupt(13,hieu_ung1,FALLING);
  //attachInterrupt(13,hieu_ung2,RISING);
}

void loop()
{

 
/*lua chon hieu ung tu touchpab 1*/
//  if(state1 != bufstate1 )
//  {
//    if(hieu_ung <5)
//       hieu_ung += 1;
//    else hieu_ung = 1;
//    bufstate1 = state1;  
//  }
hieu_ung = 4;

/*lua chon so led tu touchpad 2*/
//  if(state2 != bufstate2)
//  {
//    if(so_led<12)
//    so_led+=1;
//    else if(so_led == 12)
//    {
//      so_led=1;
//      colorWipe(strip.Color(0, 0, 0),12, 1);
//    }
//    bufstate2=state2;
//  }
so_led = 12;
    
  if(hieu_ung == 1)
  colorWipe(strip.Color(255, 0, 0),so_led, 50);
  else if(hieu_ung == 2)
  colorWipe(strip.Color(0, 255, 0),so_led, 50);
  else if(hieu_ung == 3)
  colorWipe(strip.Color(0, 0, 255),so_led, 50);
  else if(hieu_ung == 4)
  rainbow(1,so_led);
}

void colorWipe(unsigned int c,unsigned int so_led, unsigned int wait)
{
  for (int i = 0; i < so_led; i++) //numPixels = numLEDs
  {
    strip.setPixelColor(i, c); //set color for once LED
    strip.show();
    delay(wait);
  }
}

//rainbow led for all
void rainbow(unsigned int wait,unsigned int so_led)
{
  int i, j;

  for (j = 0; j < 256; j++)
  {
    for (i = 0; i < so_led; i++)
    {
      strip.setPixelColor(i, Wheel((i + j) & 255));
    }
    strip.show();
    delay(wait);
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
    delay(wait);
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
