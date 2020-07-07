#include <WiFiClient.h>
#include <WiFiServer.h>
#include <WiFiUdp.h>

#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

#define WIFI_AP "Helia"
#define WIFI_PASSWORD "heliahelia"
#define LED1 D0
int mat_data = 0;


void setup() 
{

    // byte attention = 0;
    pinMode(LED1, OUTPUT);
    
    Serial.begin(9600);                            //Serial connection
    WiFi.begin(WIFI_AP, WIFI_PASSWORD);   //WiFi connection
    while (WiFi.status() != WL_CONNECTED) 
    {  
        delay(500);
        Serial.println("Waiting for connection");
    
    }

}
int i = 0;
void loop() {

  if (WiFi.status() == WL_CONNECTED) 
  { //Check WiFi connection status
      while (i < 15) {
        DynamicJsonBuffer JSONbuffer;
        JsonObject& JSONencoder = JSONbuffer.createObject(); 
    
        JSONencoder["sensorName"] = "myAtten1";
        JSONencoder["sensorType"] = "Attention";
    
        JsonArray& values = JSONencoder.createNestedArray("values"); //JSON array
        JsonArray& timestamps = JSONencoder.createNestedArray("timestamps"); //JSON array
    
        // byte attention;
        if (Serial.available() > 0)
        { 
          mat_data = Serial.parseInt();
           //Serial.println(mat_data);
          if(mat_data > 0x31)
            digitalWrite(LED1 , LOW);
          else
            digitalWrite(LED1 , HIGH);
        }
        
        values.add(mat_data); //Add value to array
        timestamps.add(String(millis()+0)); //Add value to array
    
        values.add(random(50));//Add value to array
        timestamps.add(String(millis()+1)); //Add value to array
    
        values.add(random(70));//Add value to array
        timestamps.add(String(millis()+2)); //Add value to array
    
    
        char JSONmessageBuffer[300];
        JSONencoder.prettyPrintTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
        Serial.println(JSONmessageBuffer);
    
        HTTPClient http;    //Declare object of class HTTPClient
    
        http.begin("http://192.168.43.162:5050/postjson");      //Specify request destination
        http.addHeader("Content-Type", "application/json");  //Specify content-type header
    
        int httpCode = http.POST(JSONmessageBuffer);   //Send the request
        String payload = http.getString();                                        //Get the response payload
    
        Serial.println(httpCode);   //Print HTTP return code
        Serial.println(payload);    //Print request response payload
    
        http.end();  //Close connection
        i = i + 1;
      }
  } 
  else 
    Serial.println("Error in WiFi connection");

}
