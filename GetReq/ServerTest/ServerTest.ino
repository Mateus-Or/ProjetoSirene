
/* Create a WiFi access point and provide a web server on it. */



#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

/* Set these to your desired credentials. */
const char* ssid = "TOIZIM DA MORTE PRIME";
const char* password = "984910671MAFS";

// Configurações de IP estático
IPAddress novoIP(192, 168, 4, 1);
IPAddress gateway(192, 168, 100, 1);
IPAddress subnet(255, 255, 0, 0);
IPAddress dns(8, 8, 8, 8);

ESP8266WebServer server(80);

/* Just a little test message.  Go to http://192.168.4.1 in a web browser
   connected to this access point to see it.
*/
void handleRoot() {
  server.send(200, "text/html", "<h1>You are connected</h1>");
  Serial.println("Alguém acessou!");
}

void ligar() {
  digitalWrite(D0,HIGH);
  server.send(200, "ligado");  
}

void desligar() {
  digitalWrite(D0,LOW);
  server.send(200, "desligado");  
}

void setup() {
  delay(1000);
  pinMode(D0, OUTPUT);
  Serial.begin(115200);

  if (!WiFi.config(novoIP, gateway, subnet, dns)) {
    Serial.println("Configuração de IP estático falhou!");
  }


  // Conecta-se à rede Wi-Fi
  Serial.println();
  Serial.print("Conectando-se à rede ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("Conectado à rede Wi-Fi");
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP());

 
  server.on("/", handleRoot);
  server.on("/ligar", ligar);
  server.on("/desligar", desligar);
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}