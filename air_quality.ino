#include <PubSubClient.h>
#include <WiFiEspClient.h>
#include <WiFiEspServer.h>
#include <WiFiEsp.h>
#include <WiFiEspUdp.h>
#include <ArduinoJson.h>
#include <SoftwareSerial.h>
//#include <LiquidCrystal.h>
//LiquidCrystal lcd(13, 12, 6, 5, 4, 3); // LCD connections

#define ORG "tryx3h"
#define DEVICE_TYPE "Arduino"
#define DEVICE_ID "air"
#define TOKEN "+lB96MbP8CkUq2Hauf"
#define WIFI_AP "AK"
#define WIFI_PASSWORD "asdfghjkl"
         
WiFiEspClient espClient;

float t=0;
char data = 0;
SoftwareSerial soft(10, 11); // 10-tx,11-rx
int status = WL_IDLE_STATUS;

char server[] = ORG ".messaging.internetofthings.ibmcloud.com";
char authMethod[] = "use-token-auth";
char token[] = TOKEN;
char clientId[] = "d:" ORG ":" DEVICE_TYPE ":" DEVICE_ID;

const char publishTopic[] = "iot-2/evt/status/fmt/json";
const char responseTopic[] = "iotdm-1/response";
const char manageTopic[] = "iotdevice-1/mgmt/manage";
const char updateTopic[] = "iotdm-1/device/update";
const char rebootTopic[] = "iotdm-1/mgmt/initiate/device/reboot";

void callback(char* publishTopic, char* payload, unsigned int payloadLength);

PubSubClient client(server, 1883, callback, espClient);

int publishInterval = 30000; // 30 seconds
long lastPublishMillis;
int period = 5000;
unsigned long time_now = 0;           
void setup() {
        Serial.begin(115200);
        InitWiFi();
        Serial.print(WiFi.localIP());
        if (!!!client.connected()) {
                Serial.print("Reconnecting client to ");
                Serial.println(server);
                while (!!!client.connect(clientId, authMethod, token)) {
                Serial.print(".");
        }

 //       lcd.begin(16, 2);  // to intialize LCD


//  lcd.setCursor(0,0); 

 // lcd.print("    Welcome");

 // lcd.setCursor(0,1);

 // lcd.print("       To        "); 

  delay(3000);


 // lcd.clear();

  //lcd.setCursor(0,0);

  //lcd.print("     AIR"); 

  //lcd.setCursor(0,1);

  //lcd.print("QUALITY MONITOR");  

  delay(1000); 
        Serial.println();
        }

}


void loop() {
                 
           t = analogRead(A0);  // Read sensor value and stores in a variable t

  Serial.print("Airquality = ");

  Serial.println(t);

//  lcd.clear();
//  lcd.setCursor (0, 0);
 // lcd.print ("Air Qual: ");
 // lcd.print (t);
 // lcd.print (" PPM ");
  
//  lcd.setCursor (0,1);
  if (t<=500)
   {
 //  lcd.print("Fresh Air");
   Serial.print("Fresh Air ");
   
}
  else if( t>=500 && t<=1000 )
{
   //lcd.print("Poor Air");
   Serial.print("Poor Air");
  
}
  else if (t>=1000 )
{
  //lcd.print("Very Poor");
  Serial.print("Very Poor");
  
}
  //lcd.scrollDisplayLeft();
  delay(5000);

 // lcd.clear();

 // lcd.setCursor(0,0);

 // lcd.print("  SENDING DATA");

 // lcd.setCursor(0,1);

 // lcd.print("    TO CLOUD");
          String payload = "{\"d\":{ \"t\" :";
          payload += t;
          payload += "} }";
          Serial.print("Sending payload: ");
          Serial.println(payload);

          while (!!!client.publish(publishTopic, (char *)payload.c_str())) {
                  Serial.println("Publish ok");
                  if (!!!client.connected()) {
                        Serial.print("Reconnecting client to ");
                        Serial.println(server);
                        while (!!!client.connect(clientId, authMethod, token)) {
                              Serial.print(".");
                              delay(1000);
                        }
                        Serial.println();
                  }
          
          } 
            time_now = millis();           
            Serial.println("5 sec Delay");           
            while(millis() < time_now + period){
                //wait approx. [period] ms
            }  
}

void InitWiFi()
{
      // initialize serial for ESP module
      soft.begin(112500);
      // initialize ESP module
      WiFi.init(&soft);
      
      Serial.println("Connecting to AP …");
      // attempt to connect to WiFi network
      while ( status != WL_CONNECTED) {
            Serial.print("Attempting to connect to WPA SSID: ");
            Serial.println(WIFI_AP);
            // Connect to WPA/WPA2 network
            status = WiFi.begin(WIFI_AP, WIFI_PASSWORD);
            delay(1000);
      }
      Serial.println("Connected to AP");
}

void callback(char* publishTopic, char* payload, unsigned int length) {
      Serial.println("callback invoked");
}
