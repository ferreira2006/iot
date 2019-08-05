#include <ESP8266WiFi.h>
#include <FS.h>
#include <ESPAsyncWebServer.h>
 
const char* ssid = "ESP32";
const char* password =  "12345678";
 
AsyncWebServer server(80);
// http://localIP/dashboard
 
void setup(){
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
 
  server.begin();
}
 
void loop(){}
