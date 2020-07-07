//#include "DHT.h"
#include <ESP8266WiFi.h>
#include <ThingsBoard.h>
#include <ArduinoJson.h>

#define WIFI_AP "Helia"
#define WIFI_PASSWORD "heliahelia"

#define TOKEN "TMRWSzj2Jy1R2vOpolxk"

#define LED1 D0
int mat_data=0;
int mat_data_b=0;
// DHT
//#define DHTPIN 2
//#define DHTTYPE DHT22

char thingsboardServer[] = "192.168.43.20";

WiFiClient wifiClient;
//ThingsBoard tb(wifiClient);
// Initialize DHT sensor.
//DHT dht(DHTPIN, DHTTYPE);

ThingsBoard tb(wifiClient);

int status = WL_IDLE_STATUS;
unsigned long lastSend;



void setup()
{
  Serial.begin(9600);
  //dht.begin();
  delay(10);
  InitWiFi();
  pinMode(LED1, OUTPUT);
  lastSend = 0;
}



void loop()
{
  if ( !tb.connected() ) {
    reconnect();
  }

  //if ( millis() - lastSend > 1000) 
  //{ // Update and send only after 1 seconds
  if (Serial.available() > 0)
  { 
     mat_data = Serial.parseInt();
     mat_data_b = mat_data;
     //Serial.println(mat_data);
    if(mat_data > 0x31)
      digitalWrite(LED1 , LOW);
    else
      digitalWrite(LED1 , HIGH); 
    lastSend = millis();
  }
  //}
  tb.sendTelemetryFloat("attention", mat_data);
  tb.loop();
}


void read_from_serial()
{
  // Serial.println("Collecting attention level.");
  if (Serial.available() > 0)
  { 
     mat_data = Serial.parseInt();
     mat_data_b = mat_data;
     //Serial.println(mat_data);
    if(mat_data > 0x31)
      digitalWrite(LED1 , LOW);
    else
      digitalWrite(LED1 , HIGH); 
  
  
    //Serial.println("Sending data to ThingsBoard:");
    //Serial.print("attention: ");
    //Serial.print(mat_data_b);
    //Serial.print(" %\t");
    //tb.sendTelemetryFloat("attention", mat_data);
  }
    //Serial.read();
    //return;
}


void InitWiFi()
{
  Serial.println("Connecting to AP ...");
  // attempt to connect to WiFi network

  WiFi.begin(WIFI_AP, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to AP");
}



void reconnect() {
  // Loop until we're reconnected
  while (!tb.connected()) {
    status = WiFi.status();
    if ( status != WL_CONNECTED) 
    {
      WiFi.begin(WIFI_AP, WIFI_PASSWORD);
      while (WiFi.status() != WL_CONNECTED) 
      {
        delay(500);
        Serial.print(".");
      }
      Serial.println("Connected to AP");
    }
    Serial.print("Connecting to ThingsBoard node ...");
    if ( tb.connect(thingsboardServer, TOKEN) ) 
    {
      Serial.println( "[DONE]" );
    }
    else 
    {
      Serial.print( "[FAILED]" );
      Serial.println( " : retrying in 5 seconds]" );
      // Wait 5 seconds before retrying
      delay( 5000 );
    }
  }
}
