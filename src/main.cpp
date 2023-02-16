#include <Arduino.h>
#include <WiFi.h>
#include <secrets.h>
#include <PubSubClient.h>

// define the pins used by a rgb led
#define RED_PIN 23
#define GREEN_PIN 22
#define BLUE_PIN 21


// mqtt client variables
WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

void turn_led_red() {
  digitalWrite(RED_PIN, HIGH);
  digitalWrite(GREEN_PIN, LOW);
  digitalWrite(BLUE_PIN, LOW);
  Serial.println("LED is red");
}

void turn_led_green() {
  digitalWrite(RED_PIN, LOW);
  digitalWrite(GREEN_PIN, HIGH);
  digitalWrite(BLUE_PIN, LOW);
  Serial.println("LED is green");
}

void turn_led_blue() {
  digitalWrite(RED_PIN, LOW);
  digitalWrite(GREEN_PIN, LOW);
  digitalWrite(BLUE_PIN, HIGH);
  Serial.println("LED is blue");
}

void turn_led_off() {
  digitalWrite(RED_PIN, LOW);
  digitalWrite(GREEN_PIN, LOW);
  digitalWrite(BLUE_PIN, LOW);
  Serial.println("LED is off");
}

char* ledColor;
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;

  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    messageTemp += (char)payload[i];
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if (String(topic) == MQTT_SUB_TOPIC_2) {
    Serial.print("Changing output to ");
    if (messageTemp == "Red") {
      turn_led_red();
      ledColor = "Red";
    } else if (messageTemp == "Green") {
      turn_led_green();
      ledColor = "Green";
    } else if (messageTemp == "Blue") {
      turn_led_blue();
      ledColor = "Blue";
    } else {
      Serial.println("off");
      turn_led_off();
      ledColor = "none";
    }
  }
}

void setup_wifi(){
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.println("Connecting to WiFi");
}

void setup_mqtt() {
  client.setServer(MQTT_SERVER, 1883);
  client.setCallback(callback);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  pinMode(LED_BUILTIN, OUTPUT);

  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);

  setup_wifi();
  setup_mqtt();
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



void reconnect_mqtt() {
  // Loop until we're reconnected
  while (!client.connected()) {
    // Create a random client ID
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    //if (client.connect(clientId.c_str(), MQTT_USER, MQTT_PASSWORD)) {
     if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish(MQTT_PUB_TOPIC_1, "hello world");
      // ... and resubscribe
      client.subscribe(MQTT_SUB_TOPIC_2);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  wifi_status();
  if (!client.connected()) {
    reconnect_mqtt();
  }

   client.loop();

   client.publish(MQTT_PUB_TOPIC_2, ledColor);
   delay(5000);

}