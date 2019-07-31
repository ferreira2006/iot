#include <ESP8266WiFi.h>

const char* ssid     = "esp32";
const char* password = "12345678";

// Porta de acesso 192.168.4.1:80
WiFiServer server(80);

// Requisição HTTP
String header;

// Estado atual do pino 13
String output13State = "off";

// Para teste usaremos o led da placa
const int output13 = LED_BUILTIN;

void setup() {
  Serial.begin(115200);
  pinMode(output13, OUTPUT);
  digitalWrite(output13, LOW);
  Serial.print("Configurtando ESP no modo AP…");
  // Removee o parametro password se não quiser exigir a senha
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("IP de acesso: ");
  Serial.println(IP);
  
  server.begin();
}

void loop(){
  WiFiClient client = server.available();   // Aguardando clientes

  if (client) {                             // Se existir um cliente
    Serial.println("Novo Cliente.");         
    String currentLine = "";                
    while (client.connected()) {            // Enquanto o cliente estiver conectado
      if (client.available()) {             
        char c = client.read();             
        Serial.write(c);                    
        header += c;
        if (c == '\n') {                    
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Conexão fechada.");
            client.println();

            if (header.indexOf("GET /13/on") >= 0) {
              Serial.println("GPIO 13 on");
              output13State = "on";
              digitalWrite(output13, HIGH); // Liga pino
            } else if (header.indexOf("GET /13/off") >= 0) {
              Serial.println("GPIO 13 off");
              output13State = "off";
              digitalWrite(output13, LOW);  // Dsliga Pino
            }
            
            // Exibição da página HTML
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
           
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #555555;}</style></head>");
            client.println("<body><h1Controle de RELÉ</h1>");           
            client.println("<p>Relé01 - Status: " + output13State + "</p>");
            
            if (output13State=="off") {
              client.println("<p><a href=\"/13/on\"><button class=\"button\">LIGAR</button></a></p>");
            } else {
              client.println("<p><a href=\"/13/off\"><button class=\"button button2\">DESLIGAR</button></a></p>");
            }             
            client.println("</body></html>");
            client.println();            
            
            break; // Sai do loop
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  
          currentLine += c;     
        }
      }
    }
    
    header = "";    
    client.stop(); // Fecha conexão com o cliente atual
    Serial.println("Cliente desconectado.");
    Serial.println("");
  }
}
