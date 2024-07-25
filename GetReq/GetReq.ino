#include <ESP8266WiFi.h>
#include <ArduinoHttpClient.h>

// Configurações da rede Wi-Fi
const char* ssid = "TOIZIM DA MORTE PRIME";
const char* password = "984910671MAFS";

// Endereço IP do servidor e porta
const char* serverAddress = "192.168.4.1";
const int serverPort = 80;

// Configurações de IP estático
IPAddress novoIP(192, 168, 4, 2);
IPAddress gateway(192,168,100,1);
IPAddress subnet(255, 255, 0, 0);
IPAddress dns(8, 8, 8, 8);

WiFiClient wifi;
HttpClient client = HttpClient(wifi, serverAddress, serverPort);

void setup() {
  Serial.begin(115200);
  delay(10);

  // Configura IP estático
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

  // Faz a solicitação HTTP GET para /ligar
  fazerRequisicaoHTTP("/ligar");

  // Espera um pouco antes de fazer a próxima requisição
  delay(5000);

  // Faz a solicitação HTTP GET para /desligar
  fazerRequisicaoHTTP("/desligar");
}

void loop() {
  // Não é necessário fazer nada no loop neste exemplo
}

void fazerRequisicaoHTTP(String rota) {
  // Envia a requisição GET para o servidor
  Serial.print("Enviando requisição GET para ");
  Serial.println(rota);

  client.get(rota);

  // Recebe a resposta do servidor
  int statusCode = client.responseStatusCode();
  String response = client.responseBody();

  Serial.print("Código de status: ");
  Serial.println(statusCode);
  Serial.print("Resposta: ");
  Serial.println(response);

  client.stop();
}