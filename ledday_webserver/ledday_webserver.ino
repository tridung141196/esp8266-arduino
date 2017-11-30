#include <Adafruit_NeoPixel.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>


//const char *ssid = "dungvo";

#define DATA_PIN 15    //data in pin
#define TOTAL_LED 12

const char* ssid = "Wifi";
const char* password = "quenpassroi";

int btnPin = 12; //touch pin
int hieuUng=0; 
int NUM_LED=0;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(TOTAL_LED, DATA_PIN, NEO_GRB + NEO_KHZ800);
ESP8266WebServer server(80);

const char *html = \
"<html>\
  <head>\ 
    <title>ESP8266 Webserver</title>\
  </head>\
  <body>\
<a href=\"/chonhieuung\">ChonHieuUng</a>\
<a href=\"/chonsoled\">ChonSoLed</a>\
  </body>\
</html>";

void handleRoot()
{
  server.send(200, "text/plain", "<h1>You are connected</h1>");
}
void chonhieuung()
{
  led();
  server.send(200, "text/plain", "<a href=\"/inline\"> hieuungkhac</a>");
}
void chonsoled()
{
  NUM_LED +=1;
  if(NUM_LED == 13)
  NUM_LED = 0;
  server.send(200, "text/plain", "<a href=\"/outline\"> chonsoled</a>");
}

void setup()
{
  strip.begin();
  strip.setBrightness(10);   //setup brightness
  strip.show();
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");

  Serial.println(WiFi.localIP());
//  WiFi.softAP(ssid);
//
//  IPAddress myIP = WiFi.softAPIP();
//  Serial.print("AP IP address: ");
//  Serial.println(myIP);
  server.on("/", handleRoot);
  server.on("/inline",chonhieuung);
  server.on("/outline",chonsoled);
  server.begin();
  Serial.println("HTTP server started");
}
void loop()
{
  server.handleClient();
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
  for (int i = NUM_LED; i >= 0 ; i--) //numPixels = numLEDs
  {
    strip.setPixelColor(i*2, c); //set color for once LED
    strip.show();
  }
  for (int i = 0; i <= NUM_LED ; i++) //numPixels = numLEDs
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
    for (i = 0; i < NUM_LED; i++)
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
    for (i = 0; i < NUM_LED; i++)
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
