#include <FS.h>                   //this needs to be first, or it all crashes and burns...
#include <ESP8266WiFi.h>          //ESP8266 Core WiFi Library (you most likely already have this in your sketch)

#include <DNSServer.h>            //Local DNS Server used for redirecting all requests to the configuration portal
#include <ESP8266WebServer.h>     //Local WebServer used to serve the configuration portal
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager WiFi Configuration Magic
#include <ESP8266mDNS.h>          // Useful to access to ESP by hostname.local
#include <ArduinoOTA.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>
#include <DHT.h>

#ifdef USE_MQTT
  #define MQTT_MAX_PACKET_SIZE 512
  #include <PubSubClient.h>                 // MQTT
#endif  // USE_MQTT

bool shouldDebug = true;
/**************************** PIN DEFINITIONS ********************************************/
const int redPin = D1;
const int greenPin = D2;
const int bluePin = D3;
#define PIRPIN    D5
#define DHTPIN    D7
#define DHTTYPE   DHT22
#define LDRPIN    A0




// Settings to collect
char host_name[40] = "";
char passcode[40] = "";
char port_str[6] = "8456";

char mqtt_server[40] = "broker.mqtt-dashboard.com";
char mqtt_username[40] = "";
char mqtt_password[40] = "";
char mqtt_port[6] = "1883";




const char *wifi_config_name = "VT Sensor Node";
bool shouldSaveConfig = false;    // Flag for saving data
int port = 80;

ESP8266WebServer server(port);
WiFiClient espClient;
PubSubClient client(espClient);
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);
  Serial.println();
  Serial.println("ValidateThis Sensor Node v1");
  PrintDashes();
  
  HandleSetup(wifi_config_name, false);
  PrintDashes();

  // Handle all the setup for the device
  wifi_station_set_hostname(host_name);
  WiFi.hostname(host_name);
  MDNS.begin(host_name);
  
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
  if (shouldDebug)
    debugValues();

  
  
  ArduinoOTA.setHostname(host_name);
  ArduinoOTA.setPassword(passcode);
  ArduinoOTA.onStart([]() {
    Serial.println("Starting");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.onEnd([]() {
    ESP.restart();
  });
  ArduinoOTA.begin();
  setupWebServer();

  setupMQTT();
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

void debugValues(){
  Serial.println("DEBUG VALUES");
  PrintDashes();
  Serial.println("hostname: " + String(host_name));
  Serial.println("passcode: " + String(passcode));
  Serial.println("port_str: " + String(port_str));
  
  Serial.println("mqtt_server: " + String(mqtt_server));
  Serial.println("mqtt_port: " + String(mqtt_port));
  Serial.println("mqtt_username: " + String(mqtt_username));
  Serial.println("mqtt_password: " + String(mqtt_password));
  PrintDashes();
}

void loop() {
  ArduinoOTA.handle();
  server.handleClient();
  if (!client.connected()) {
    reconnect();
    //software_Reset();
  }
  client.loop();
  
  
  // put your main code here, to run repeatedly:
  if (i < 10) {
    i++;
    Serial.println("Loop: " + String(i));
  }
}

void PrintDashes(){
  Serial.println("-----------------------------");
}

