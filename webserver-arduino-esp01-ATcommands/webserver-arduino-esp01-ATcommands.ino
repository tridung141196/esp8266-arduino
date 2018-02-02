#include <SoftwareSerial.h>
#include <DHT.h>

#define DEBUG false
const int DHTPIN = 9; //Chân đọc dữ liệu từ DHT
const int DHTTYPE = DHT22; //Chọn kiểu cảm biên (DHT11 hoặc DHT22))
DHT dht(DHTPIN, DHTTYPE);

SoftwareSerial Serial1(2, 3); //  10-RX, 11-TX 

//Đoạn Code HTML sẽ hiển thị lên trình duyệt.
String html = 
"<html>\
  <head>\
<title>ESP8266 Webserver</title>\
  </head>\
  <body>\
  <h1>ARDUINO STATER</h1>\
  <a>LED  </a>\
  <a href=\"/LED=ON\">ON </a>\
  <a href=\"/LED=OFF\">OFF </a><br>\
  </body>\
</html>";
 
void setup()
{
  Serial.begin(9600);    //Khởi động Serial Monitor
  Serial1.begin(115200); //Khởi động UART kết nối ESP-01
  pinMode(13,OUTPUT);    //Khai báo LED 13
  digitalWrite(13,LOW);  //Tắt LED 13
 
  sendData("AT+RST\r\n",2000,DEBUG); // reset module
  sendData("AT+CWMODE=2\r\n",1000,DEBUG); // Khai báo ESP-01 là Access Point
  sendData("AT+CWSAP=\"CONTROL\",\"12345678\",5,3\r\n",5000,DEBUG); //Tạo điểm phát Wifi với ID và Pass.
  sendData("AT+CIFSR\r\n",1000,true); //Hiện IP của ESP-01
  sendData("AT+CIPMUX=1\r\n",1000,DEBUG); // configure for multiple connections
  sendData("AT+CIPSERVER=1,80\r\n",1000,DEBUG); // turn on server on port 80
}
 
void loop()
{
  String IncomingString=""; 
  boolean StringReady = false;
  while(Serial1.available())
  {
    IncomingString=Serial1.readString();
    StringReady= true;
    if (StringReady){
    Serial.println("Received String: " + IncomingString);
      if (IncomingString.indexOf("LED=ON") != -1) 
      {
        digitalWrite(13,HIGH);
      }
 
      if (IncomingString.indexOf("LED=OFF") != -1) 
      {
        digitalWrite(13,LOW);
      }
      else
      {
        String webpage = "<h1>Hello World</h1><a href=\"\">link text</a>";
        espsend(html);
        float temp = dht.readTemperature();
        float humi = dht.readHumidity();
        if (isnan(temp) || isnan(humi))
        {
          String c="sensor is not conneted";
          espsend(c);  
        }
      
        else
        {
          String add1="<h4>Temperature=</h4>";
          add1+=  String(temp);
          add1+="&#x2103";   //////////Hex code for degree celcius
          add1+="<h4>Humidityh=</h4>";
          add1+=  String(humi);
          add1+="%";   //////////Hex code for degree celcius
          espsend(add1);                                   
        } 
      }
     
      String closeCommand = "AT+CIPCLOSE=0\r\n";  //Lệnh ngắt đóng kết nối với thiết bị
      sendData(closeCommand,3000,DEBUG);
    }
  }
}
 
  //////////////////////////////sends data from ESP to webpage///////////////////////////
 
 void espsend(String d)
  {
    String cipSend = " AT+CIPSEND=0,";
    cipSend +=d.length();
    cipSend +="\r\n";
    sendData(cipSend,1000,DEBUG);
    sendData(d,1000,DEBUG); 
  }

//////////////gets the data from esp and displays in serial monitor///////////////////////         
String sendData(String command, const int timeout, boolean debug)
{
  String response = "";
  Serial1.print(command);
  long int time = millis();
  while( (time+timeout) > millis())
  {
    while(Serial1.available())
      {
         char c = Serial1.read(); // read the next character.
         response+=c;
      }  
  }
  
  if(debug)
    {
    Serial.print("Response ="+response); //displays the esp response messages in arduino Serial monitor
    }
  return response;
}
