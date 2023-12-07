#include <ESP8266WiFi.h>
#include "ThingSpeak.h"
#include "MQ7.h"

MQ7 mq7(A0,5.0);

char ssid[] = "Phone";
char pass[] = "hereyougo2021";
WiFiClient  client;

float dust = 0;
float co = 0;
float co_read = 0;
float calcVoltage = 0;
float dustDensity = 0;
int Pin_D1 = 4;
int Pin_D2 = 5;

void setup() {
  Serial.begin(9600);
  pinMode(Pin_D1, OUTPUT);
  pinMode(Pin_D2, OUTPUT);
  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client);
  if(WiFi.status() != WL_CONNECTED){
    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin(ssid, pass);
      Serial.print(".");
      delay(5000);
    }
    Serial.print("Connected.");
  }
}

void dustSensor() {
  digitalWrite(Pin_D2,  LOW);
  delay(1000);
  digitalWrite(Pin_D1, HIGH);
  delay(10000);
}

void coSensor() {
  digitalWrite(Pin_D1,  LOW);
  delay(1000);
  digitalWrite(Pin_D2, HIGH);
  delay(10000);
}

void loop() {

  dustSensor();
  dust = analogRead(0);
  calcVoltage = dust * (3 / 1024.0);
  dustDensity = 0.17 * calcVoltage - 0.1;
  Serial.print("Dust Density = ");
  Serial.println(dustDensity);
  Serial.println("");
  int x = ThingSpeak.writeField(2253626, 3, dust, "ZL1K7BIINL8Z2EY0");
  if(x == 200){
    Serial.println("Channel update successful.");
  }
  else{
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }

  coSensor();
  co_read = analogRead(0);
  co = mq7.getPPM();
  Serial.print("CO = ");
  Serial.println(co);
  Serial.println("");
  x = ThingSpeak.writeField(2303832, 1, co, "KRGQYEFXUJV96ITN");
  if(x == 200){
    Serial.println("Channel update successful.");
  }
  else{
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
}