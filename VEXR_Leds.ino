#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>
#include "WebApp.h"
#include <string>
#include "AnimationType.h"
#include "Strips.h"

using namespace std;

#define APSSID "VESC_LEDS (IP=4.4.4.4)"  // The SSID of the wifi access point

IPAddress local_IP(4, 4, 4, 4);
IPAddress gateway(4, 4, 4, 4);
IPAddress subnet(255, 255, 255, 0);
ESP8266WebServer server(80);
ESP8266HTTPUpdateServer httpUpdater;
Strips strips;  // TODO : Use individual objects
WebApp webApp;
bool POWERED_ON = true;  // TODO : move this to Strip
AnimationType animation = AnimationType::LINEAR;

/**
  Display the webapp  root
**/
void displayIndexPage() {
  server.send(200, "text/html", webApp.getHtml());  // TODO : Use LittleFS instead of a class : https://byfeel.info/esp8266-systeme-de-fichier-littlefs/
}

/**
  Starts the http server
**/
void startServer() {
  server.on("/", displayIndexPage);

  /******
   POWER 
  ******/
  server.on("/power", HTTP_GET, []() {
    server.send(200, "text/plain", POWERED_ON ? "1" : "0");
  });
  server.on("/power/on", HTTP_POST, []() {
    strips.powerOn(true);
    server.send(200);
  });
  server.on("/power/off", HTTP_POST, []() {
    strips.powerOn(false);
    server.send(200);
  });

  /**********
   BRIGHTNESS 
  ***********/
  server.on("/brightness/front", HTTP_GET, []() {
    String brightnessFront(strips.brightnessFront);
    server.send(200, "text/plain", brightnessFront);
  });
  server.on("/brightness/front", HTTP_POST, []() {
    String level = server.arg("plain");
    strips.brightnessFront = level.toInt();
    server.send(200);
  });
  server.on("/brightness/back", HTTP_GET, []() {
    String brightnessBack(strips.brightnessBack);
    server.send(200, "text/plain", brightnessBack);
  });
  server.on("/brightness/back", HTTP_POST, []() {
    String level = server.arg("plain");
    strips.brightnessBack = level.toInt();
    server.send(200);
  });

  /*********
   ANIMATION 
  **********/
  server.on("/animation", HTTP_GET, []() {
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
  server.on("/animation/linear", HTTP_POST, []() {
    animation = AnimationType::LINEAR;
    server.send(200);
  });
  server.on("/animation/accordion", HTTP_POST, []() {
    animation = AnimationType::ACCORDION;
    server.send(200);
  });
  server.on("/animation/fixed", HTTP_POST, []() {
    animation = AnimationType::FIXED;
    server.send(200);
  });


  httpUpdater.setup(&server);
  server.begin();
}

/******
 SETUP
******/
void setup() {
  Serial.begin(9600);

  WiFi.softAPConfig(local_IP, gateway, subnet);
  WiFi.softAP(APSSID);

  IPAddress myIP = WiFi.softAPIP();

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

  if (strips.isPoweredOn()) {
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
