#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>
#include "WebApp.h"
#include <string>
#include "Strip.h"

using namespace std;

#define APSSID "VESC_LEDS (IP=4.4.4.4)"  // The SSID of the wifi access point

IPAddress local_IP(4, 4, 4, 4);
IPAddress gateway(4, 4, 4, 4);
IPAddress subnet(255, 255, 255, 0);
ESP8266WebServer server(80);
ESP8266HTTPUpdateServer httpUpdater;
Strip frontStrip(D1, 1);
Strip backStrip(D2, 2);
WebApp webApp;
bool ledUp = false;

/**
  Display the webapp  root
**/
void displayIndexPage() {
  server.send(200, "text/html", webApp.getHtml());  // TODO : Use LittleFS instead of a class : https://byfeel.info/esp8266-systeme-de-fichier-littlefs/
}

/**
  Makes the string name of the animation 
  param animation : the types animation
  returns the animation type as a string
**/
string makeAnimationName(int animation) {
  switch (animation) {
    case AnimationType::LINEAR: return "linear";
    case AnimationType::ACCORDION: return "accordion";
    case AnimationType::FIXED: return "fixed";
  }
  return "linear";
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
    // TODO : multiple switches
    server.send(200, "text/plain", frontStrip.isPoweredOn() && backStrip.isPoweredOn() ? "1" : "0");
  });
  server.on("/power/on", HTTP_POST, []() {
    frontStrip.powerOn(true);
    backStrip.powerOn(true);
    Serial.println("Powered On");
    server.send(200);
  });
  server.on("/power/off", HTTP_POST, []() {
    frontStrip.powerOn(false);
    backStrip.powerOn(false);
    Serial.println("Powered Off");
    server.send(200);
  });

  /**********
   BRIGHTNESS 
  ***********/
  server.on("/brightness/front", HTTP_GET, []() {
    String brightnessFront(frontStrip.getBrightness());
    server.send(200, "text/plain", brightnessFront);
  });
  server.on("/brightness/front", HTTP_POST, []() {
    int level = server.arg("plain").toInt();
    frontStrip.setBrightness(level);
    server.send(200);
    Serial.println("Set front brightness to " + String(level));
  });
  server.on("/brightness/back", HTTP_GET, []() {
    String brightnessBack(backStrip.getBrightness());
    server.send(200, "text/plain", brightnessBack);
  });
  server.on("/brightness/back", HTTP_POST, []() {
    int level = server.arg("plain").toInt();
    backStrip.setBrightness(level);
    server.send(200);
    Serial.println("Set back brightness to " + String(level));
  });

  /*********
   ANIMATION 
  **********/
  server.on("/animation", HTTP_GET, []() {
    // TODO : separate back and front animations
    AnimationType animation = frontStrip.getAnimation();
    const string animationName = makeAnimationName(animation);
    server.send(200, "text/plain", animationName.data());
  });
  server.on("/animation/linear", HTTP_POST, []() {
    // TODO : separate back and front animations
    frontStrip.setAnimation(AnimationType::LINEAR);
    backStrip.setAnimation(AnimationType::LINEAR);
    Serial.println("Set animation to linear");
    server.send(200);
  });
  server.on("/animation/accordion", HTTP_POST, []() {
    // TODO : separate back and front animations
    frontStrip.setAnimation(AnimationType::ACCORDION);
    backStrip.setAnimation(AnimationType::ACCORDION);
    Serial.println("Set animation to accordion");
    server.send(200);
  });
  server.on("/animation/fixed", HTTP_POST, []() {
    // TODO : separate back and front animations
    frontStrip.setAnimation(AnimationType::FIXED);
    backStrip.setAnimation(AnimationType::FIXED);
    Serial.println("Set animation to fixed");
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

  frontStrip.setColor(255, 255, 255);
  backStrip.setColor(255, 0, 0);
}

/**
  Main loop
**/
void loop() {
  server.handleClient();
  frontStrip.animate();
  backStrip.animate();
  delay(5);
}
