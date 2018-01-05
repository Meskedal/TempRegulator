#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

const char* ssid = "Liljeveien1";
const char* password = "hoppeslott";

ESP8266WebServer server(80);

#define gpio_2 2
String inputString = "";
char inChar;

void handleRoot() {
  temp_receive();
  //Serial.println("After: " + inputString); Debugg print
  server.send(200, "text/plain", inputString);
  inputString = "";
}

void temp_receive(){
  digitalWrite(gpio_2,HIGH); //Interrupts Atmega8
  delayMicroseconds(400); //Empirical found delay for transmission
  while (Serial.available() > 0){
    inChar = Serial.read();
    inputString+= inChar;
  }
  digitalWrite(gpio_2,LOW);
}
void handleNotFound(){
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void setup(void){
  delay(50); 
  pinMode(gpio_2, OUTPUT); //Setup interrupt 
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);

  server.on("/inline", [](){
    server.send(200, "text/plain", "this works as well");
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void){
  delay(100);
  server.handleClient();
}
