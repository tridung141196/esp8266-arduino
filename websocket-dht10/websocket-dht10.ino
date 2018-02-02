#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>
#define trigPin 13
#define echoPin 12
const char* ssid = "Redmi";
const char* password = "hahahaha";
const int LED = 16;
const int BTN = 0;
int Auto=1,limit=50, lastState=0;
// để đưa đoạn code HTML vào chương trình Arduino, cần chuyển đổi code HTML sang dạng char
const char index_html[] PROGMEM = ""
"<!DOCTYPE HTML>"
"<html>"
"<head>"
"<title>ESP8266 WebSocket</title>"
"</head>"
"<body>"
"<div> Webscoket status <span id=\"status\" ></span> </div>"
"<div> Khoan cach hien tai: <input type='text' size='16' id='dis' value='0'> </div>"
"<div> Mode <select id='mode'>"
"<option value='1'>Auto</option>"
"<option value='00'>Motor On</option>"
"<option value='01'>Motor Off</option>"
"</select> khoan cach <input type='text' size='1' id='limit' value='50'></div>"
"<button onClick='onClick()'>OK</button>"
"<script type='text/javascript'>"
"var url = window.location.host;"
"var ws = new WebSocket('ws://' + url + '/ws');"
"ws.onopen = function()"
"{"
"document.getElementById('status').style = 'color:green';"
"document.getElementById('status').innerHTML = 'Connected';"
"};"
"ws.onmessage = function(evt)"
"{"
"document.getElementById('dis').value = evt.data;"
"};"
"ws.onclose = function() {"
"document.getElementById('status').style = 'color:red';"
"document.getElementById('status').innerHTML = 'Disconnected';"
"};"
"function onClick () {"
"var status = 'LED_OFF';"
"data = document.getElementById('mode').value+document.getElementById('limit').value;"
"ws.send(data)"
"}"
"</script>"
"</body>"
"</html>";

AsyncWebServer server(8000);
AsyncWebSocket ws("/ws");
// Hàm xử lí sự kiện trên Server khi client là browser phát sự kiện
int convert(uint8_t num){
  int data = (int)num;
  int out;
  if( data>47 && data <58)
    out = data -48;
    else out=data-87;
    return out;
}
void onWsEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len) {
  if (type == WS_EVT_DATA && len > 0) { // type: loại sự kiện mà server nhận được. Nếu sự kiện nhận được là từ websocket thì bắt đầu xử lí
    data[len] = 0;
    Auto = convert(data[0]);
    if(Auto==0){
     if(convert(data[1])==0)
        digitalWrite(LED, LOW);
     else
        digitalWrite(LED, HIGH);
    }
    else{
      limit = 0;
      for(int i=1;i<len;i++){
        limit += convert(data[i])*pow(10,len-i-1);
      }
    }
  }
}

void setup()
{
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(LED, OUTPUT);
  pinMode(BTN, INPUT);
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  WiFi.mode(WIFI_AP_STA);
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.printf("STA: Failed!\n");
    WiFi.disconnect(false);
    delay(1000);
    WiFi.begin(ssid, password);
  }
//  WiFi.softAP("votridung");
//
//  IPAddress myIP = WiFi.softAPIP();
//  Serial.print("AP IP address: ");
//  Serial.println(myIP);
  ws.onEvent(onWsEvent); // gọi hàm onWsEvent
  server.addHandler(&ws);
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
  request->send_P(200, "text/html", index_html); // trả về file index.html trên giao diện browser khi browser truy cậpvào IP của server
  });
  server.begin(); // khởi động server
}
void loop()
{
  long duration, distance;
  digitalWrite(trigPin, LOW);  // Added this line
  delayMicroseconds(2); // Added this line
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10); // Added this line
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) / 29.1;
  Serial.println(distance);
    if(distance != lastState)
      ws.textAll(String(distance));
    if(Auto){
      if(distance > limit)
        digitalWrite(LED, LOW);
      else
        digitalWrite(LED, HIGH);
    }
    delay(1000);
}
