#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <RTClib.h>
#include <ArduinoHttpClient.h>
#include <ESP8266WebServer.h>

// Configurações da rede Wi-Fi
const char* ssid = "IoT";
const char* password = "BJsKshUyePRUFMyMJBR9";

// Endereços do servidor para JSON
const char* jsonServerUrlDias = "http://10.84.5.33:5000/diasdasemana";
const char* jsonServerUrlHorarios = "http://10.84.5.33:5000/alarms";

// Configurações de IP estático[]
IPAddress local_IP(10, 84, 101, 15);
IPAddress gateway(10, 84, 101, 5);
IPAddress subnet(255, 255, 255, 0);
IPAddress dns(8, 8, 8, 8);

WiFiClient wifi;
HttpClient client = HttpClient(wifi, "10.84.5.33", 5000);
RTC_DS1307 rtc;
ESP8266WebServer server(80);

struct AlarmeDia {
  int id;
  String dia;
  bool ativo;
};

struct AlarmeHorario {
  String time;
  bool ativo;
  int id;
};

AlarmeDia alarmesDias[7];
std::vector<AlarmeHorario> alarmesHorarios;
 
void setup() {
  Serial.begin(115200);
  pinMode(D0, OUTPUT);

  // Inicializa o RTC
  if (!rtc.begin()) {
    Serial.println("Não foi possível encontrar o RTC");
    
    // Scanner I2C adicionado
    Wire.begin(D1, D2); // Use os mesmos pinos SDA e SCL
    Serial.println("Scanning I2C devices...");

    byte error, address;
    int nDevices = 0;

    for (address = 1; address < 127; address++) {
      Wire.beginTransmission(address);
      error = Wire.endTransmission();

      if (error == 0) {
        Serial.print("I2C device found at address 0x");
        if (address < 16) Serial.print("0");
        Serial.print(address, HEX);
        Serial.println("  !");
        nDevices++;
      } else if (error == 4) {
        Serial.print("Unknown error at address 0x");
        if (address < 16) Serial.print("0");
        Serial.println(address, HEX);
      }
    }

    if (nDevices == 0) {
      Serial.println("No I2C devices found\n");
    } else {
      Serial.println("done\n");
    }

    while (1); // Trava o código se não encontrar o RTC
  } else {
    Serial.println("RTC encontrado");
  }

  if (!rtc.isrunning()) {
    Serial.println("RTC não está em execução, ajustando a hora!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  // Configura IP estático
  if (!WiFi.config(local_IP, gateway, subnet, dns)) {
    Serial.println("Falha ao configurar IP estático");
  }

  // Conecta-se à rede Wi-Fi
  Serial.print("\nConectando-se à rede ");
  Serial.println(ssid);

  // Inicia a conexão
  WiFi.begin(ssid, password);  // Conecta ao SSID definido (rede WPA2 Enterprise)

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("\nConectado ao WiFi!");
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP());

  // Inicializa as rotas do servidor web
  server.on("/", handleRoot);
  server.on("/tocar", tocar);
  server.begin();
  Serial.println("Servidor HTTP iniciado");

  // Obtém e analisa os JSONs iniciais
  atualizarJson();
}

void loop() {
  server.handleClient(); // Processa solicitações HTTP
  DateTime now = rtc.now();
  String diaSemana = obterDiaSemana(now.dayOfTheWeek());

  if ((now.second()) % 30 == 0) {
    atualizarJson();
  }

  for (int i = 0; i < 7; i++) {
    if (alarmesDias[i].ativo && alarmesDias[i].id == now.dayOfTheWeek()) {
      for (auto& alarme : alarmesHorarios) {
        if (alarme.ativo) {
          String currentTime = String(now.hour()) + ":" + (now.minute() < 10 ? "0" : "") + String(now.minute());

          if (currentTime == alarme.time) {
            Serial.println("Ativando alarme para " + diaSemana + " às " + alarme.time);
            tocarDispositivo();
            delay(57000);
          }
        }
      }
    }
  }

  delay(1000);
}

void atualizarJson() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    // Obtém e analisa o JSON dos dias da semana
    http.begin(wifi, jsonServerUrlDias);
    int httpCode = http.GET();

    if (httpCode > 0) {
      if (httpCode == HTTP_CODE_OK) {
        String payload = http.getString();
        Serial.println(payload);
        parseJsonDias(payload);
      }
    } else {
      Serial.printf("Erro na requisição HTTP: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();

    // Obtém e analisa o JSON dos horários dos alarmes
    http.begin(wifi, jsonServerUrlHorarios);
    httpCode = http.GET();

    if (httpCode > 0) {
      if (httpCode == HTTP_CODE_OK) {
        String payload = http.getString();
        Serial.println(payload);
        parseJsonHorarios(payload);
      }
    } else {
      Serial.printf("Erro na requisição HTTP: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
  } else {
    Serial.println("Erro ao conectar ao WiFi");
  }
}

void parseJsonDias(String jsonString) {
  JsonDocument doc;
  DeserializationError error = deserializeJson(doc, jsonString);

  if (error) {
    Serial.print(F("Falha ao analisar JSON: "));
    Serial.println(error.c_str());
    return;
  }

  for (int i = 0; i < 7; i++) {
    alarmesDias[i].id = doc[i]["id"];
    alarmesDias[i].dia = doc[i]["dia"].as<String>();
    alarmesDias[i].ativo = doc[i]["ativo"];
  }
}

void parseJsonHorarios(String jsonString) {
  JsonDocument doc;
  DeserializationError error = deserializeJson(doc, jsonString);

  if (error) {
    Serial.print(F("Falha ao analisar JSON: "));
    Serial.println(error.c_str());
    return;
  }

  alarmesHorarios.clear();
  for (JsonObject item : doc.as<JsonArray>()) {
    AlarmeHorario alarme;
    alarme.time = item["time"].as<String>();
    alarme.ativo = item["ativo"];
    alarme.id = item["id"];
    alarmesHorarios.push_back(alarme);
  }
}

// Funções do servidor web
void handleRoot() {
  server.send(200, "text/html", "<h1>You are connected</h1>");
  Serial.println("Alguém acessou!");
}

void tocar() {
  digitalWrite(D0, HIGH);
  server.send(200, "ligado");
  delay(3000);
  digitalWrite(D0, LOW);
  server.send(200, "desligado");
}


// Funções auxiliares para ligar/desligar o dispositivo
void tocarDispositivo() {
  digitalWrite(D0, HIGH);
  server.send(200, "ligado");
  delay(3000);
  digitalWrite(D0, LOW);
  server.send(200, "desligado");
}

String obterDiaSemana(int dia) {
  switch (dia) {
    case 0: return "Domingo";
    case 1: return "Segunda-feira";
    case 2: return "Terça-feira";
    case 3: return "Quarta-feira";
    case 4: return "Quinta-feira";
    case 5: return "Sexta-feira";
    case 6: return "Sábado";
    default: return "";
  }
}
