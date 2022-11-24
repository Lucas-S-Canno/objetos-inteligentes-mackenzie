#include <SoftwareSerial.h>
//sensor temp corporal
#include <OneWire.h>
#include <DallasTemperature.h>
//sensor de temp e umi ambiente
#include <dht.h>
//sensor de BPM
#define USE_ARDUINO_INTERRUPTS true
#include <PulseSensorPlayground.h>
//MQTT
#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>
#include <ArduinoJson.h>

//sensor temp corporal
#define ONE_WIRE_BUS 2
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

//sensor temp e umi ambiente
#define dht_dpin A1
dht DHT;

//sensor de BPM
const int PulseWire = 0;
const int LED13 = 13;
int Threshold = 550;
PulseSensorPlayground pulseSensor;

//esp8266 serial
#define DEBUG true
SoftwareSerial esp8266(3, 4);

// JSON
const char ALIAS1[] = "Temp";
const char ALIAS2[] = "Umi";
const char ALIAS3[] = "Temp_corp";
const char ALIAS4[] = "bpm";

unsigned long espera;
unsigned long reconecta;
const long INTERVALO = 1200000; //20 minutos
const char* ID_DISPOSITIVO = "Canno";
const char* TOKEN_API = "77661629-ab2c-442e-a66d-e526f52a09f2";
const char* SENHA_ID = "04031996";

void setup() {
  Serial.begin(9600);
  sensors.begin();
  pulseSensor.analogInput(PulseWire);   
  pulseSensor.blinkOnPulse(LED13);
  pulseSensor.setThreshold(Threshold);   
  if (pulseSensor.begin()) {
    Serial.println("We created a pulseSensor Object !");
  }

  esp8266.begin(19200);
  sendData("AT+RST\r\n", 2000, DEBUG);
  sendData("AT+CWJAP=\"Canno\",\"04031996\"\r\n", 2000, DEBUG);
  delay(3000);
  sendData("AT+CWMODE=1\r\n", 1000, DEBUG);
  sendData("AT+CIFSR\r\n", 1000, DEBUG);
  sendData("AT+CIPMUX=1\r\n", 1000, DEBUG);
  sendData("AT+CIPSERVER=1,80\r\n", 1000, DEBUG);
  Serial.print("FIM");
  delay(2000);
}

void loop() {
  sensors.requestTemperatures();

  float tempC = sensors.getTempCByIndex(0);
  float tempF = sensors.getTempFByIndex(0);

  DHT.read11(dht_dpin);
  Serial.print("Umidade = ");
  Serial.print(DHT.humidity);
  Serial.print(" %  ");
  Serial.print("Temperatura Ambiente= ");
  Serial.print(DHT.temperature); 
  Serial.println(" Celsius  ");

  Serial.print("Temperatura Corporal: ");
  Serial.print(tempC);
  Serial.print("C  |  ");
  Serial.print(tempF);
  Serial.println("F");

  int myBPM = pulseSensor.getBeatsPerMinute();
  if (pulseSensor.sawStartOfBeat()) {
    Serial.print("BPM: ");
    Serial.println(myBPM);
  }
  Serial.print("------------------------------------------------------\n");

  // MQTT
   MQTT.loop();
   if(!MQTT.connected()){
    if(reconecta < millis()){
      Serial.println("");
      Serial.println("Conectando ao servidor...");
      if(!MQTT.connect(ID_DISPOSITIVO, TOKEN_API, SENHA_ID)){ 
        Serial.println("Falha na conexao com o servidor.");
      } else {
        Serial.println("Conectado!");
        Serial.print("Enviando publicacoes para ");
        Serial.println(TOPICO);  
      }
      reconecta = millis() + 5000;
      
    }
  }else{
    DynamicJsonDocument json(JSON_OBJECT_SIZE(2));
          
    json[ALIAS1] = "Temp";
    json[ALIAS2] = "Umi";
    json[ALIAS3] = "Temp_corp";
    json[ALIAS4] = "bpm";

    size_t tamanho_mensagem = measureJson(json) + 1;

    char mensagem[tamanho_mensagem];

    serializeJson(json, mensagem, tamanho_mensagem);
    
    Serial.println("");
    Serial.print("Mensagem enviada: ");
    Serial.println(mensagem);
    MQTT.publish(TOPICO, mensagem);
  }

  delay(2000); 
}

String sendData(String command, const int timeout, boolean debug) {
  String response = "";
  esp8266.print(command);
  long int time = millis();
  while ( (time + timeout) > millis()) {
    while (esp8266.available()) {
      char c = esp8266.read();
      response += c;
    }
  }
  if (debug) {
    Serial.print(response);
  }
  return response;
}