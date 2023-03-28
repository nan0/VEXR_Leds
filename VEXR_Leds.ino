#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>
#include "Strips.h"
#include "WebApp.h"
#include <string>

using namespace std;

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
    Serial.println("turned on");
    server.send(200);
  });
  server.on("/off", []() {
    POWERED_ON = false;
    server.send(200);
  });
  server.on("/brightness/front", []() {
    int level = server.arg("level").toInt();
    strips.brightnessFront = level;
    server.send(200);
  });
  server.on("/brightness/back", []() {
    int level = server.arg("level").toInt();
    strips.brightnessBack = level;
    server.send(200);
  });
  server.on("/animate/accordion", []() {
    animation = AnimationType::ACCORDION;
    server.send(200);
  });
  server.on("/animate/fixed", []() {
    animation = AnimationType::FIXED;
    server.send(200);
  });
  server.on("/status/power", []() {
    server.send(200, "text/plain", POWERED_ON ? "1" : "0");
  });
  server.on("/status/brightness/front", []() {
    String brightnessFront(strips.brightnessFront);
    server.send(200, "text/plain", brightnessFront);
  });
  server.on("/status/brightness/back", []() {
    String brightnessBack(strips.brightnessBack);
    server.send(200, "text/plain", brightnessBack);
  });
  server.on("/status/animation", []() {
    String animationName;
    switch (animation) {
      case AnimationType::LINEAR:
        animationName = "linear";
        break;
      case AnimationType::ACCORDION:
        animationName = "accordion";
        break;
      case AnimationType::FIXED:
        animationName = "fixed";
        break;
    }
    server.send(200, "text/plain", animationName);
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

  delay(10);
}
