#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>
#include "Strips.h"
#include "WebApp.h"

#define APSSID "VESC_LEDS (IP=4.4.4.4)"  // The SSID of the wifi access point

IPAddress local_IP(4, 4, 4, 4);
IPAddress gateway(4, 4, 4, 4);
IPAddress subnet(255, 255, 255, 0);
ESP8266WebServer server(80);
ESP8266HTTPUpdateServer httpUpdater;
Strips strips;
WebApp webApp;
bool POWERED_ON = true;

typedef enum {
  LINEAR,
  ACCORDION,
  FIXED
} AnimationType;

AnimationType animation = AnimationType::LINEAR;

/**
  Display the webapp  root
**/
void displayIndexPage() {
  server.send(200, "text/html", webApp.getHtml());
}

/**
  Starts the http server
**/
void startServer() {
  server.on("/", displayIndexPage);
  server.on("/on", []() {
    POWERED_ON = true;
    displayIndexPage();
  });
  server.on("/off", []() {
    POWERED_ON = false;
    displayIndexPage();
  });
  server.on("/animate/linear", []() {
    animation = AnimationType::LINEAR;
    displayIndexPage();
  });
  server.on("/animate/accordion", []() {
    animation = AnimationType::ACCORDION;
    displayIndexPage();
  });
  server.on("/animate/fixed", []() {
    animation = AnimationType::FIXED;
    displayIndexPage();
  });

  httpUpdater.setup(&server);
  server.begin();
}

/**
  App setup
**/
void setup() {
  WiFi.softAPConfig(local_IP, gateway, subnet);
  WiFi.softAP(APSSID);

  IPAddress myIP = WiFi.softAPIP();
  Serial.begin(9600);
  Serial.print("AP IP address: ");
  Serial.println(myIP);

  startServer();

  Serial.println("HTTP server started");
}

/**
  Main loop
**/
void loop() {
  server.handleClient();

  if (POWERED_ON) {
    switch (animation) {
      case AnimationType::LINEAR:
        strips.animateLinear();
        break;
      case AnimationType::ACCORDION:
        strips.animateAccordion();
        break;
      case AnimationType::FIXED:
        strips.fixed();
        break;
    }
  }
}
