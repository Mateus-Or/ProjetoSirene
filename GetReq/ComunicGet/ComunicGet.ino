#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <RTClib.h>
#include <ArduinoHttpClient.h>

// Configurações da rede Wi-Fi
const char* ssid = "TOIZIM DA MORTE PRIME";
const char* password = "984910671MAFS";

// Endereço do servidor do notebook
const char* jsonServerUrlDias = "http://192.168.100.13:5000/diasdasemana";
const char* jsonServerUrlHorarios = "http://192.168.100.13:5000/alarms";

// Endereço IP do servidor e porta do outro ESP8266
const char* serverAddress = "192.168.4.1"; // IP do outro ESP8266
const int serverPort = 80;

// Configurações de IP estático
IPAddress local_IP(192, 168, 100, 2);  // Substitua pelo IP desejado
IPAddress gateway(192, 168, 100, 1);     // Substitua pelo IP do seu roteador
IPAddress subnet(255, 255, 0, 0);    // Substitua pela máscara de sub-rede

WiFiClient wifi;
HttpClient client = HttpClient(wifi, serverAddress, serverPort);
RTC_DS1307 rtc;

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
  //Wire.begin(D1, D2);

  // Inicializa o RTC
  if (!rtc.begin()) {
    Serial.println("Não foi possível encontrar o RTC");
    while (1);
  }
  else {
    Serial.println("RTC found");
  }

  if (!rtc.isrunning()) {
    Serial.println("RTC is NOT running, let's set the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  // Configura IP estático
  if (!WiFi.config(local_IP, gateway, subnet)) {
    Serial.println("Falha ao configurar IP estático");
  }

  // Conecta-se à rede Wi-Fi
  Serial.print("\nConectando-se à rede ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("Conectado ao WiFi!");
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP());
  
  // Obtém e analisa o JSON dos dias da semana
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
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

void loop() {
  DateTime now = rtc.now();
  String diaSemana = obterDiaSemana(now.dayOfTheWeek());

  if ((now.hour()) % 2 == 0 && (now.minute()) == 0) {
    atualizarJson();
  }

  for (int i = 0; i < 7; i++) {
    if (alarmesDias[i].ativo && alarmesDias[i].id == now.dayOfTheWeek()) {
      for (auto& alarme : alarmesHorarios) {
        if (alarme.ativo) {
          String currentTime = String(now.hour()) + ":" + (now.minute() < 10 ? "0" : "") + String(now.minute());

          if (currentTime == alarme.time) {
            Serial.println("Ativando alarme para " + diaSemana + " às " + alarme.time);
            fazerRequisicaoHTTP("/ligar");
            delay(5000);
            fazerRequisicaoHTTP("/desligar");
            delay(55000);
          }
        }
      }
    }
  }
   DateTime agora = rtc.now();

  Serial.print(agora.year(), DEC);
  Serial.print('/');
  Serial.print(agora.month(), DEC);
  Serial.print('/');
  Serial.print(agora.day(), DEC);
  Serial.print(" ");
  Serial.print(agora.hour(), DEC);
  Serial.print(':');
  Serial.print(agora.minute(), DEC);
  Serial.print(':');
  Serial.print(agora.second(), DEC);
  Serial.println();

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
  StaticJsonDocument<2048> doc;
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
  StaticJsonDocument<2048> doc;
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

void fazerRequisicaoHTTP(String rota) {
  Serial.print("Enviando requisição GET para ");
  Serial.println(rota);

  client.get(rota);

  int statusCode = client.responseStatusCode();
  String response = client.responseBody();

  Serial.print("Código de status: ");
  Serial.println(statusCode);
  Serial.print("Resposta: ");
  Serial.println(response);

  client.stop();
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
