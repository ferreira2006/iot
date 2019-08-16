#include <ESP8266WiFi.h>
#include <FS.h>
#include <ESPAsyncWebServer.h>
 
const char* ssid = "ESP32";
const char* password =  "12345678";
 
AsyncWebServer server(80);
// http://localIP/dashboard

int relayPin = LED_BUILTIN;
 
void setup(){

  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW);
  
  Serial.begin(115200);
 
  SPIFFS.begin();
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando o WiFi...");
  }
 
  Serial.println(WiFi.localIP());
 
  server.on("/dashboard", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/dashboard.html", "text/html");
  });
 
  server.on("/dashboard.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/dashboard.css", "text/css");
  });

  server.on("/favicon.ico", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/favicon.png", "image/png");
  });

server.on("/relay/off", HTTP_PATCH, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", "ok");
    digitalWrite(relayPin, LOW);
  });
  
  server.on("/relay/on", HTTP_PATCH, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain","ok");
    digitalWrite(relayPin, HIGH);
  });
  
  server.on("/relay/toggle", HTTP_PATCH, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain","ok");
    digitalWrite(relayPin, !digitalRead(relayPin));
  });
  
  server.on("/relay", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", String(digitalRead(relayPin)));
  });
 
  server.begin();
}
 
void loop(){}
