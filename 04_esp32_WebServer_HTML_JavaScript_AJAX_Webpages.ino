#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

ESP8266WebServer server;
uint8_t pin_led = LED_BUILTIN;
char* ssid = "corporativo";
char* password = "legislativo";

char webpage[] PROGMEM = R"=====(
<html>
<head>
</head>
<body>
<p> Status: <span id="led-state">__</span> </p>
<button onclick="ligarRele()"> ON/OFF </button>
</body>
<script>
function ligarRele(){
  console.log("Botão acionado!");
  var xhr = new XMLHttpRequest();
  var url = "/ledstate";
  xhr.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("led-state").innerHTML = this.responseText;
    }
  };
  xhr.open("GET", url, true);
  xhr.send();
};
document.addEventListener('DOMContentLoaded', ligarRele, false);
</script>
</html>
)=====";

void setup(){
  pinMode(pin_led, OUTPUT);
  WiFi.begin(ssid,password);
  Serial.begin(115200);
  while(WiFi.status()!=WL_CONNECTED)  {
    Serial.print(".");
    delay(500);
  }
  
  Serial.println("");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.on("/",[](){server.send_P(200,"text/html", webpage);});
  server.on("/ledstate",getLEDState);
  server.begin();
}

void loop(){
  server.handleClient();
}

void toggleLED(){
  digitalWrite(pin_led,!digitalRead(pin_led));
}

void getLEDState(){
  toggleLED();
  String led_state = digitalRead(pin_led) ? "ON" : "OFF";
  server.send(200,"text/plain", led_state);
}
