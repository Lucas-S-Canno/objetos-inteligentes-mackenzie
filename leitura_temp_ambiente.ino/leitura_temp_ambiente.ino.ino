#include <dht.h>

#define dht_dpin A1

dht DHT;

void setup()
{
  Serial.begin(9600);
  delay(1000);
}

void loop()
{
  DHT.read11(dht_dpin);
  Serial.print("Umidade = ");
  Serial.print(DHT.humidity);
  Serial.print(" %  ");
  Serial.print("Temperatura = ");
  Serial.print(DHT.temperature); 
  Serial.println(" Celsius  ");

  delay(2000);  
}