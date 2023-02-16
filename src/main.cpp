#include <Arduino.h>
#include <WiFi.h>

#define WIFI_SSID "your SSID"
#define WIFI_PASSWORD "your password"

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  pinMode(LED_BUILTIN, OUTPUT);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.println("Connecting to WiFi");
}

bool isWiFiConnected = false;
void wifi_status(){
  //turn built-in LED on if WiFi is connected else blink built-in LED
  if (WiFi.status() == WL_CONNECTED && !isWiFiConnected) {
    digitalWrite(LED_BUILTIN, HIGH);
    //write to serial monitor if WiFi is connected
    Serial.println("WiFi connected");
    isWiFiConnected = true;
  } 
  
  if(WiFi.status() != WL_CONNECTED) {
    isWiFiConnected = false;
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    delay(1000);
    
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  wifi_status();
  
}