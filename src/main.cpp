#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#include <OneWire.h>
#include <DallasTemperature.h>
   
// Set WiFi credentials
#define WIFI_SSID "Deco15"
#define WIFI_PASS "greenballoon560"
#define UDP_PORT 515



// UDP
WiFiUDP UDP;
char packet[255];
char reply[] = "Packet received!";
char outputChar[128];
IPAddress iplox(192, 168, 1, 12);

// Set your Gateway IP address
IPAddress local_IP(192, 168, 1, 100);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 0, 0);

const int pinCidlaDS = 2;

// vytvoření instance oneWireDS z knihovny OneWire
OneWire oneWireDS(pinCidlaDS);
// vytvoření instance senzoryDS z knihovny DallasTemperature
DallasTemperature senzoryDS(&oneWireDS);
   
void setup() {

  delay(100);

  //pinMode(0, OUTPUT);    // sets the digital pin 13 as output
  //digitalWrite(0, HIGH);
  // Setup serial port
  Serial.begin(9600);
  Serial.println("Serial begin");

  senzoryDS.begin();

  // Configures static IP address
  if (!WiFi.config(local_IP, gateway, subnet)) {
    Serial.println("STA Failed to configure");
  }

  // Begin WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASS);
   
  // Connecting to WiFi...
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);
  // Loop continuously while WiFi is not connected
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(100);
    Serial.print(".");
  }
   
  // Connected to WiFi
  Serial.println();
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());

/*
  // Begin listening to UDP port
  UDP.begin(UDP_PORT);
  Serial.print("Listening on UDP port ");
  Serial.println(UDP_PORT);
*/
   
}


void myFunction(String ID, float value) {
  String outputStr = "UDP_" + ID + value;
  //Serial.print("outputStr: "); 
  Serial.println(outputStr); // This gives me what I expect
  outputStr.toCharArray(outputChar, outputStr.length()+1);
  //Serial.print("outputChar: "); 
  Serial.println(outputChar); // This yields the same as above, as expected
}

void loop() {
  senzoryDS.requestTemperatures();
  Serial.print("Teplota cidla DS18B20: ");
  int teplota = senzoryDS.getTempCByIndex(0);
  Serial.print(senzoryDS.getTempCByIndex(0));
  Serial.println("°C");
  /* 
  // If packet received...
  int packetSize = UDP.parsePacket();
  if (packetSize) {
  
    
    Serial.print("Received packet! Size: ");
    Serial.println(packetSize);
    int len = UDP.read(packet, 255);
    if (len > 0)
    {
      packet[len] = '\0';
    }
    Serial.print("Packet received: ");
    Serial.println(packet);
    */

  String ID = "ESP1_Temp1=";
  float value = senzoryDS.getTempCByIndex(0);
  myFunction(ID, value);

    // Send return packet
    //UDP.beginPacket(UDP.remoteIP(), UDP.remotePort());
  UDP.beginPacket(iplox, 515);
  UDP.write(outputChar);
  UDP.endPacket();

    //delay(10000);
    ESP.deepSleep(10e6, RF_DEFAULT);
  //}
 
}
