#include <SoftwareSerial.h>
//sensor temp corporal
#include <OneWire.h>
#include <DallasTemperature.h>
//sensor de temp e umi ambiente
#include <dht.h>
//sensor de BPM
#define USE_ARDUINO_INTERRUPTS true
#include <PulseSensorPlayground.h>  

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
SoftwareSerial esp8266(3, 4);
#define DEBUG true

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

  //criação da pagina com dados
  if (esp8266.available()) {
    if (esp8266.find("+IPD,")) {
      delay(300);
      int connectionId = esp8266.read() - 48;
      String webpage = "<head><meta http-equiv=""refresh"" content=""3"">";
      webpage += "</head>";
      webpage += "<h1>Web Server</h1>";
      webpage += "<h2>Umidade: ";
      webpage += DHT.humidity;
      webpage += "%";
      webpage += "</h2>";
      webpage += "<h2>Temperatura do ambiente: ";
      webpage += DHT.temperature;
      webpage += "C";
      webpage += "</h2>";
      webpage += "<h2>Temperatura corporal: ";
      webpage += tempC;
      webpage += "C | ";
      webpage += tempF;
      webpage += "F";
      webpage += "</h2>";
      webpage += "<h2>BPM: ";
      webpage += myBPM;
      webpage += "</h2>";
      String cipSend = "AT+CIPSEND=";
      cipSend += connectionId;
      cipSend += ",";
      cipSend += webpage.length();
      cipSend += "\r\n";
      sendData(cipSend, 1000, DEBUG);
      sendData(webpage, 1000, DEBUG);
      String closeCommand = "AT+CIPCLOSE=";
      closeCommand += connectionId; // append connection id
      closeCommand += "\r\n";
      sendData(closeCommand, 3000, DEBUG);
    }
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