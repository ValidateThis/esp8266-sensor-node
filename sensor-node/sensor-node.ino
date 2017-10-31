
#include <ESP8266WiFi.h>          //ESP8266 Core WiFi Library (you most likely already have this in your sketch)

#include <DNSServer.h>            //Local DNS Server used for redirecting all requests to the configuration portal
#include <ESP8266WebServer.h>     //Local WebServer used to serve the configuration portal
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager WiFi Configuration Magic

bool shouldSaveConfig = false;    // Flag for saving data

void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);
  Serial.println();

  HandleWifi("VT Sensor Node", false);

  
  //if you get here you have connected to the WiFi
  Serial.println("connected...yeey :)");
  
}

//+=============================================================================
// Gets called when WiFiManager enters configuration mode
//
void configModeCallback (WiFiManager *myWiFiManager) {
  Serial.println("Entered config mode");
  Serial.println(WiFi.softAPIP());
  //if you used auto generated SSID, print it
  Serial.println(myWiFiManager->getConfigPortalSSID());
}

//+=============================================================================
// Callback notifying us of the need to save config
//
void saveConfigCallback () {
  Serial.println("Should save config");
  shouldSaveConfig = true;
}

void HandleWifi(char* APName, bool resetConf) {
  //WiFiManager
  //Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wifiManager;
  
  //reset saved settings
  if (resetConf)
    wifiManager.resetSettings();

  // set callback that gets called when connecting to previous WiFi fails, and enters Access Point mode
  wifiManager.setAPCallback(configModeCallback);
  // set config save notify callback
  wifiManager.setSaveConfigCallback(saveConfigCallback);

  // CONFIG STUFF

  // Config how this should sit on the network
//  WiFiManagerParameter custom_hostname("hostname", "Choose a hostname to this IRBlaster", host_name, 40);
//  wifiManager.addParameter(&custom_hostname);
//  WiFiManagerParameter custom_passcode("passcode", "Choose a passcode", passcode, 40);
//  wifiManager.addParameter(&custom_passcode);
//  WiFiManagerParameter custom_port("port_str", "Choose a port", port_str, 40);
//  wifiManager.addParameter(&custom_port);

  wifiManager.setCustomHeadElement("<style>@import url(https://fonts.googleapis.com/css?family=Open+Sans);@import url(https://fonts.googleapis.com/icon?family=Material+Icons);.c,body>div{padding:0}*{box-sizing:border-box}body,html{width:100%;height:100%}body{font-family:'Open Sans',sans-serif;margin:0;display:flex;justify-content:center;align-items:center;background:radial-gradient(circle at 0 0,#0b8793,#360033);text-align:initial}body>div{width:350px;max-width:350px;min-height:392px;max-height:100%;background:#fff;color:#212121;border-radius:2px;box-shadow:0 3px 8px rgba(0,0,0,.2);overflow-Y:auto;animation:fadeInUp .5s}body>div>br{display:none}.c>a,body>div>form>button{background:0 0;padding:18px 32px;cursor:pointer;display:block;font-size:1.2rem;text-align:left;color:#212121;border:none;width:100%;line-height:2.4rem}.c>a:hover,body>div>div:hover,body>div>form>button:hover{background:#ddd}h1,h3{font-weight:400;margin:0;text-align:center}h1{padding:15px 0}h3{border-bottom:2px solid rgba(0,0,0,.05);padding:0 0 15px;font-size:.8em;margin-top:-20px}dl{margin:32px}dt{font-weight:700}dd{margin:0 0 10px 32px}.c>a:before,form>button:before,form[action='/wifi'] button:before{font-family:'Material Icons';content:'wifi';font-size:28px;vertical-align:middle;margin-right:20px}form[action='/r']>button:before{content:'refresh'}form[action='/i']>button:before{content:'info_outline'}form[action='/0wifi']>button:before{content:'edit'}.c>a,form[action=wifisave],form[action=wifisave]>button{font-size:0}form[action=wifisave]>button:before{content:'done'}form[action=wifisave]>button:after{content:'Connect';font-size:1.2rem;vertical-align:middle}form[action=wifisave] input{font-size:1.1rem;width:calc(100% - 60px);margin:10px 30px;outline:0;border:none;border-bottom:1px solid rgba(0,0,0,.3)}form[action=wifisave] input:focus{border-bottom:1px solid #383c89}form[action=wifisave] input:before{content:'Test';border:1px solid;display:block}.c>a:before{content:'refresh'}.c>a:after{content:'Rescan';font-size:1.2rem;vertical-align:middle}body>div>div{position:relative;padding:0;font-size:0}body>div>div>a{display:block;padding:10px 32px;font-size:1rem;color:#212121;text-decoration:none;position:relative;z-index:1}.q{position:absolute;top:9px;right:32px;font-size:.9rem;background:0 0;width:auto}.q:after{font-family:'Material Icons';content:'lock_open';font-size:1.6rem;vertical-align:middle;margin-left:5px}.l:after{content:'lock_outline'}@keyframes fadeInUp{0%{opacity:0;transform:translate3d(0,20%,0)}to{opacity:1;transform:none}}</style>");
  WiFiManagerParameter custom_text("<p>This is just a text paragraph</p>");
  wifiManager.addParameter(&custom_text);
  WiFiManagerParameter custom_mqtt_server("server", "mqtt server", "iot.eclipse", 40, " readonly");

  
  //fetches ssid and pass from eeprom and tries to connect
  //if it does not connect it starts an access point with the specified name
  //here  "AutoConnectAP"
  //and goes into a blocking loop awaiting configuration
  wifiManager.autoConnect(APName);
  //or use this for auto generated name ESP + ChipID
  //wifiManager.autoConnect();
}


int i = 0;

void loop() {
  // put your main code here, to run repeatedly:
  i++;

  Serial.println("Loop: " + String(i));
}
