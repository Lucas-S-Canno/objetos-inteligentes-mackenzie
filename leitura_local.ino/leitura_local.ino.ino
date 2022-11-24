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

bool bpm = true;

void setup() {
  Serial.begin(9600);
  sensors.begin();
  pulseSensor.analogInput(PulseWire);   
  pulseSensor.blinkOnPulse(LED13);
  pulseSensor.setThreshold(Threshold);   
  if (pulseSensor.begin()) {
    Serial.println("We created a pulseSensor Object !");
  }
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
  Serial.print(tempC+3.2);
  Serial.print("C  |  ");
  Serial.print(tempF+4);
  Serial.println("F");

  int myBPM = pulseSensor.getBeatsPerMinute();
  if (pulseSensor.sawStartOfBeat()) {
    Serial.print("BPM: ");
    Serial.println(myBPM);
  }
  Serial.print("------------------------------------------------------\n");


  delay(2000); 
}