
#include <ESP8266WiFi.h>          //ESP8266 Core WiFi Library (you most likely already have this in your sketch)

#include <DNSServer.h>            //Local DNS Server used for redirecting all requests to the configuration portal
#include <ESP8266WebServer.h>     //Local WebServer used to serve the configuration portal
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager WiFi Configuration Magic
#include <ESP8266mDNS.h>          // Useful to access to ESP by hostname.local


const char *wifi_config_name = "VT Sensor Node";

// Settings to collect
char passcode[40] = "";
char host_name[40] = "";
char port_str[20] = "80";

char mqtt_server[40];
char mqtt_port[6] = "8080";

int port = 80;

bool shouldSaveConfig = false;    // Flag for saving data

void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);
  Serial.println();
  Serial.println("ValidateThis Sensor Node v1");
  PrintDashes();
  
  // Handle all the setup for the device
  wifi_station_set_hostname(host_name);
  WiFi.hostname(host_name);
  MDNS.begin(host_name);
  
  HandleSetup(wifi_config_name, true);
  PrintDashes();
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("Local IP: " + ipToString(WiFi.localIP()));
  Serial.println("Named URL to send commands: http://" + String(host_name) + ":" + port_str);
  Serial.println("IP URL to send commands: http://" + ipToString(WiFi.localIP()) + ":" + port_str);

  // Configure mDNS
  if (MDNS.begin(host_name)) {
    Serial.println("mDNS started. Hostname is set to " + String(host_name) + ".local");
    MDNS.addService("http", "tcp", port); // Announce the ESP as an HTTP service  
  }
  
  PrintDashes();
}


//+=============================================================================
// IP Address to String
//
String ipToString(IPAddress ip)
{
  String s="";
  for (int i=0; i<4; i++)
    s += i  ? "." + String(ip[i]) : String(ip[i]);
  return s;
}


int i = 0;

void loop() {
  // put your main code here, to run repeatedly:
  if (i <= 10) {
    i++;
    Serial.println("Loop: " + String(i));
  }
}

void PrintDashes(){
  Serial.println("-----------------------------");
}

