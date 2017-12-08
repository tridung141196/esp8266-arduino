#include <ESP8266WiFi.h>

const char* ssid     = "Wifi";
const char* password = "quenpassroi";

const char* host = "192.168.1.105";

void setup() {
  Serial.begin(115200);
  delay(10);

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

int value = 0;

void loop() {
  delay(5000);
  Serial.print("connecting to ");
  Serial.println(host);

  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 8000;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }

  // We now create a URI for the request
  String url = "/getmode";

  Serial.print("Requesting URL: ");
  Serial.println(url);

  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\n" + "Connection: close\n" + "Content-Length: 0\n" + "\n\n");
  delay(10);
  Serial.println(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\n" + "Connection: close\n" + "Content-Length: 0\n" + "\n\n");

  // Read all the lines of the reply from server and print them to Serial
  Serial.println("Respond:");
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
//    client.flush();
//    if( line.indexOf("dung") != -1){
//      Serial.println(line.indexOf("dung"));
//    for(int i =0;i <line.length();i++){
//      Serial.println(line[i]);
//    }

}
  Serial.println();
  Serial.println("closing connection");
}
