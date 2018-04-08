/*
  Per fare upload via terminale usare: curl -F "image=@firmware.bin" IP_ESP8266/update
  Per fare upload via browser puntare a: IP_ESP8266/update
*/

#include <ESP8266HTTPUpdateServer.h>
#include <ESP8266WebServer.h>
#include <FS.h>

#define ssid "XXXXXXXXXX"
#define password "XXXXXXXXXXXX"

ESP8266WebServer server(80);
ESP8266HTTPUpdateServer httpUpdater;

void setup() {
  Serial.begin(115200);

  WiFi.begin (ssid, password);
  while ( WiFi.status() != WL_CONNECTED ) {
    delay (500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print ("Connected to ");
  Serial.println (ssid);
  Serial.print ("IP address: ");
  Serial.println (WiFi.localIP() );

  if (!SPIFFS.begin()) {
    Serial.println("SPIFFS Mount failed");
  } else {
    Serial.println("SPIFFS Mount succesfull");
  }

  server.serveStatic("/img", SPIFFS, "/img");
  server.serveStatic("/", SPIFFS, "/index.html");

  httpUpdater.setup(&server);
  server.begin();
  Serial.println("HTTP server started");

  
}

void loop() {
  server.handleClient();
  delay(100);
}

