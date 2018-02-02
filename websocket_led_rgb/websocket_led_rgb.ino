#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>
#include <Adafruit_NeoPixel.h>
#define DATA_PIN 15    //data in pin
#define TOTAL_LED 12
const char *ssid = "dungvo";
//const char* ssid = "Wifi";
//const char* password = "quenpassroi";
const int LED = 16;
const int BTN = 0;
unsigned int setColor;
int hieuUng=0;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(TOTAL_LED, DATA_PIN, NEO_GRB + NEO_KHZ800);
// để đưa đoạn code HTML vào chương trình Arduino, cần chuyển đổi code HTML sang dạng char
const char index_html[] PROGMEM = ""
"<!DOCTYPE HTML>"
"<html>"
"<head>"
"<title>ESP8266 WebSocket</title>"
"</head>"
"<body>"
"<div> Webscoket status <span id=\"status\" style=\"font-weight: bold;\"> value=\"disconected\" </span> </div>"
"<div> ESP8266 Button Status <input type=\"checkbox\" id=\"btn\" name=\"btn\" /> </div>"
"<div> Control LED <input type=\"checkbox\" id=\"led\" name=\"led\" disabled=\"true\" /> </div>"
"<div> hieu ung <input type=\"text\" id=\"hu\" name=\"hu\" value=\"1\"></div>"
"<div> chon mau <input type=\"color\" id=\"color\" name=\"color\" value=\"#ff0000\"> </div>"
"<script type=\"text/javascript\">"
"var button = document.getElementById('btn');"
"var led = document.getElementById('led');"
"var hieuung = document.getElementById('hu');"
"var status = document.getElementById('status');"
"var mau = document.getElementById('color');"
"var click = document.getElementById('ok');"
"var url = window.location.host;"
"var ws = new WebSocket('ws://' + url + '/ws');"
"ws.onopen = function()"
"{"
"status.value = 'Connected';"
"led.disabled = false;"
"};"
"ws.onmessage = function(evt)"
"{"
"if(evt.data == 'BTN_PRESSED') {"
"button.checked = true;"
"} else if(evt.data == 'BTN_RELEASE') {"
"button.checked = false;"
"}"
"};"
"ws.onclose = function() {"
"led.disabled = true;"
"status.value = 'Disconnected';"
"};"
"led.onchange = function() {"
"var status = 'LED_OFF';"
"status = hieuung.value;"
"ws.send(status);"
"}"
//"click.click = function() {"
//"var status2 = 'LED_OFF';"
//"status2 = hieuung.value;"
//"ws.send(status2);"
//"}"
"</script>"
"<div>  <input type=\"button\" id=\"ok\" onclick=\"ws.send(hieuung.value+mau.value)\" value=\"ok\"> </div>"
"</body>"
"</html>";

AsyncWebServer server(8000);
AsyncWebSocket ws("/ws");


// Hàm xử lí sự kiện trên Server khi client là browser phát sự kiện
int conver(uint8_t num){
  int data = (int)num;
  int out;
  Serial.println(data);
  if( data>47 && data <58)
    out = data -48;
    else out=data-87;
    return out;
}
void onWsEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data,size_t len) {
  if (type == WS_EVT_DATA && len > 0) { // type: loại sự kiện mà server nhận được. Nếu sự kiện nhận được là từ websocket thì bắt đầu xử lí
    data[len] = 0;
  hieuUng = conver(data[0]);
  setColor = strip.Color(conver(data[2])*16+conver(data[3]), conver(data[4])*16+conver(data[5]), conver(data[6])*16+conver(data[7]));
  }
}

void colorWipe(unsigned int c, unsigned int wait)
{
  for (int i = TOTAL_LED; i >= 0 ; i--) //numPixels = numLEDs
  {
    strip.setPixelColor(i, c); //set color for once LED
    strip.show();
  }
}

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


//rainbow led for all
void rainbow(unsigned int wait)
{
  int i, j;

  for (j = 0; j < 256; j++)
  {
    for (i = 0; i < TOTAL_LED; i++)
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
    for (i = 0; i < TOTAL_LED; i++)
    {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
  }
}
void hieu_ung1(unsigned int c1)
{
  for(int i=1;i<13;i++)
  {
        for(int j=1;j<13;j++)
        {
          if(j==i){
            strip.setPixelColor(j,strip.Color(100, 100, 20));
  //          strip.setPixelColor(j+1,strip.Color(100, 100, 20));
          }

          else
            strip.setPixelColor(j, c1);
        }
        strip.show();
        delay(100);
  }
}

//change LED from green -> red -> blue -> green (full color)

void setup()
{
  pinMode(LED, OUTPUT);
  pinMode(BTN, INPUT);
  strip.begin();
  strip.setBrightness(10);   //setup brightness
  strip.show();
  Serial.begin(115200);
  Serial.setDebugOutput(true);
//  WiFi.mode(WIFI_AP_STA);
//  WiFi.begin(ssid, password);
//  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
//  Serial.printf("STA: Failed!\n");
//  WiFi.disconnect(false);
//  delay(1000);
//  WiFi.begin(ssid, password);
//  }
  WiFi.softAP(ssid);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  ws.onEvent(onWsEvent); // gọi hàm onWsEvent
  server.addHandler(&ws);
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
  request->send_P(200, "text/html", index_html); // trả về file index.html trên giao diện browser khi browser truy cập vào IP của server
  });
  server.begin(); // khởi động server
}

void loop()
{
  if(hieuUng == 1)
  {
      colorWipe(setColor, 50);
  }
  else if(hieuUng == 2)
  {
      hieu_ung1(setColor);
  }
  else if(hieuUng == 3)
  {
      rainbow(1);
  }
  else if(hieuUng == 4)
  {
      rainbowCycle(1);
  }
}



