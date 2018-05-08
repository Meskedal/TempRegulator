#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>


const char* ssid = "Liljeveien1";
const char* password = "password";

ESP8266WebServer server(80);

#define gpio_2 2
String inputString = "";
char inChar;
int count = 0;

void handleRoot() {
  command_send('s', "");
  //Serial.println("After: " + inputString); Debugg print
  server.send(200, "text/plain", inputString);
  inputString = "";
}

/*
void temp_receive(){
  digitalWrite(gpio_2,HIGH); //Interrupts Atmega8
  delayMicroseconds(50);
  Serial.print("s");
  delayMicroseconds(400); //Empirical found delay for transmission
  while (Serial.available() > 0){
    inChar = Serial.read();
    inputString+= inChar;
  }
  digitalWrite(gpio_2,LOW);
}
*/

void command_send(char command, String data){
  digitalWrite(gpio_2,HIGH); //Interrupts Atmega8
  delayMicroseconds(200);
  Serial.print(command);
  if(command == 's'){
    delayMicroseconds(1000); //Empirical found delay for transmission
    while (Serial.available() > 0){
      inChar = Serial.read();
      inputString+= inChar;
    }
  }
  else if(command == 'u'){
    int index = 0;
    delayMicroseconds(200);
    Serial.print(data[index]);
    do{
        index++;
        delayMicroseconds(200);
        Serial.print(data[index]); 
    } while(data[index] != 'f');
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

void get_parameters(void){
  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
 
    HTTPClient http;  //Declare an object of class HTTPClient
 
    http.begin("http://192.168.1.196/temp-get/");  //Specify request destination
    int httpCode = http.GET();                                                                  //Send the request
 
    if (httpCode > 0) { //Check the returning code
 
      String payload = http.getString();   //Get the request response payload
      command_send('u', payload);                     //Print the response payload

    }
 
    http.end();   //Close connection
  }
}

void setup(void){
  delay(100); 
   
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
  pinMode(gpio_2, OUTPUT); //Setup interrupt
}

void loop(void){
  delay(100);
  server.handleClient();
  count++;
  if (count >= 254){
    get_parameters();
    count = 0;
  }
}
