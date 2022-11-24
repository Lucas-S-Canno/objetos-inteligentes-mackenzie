#include <SoftwareSerial.h>

SoftwareSerial esp8266(3, 4);
#define DEBUG true

void setup() {
  Serial.begin(9600);
  esp8266.begin(19200);
  sendData("AT+RST\r\n", 2000, DEBUG);
  sendData("AT+CWJAP=\"Canno\",\"04031996\"\r\n", 2000, DEBUG);
  delay(3000);
  sendData("AT+CWMODE=1\r\n", 1000, DEBUG);
  sendData("AT+CIFSR\r\n", 1000, DEBUG);
  sendData("AT+CIPMUX=1\r\n", 1000, DEBUG);
  sendData("AT+CIPSERVER=1,80\r\n", 1000, DEBUG);
}

void loop() {
  if (esp8266.available()) {
    if (esp8266.find("+IPD,")) {
      delay(300);
      int connectionId = esp8266.read() - 48;
      String webpage = "<head><meta http-equiv=""refresh"" content=""3"">";
      webpage += "</head><h1><u>ESP8266 - Web Server</u></h1><h2>Porta";
      webpage += "Digital 8: ";
      int a = digitalRead(8);
      webpage += a;
      webpage += "<h2>Porta Digital 9: ";
      int b = digitalRead(9);
      webpage += b;
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
