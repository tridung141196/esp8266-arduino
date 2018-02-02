#include <ESP8266WiFi.h>
#include "DHT.h"
#define DHTPIN 12
#define DHTTYPE DHT11
DHT dht(DHTPIN,DHTTYPE);
WiFiClient client;
// Tạo 1 biến
const char* ssid = "Android";
const char* password = "123456789";
const char* server = "192.168.43.149";
const int port = 8000;
int Auto=1,limHum=70,limTemp=27;
void setup() {
  pinMode(16, OUTPUT);
  Serial.begin(115200);
  Serial.println("Connecting"); 
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(100);
    }
  Serial.println("\r\nWiFi connected");
//  WiFi.softAP("votridung");
//
//  IPAddress myIP = WiFi.softAPIP();
//  Serial.print("AP IP address: ");
//  Serial.println(myIP);
}


void loop() {
  pinMode(15,OUTPUT);
  pinMode(13,OUTPUT);
   pinMode(16,OUTPUT);
  String data;
  int count=1;
  float humi = dht.readHumidity();
  float temp = dht.readTemperature();
  float f = dht.readTemperature(true);
  if (isnan(temp) || isnan(humi)) {
 
Serial.println("Failed to read from DHT sensor!");
 
return;
  }
  if(Auto==1){
    if(temp>limTemp)
      digitalWrite(13,LOW);
    else
      digitalWrite(13,HIGH);
    if(humi>limHum)
      digitalWrite(15,LOW);
    else
      digitalWrite(15,HIGH);
  }
  if (client.connect(server, port)) {
String req_uri = "/update?temp=" + String(temp, 1) + "&humd=" + String(humi, 1);
 
  client.print("GET " + req_uri + " HTTP/1.1\n" + "Host: "+ server +"\n" + "Connection: close\n" + "Content-Length:0\n" +"\n\n");
  Serial.printf("Nhiet do %s - Do am %s\r\n", String(temp, 1).c_str(), String(humi, 1).c_str());
  delay(100);
  }
  client.stop();
  if (client.connect(server, port)) {
    String  req_uri = "/getmode";
    client.print("GET " + req_uri + " HTTP/1.1\n" + "Host: "+ server +"\n" + "Connection: close\n" + "Content-Length:0\n" +"\n\n");
    delay(10);
    while(client.available()){
      String line = client.readStringUntil('\r');
      count++;
      if(count == 9){
        data=line;
        Serial.println(data);
        int num=data.toInt();
        Auto = num/100000;
        limHum=num%100;
        limTemp=(num/100)%100;
        Serial.println(limTemp);
        Serial.println(limHum);
        if(Auto == 0){
          Serial.println((num/10000)%10);
          if((num/10000)%10==0){
            digitalWrite(16,LOW);
            
          }
          else
            digitalWrite(16,HIGH);
        }
      }
    }
  }
  client.stop();
}

