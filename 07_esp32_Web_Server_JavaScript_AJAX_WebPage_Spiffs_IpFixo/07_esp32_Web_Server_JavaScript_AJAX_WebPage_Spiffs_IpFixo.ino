// Necessário o arquivo index.html carregado na memória do esp32

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <FS.h>

ESP8266WebServer server;
uint8_t pin_led = LED_BUILTIN;
char* ssid = "ESP32";
char* password = "12345678";

// IP fixo
IPAddress local_IP(192, 168, 0, 112);
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(8, 8, 8, 8);
IPAddress secondaryDNS(8, 8, 4, 4);

void setup(){
  SPIFFS.begin();
  pinMode(pin_led, OUTPUT);
  
  WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS);
  WiFi.begin(ssid,password);
  Serial.begin(115200);
  while(WiFi.status()!=WL_CONNECTED){
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.on("/",serveIndexFile);
  server.on("/ledstate",getLEDState);
  server.begin();
}

void loop(){
  server.handleClient();
}

void serveIndexFile(){
  File file = SPIFFS.open("/index.html","r");
  server.streamFile(file, "text/html");
  file.close();
}

void toggleLED(){
  digitalWrite(pin_led,!digitalRead(pin_led));
}

void getLEDState(){
  toggleLED();
  String led_state = digitalRead(pin_led) ? "OFF" : "ON";
  server.send(200,"text/plain", led_state);
}
