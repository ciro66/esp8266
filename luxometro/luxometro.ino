#include <WebSocketsServer.h>
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <Hash.h>

String content;
const char* ssid = "........";
const char* password = "..............";

int light = 0;
String lux = "";

ESP8266WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);


void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t lenght) {

        // send message to client
        while (WStype_CONNECTED) {
          light = analogRead(A0);
          lux = String(light);
          webSocket.sendTXT(num, lux);
          delay(1000);
        }
}

void setup(){
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("");
  //Aspetto che si instauri la connessione
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Indirizzo IP: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")){
    Serial.println("Partito il responder MDNS");
  }

  server.on("/", [](){
      content = "<html><head><script>var connection = new WebSocket('ws://'+location.hostname+':81/', ['arduino']);";
      content += "connection.onopen = function () {  connection.send('Connect ' + new Date()); };";
      content += "connection.onerror = function (error) {    console.log('WebSocket Error ', error);};";
      content += "connection.onmessage = function (e) { var luce = document.getElementById('light'); luce.innerHTML = e.data;};";
      content += "connection.onopen = function() { console.log('Server: ciao'); connection.send('Sono il client: Ciao');};";
      content += "</script></head><body>LUXOMETRO:<br/><br/>";
      content += "<div id = \"light\">&nbsp;</div>";
      content += "</body></html>";
    server.send(200, "text/html", content);
  });
  

  server.begin();
  Serial.println("Partito il server HTTP");

  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
  Serial.println("Partito il webSocket");
}

void loop(){
  webSocket.loop();  
  server.send(200, "text/plain", "/");
  server.handleClient();  
}
