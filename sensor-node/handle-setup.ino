

void HandleSetup(const char* APName, bool resetConf) {
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

  

  wifiManager.setCustomHeadElement("<style>.c,body>div{padding:0}*{box-sizing:border-box}body,html{width:100%;height:100%}body{font-family:'Open Sans',sans-serif;margin:0;display:flex;justify-content:center;align-items:center;background:radial-gradient(circle at 0 0,#0b8793,#360033);text-align:initial}body>div{width:350px;max-width:350px;min-height:392px;max-height:100%;background:#fff;color:#212121;border-radius:2px;box-shadow:0 3px 8px rgba(0,0,0,.2);overflow-Y:auto;animation:fadeInUp .5s}body>div>br{display:none}.c>a,body>div>form>button{background:0 0;padding:18px 32px;cursor:pointer;display:block;font-size:1.2rem;text-align:left;color:#212121;border:none;width:100%;line-height:2.4rem}.c>a:hover,body>div>form>button:hover{background:#ddd}h1,h3{font-weight:400;margin:0;text-align:center}h1{padding:15px 0}h3{border-bottom:2px solid rgba(0,0,0,.05);padding:0 0 15px;font-size:.8em;margin-top:-15px}dl{margin:32px}dt{font-weight:700}dd{margin:0 0 10px 32px}form[action='/wifi'] button:before{background-position:0 -204px;vertical-align:middle;margin-right:20px}.c>a:before,.l:after,.q:after,form>button:before{background-image:url(data:image/png;base64,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);background-repeat:no-repeat;display:inline-block;content:'';width:35px;height:35px;vertical-align:middle;margin-right:20px}.c>a:before,form[action='/r']>button:before{background-position:0 -102px}form[action='/i']>button:before{background-position:0 -35px}form[action='/0wifi']>button:before{background-position:0 0}.c>a,form[action=wifisave],form[action=wifisave]>button{font-size:0}form[action=wifisave]>button:before{background-position:0 -137px}form[action=wifisave]>button:after{content:'Connect';font-size:1.2rem;vertical-align:middle}form[action=wifisave] input{font-size:1.1rem;width:calc(100% - 60px);color:#3a3a3a;margin:10px 30px;outline:0;border:none;border-bottom:1px solid rgba(0,0,0,.3)}form[action=wifisave] input:focus{border-bottom:1px solid #383c89}form[action=wifisave] p{font-size:.8rem;margin:10px 32px}form[action=wifisave] h2,form[action=wifisave]:before{border-bottom:2px solid rgba(0,0,0,.5);padding:10px 32px;font-size:1rem}form[action=wifisave]:before{content:'WiFi Settings'}.c>a:after{content:'Scan WiFi';font-size:1.2rem;vertical-align:middle}body>div>div{position:relative;padding:0;font-size:0}body>div>div:first-child:nth-last-child(1){padding:32px;font-size:1rem;line-height:2}body>div>div:first-child:nth-last-child(1):hover{background:0 0}body>div>div>a{display:block;padding:10px 32px;font-size:1rem;color:#212121;text-decoration:none;position:relative;z-index:1}body>div>div:hover{background:#ddd}.q{position:absolute;top:6px;right:32px;font-size:.9rem;background:0 0;width:auto}.l:after,.q:after{width:32px;height:32px;margin:0;transform:scale(.8) translateY(-2px)}.q:after{background-position:0 -172px}.l:after{background-position:0 -70px}@keyframes fadeInUp{0%{opacity:0;transform:translate3d(0,20%,0)}to{opacity:1;transform:none}}</style>");
  WiFiManagerParameter custom_text("<h2>Sensor Settings</h2><p>Customise your sensor with the settings below</p>");
  wifiManager.addParameter(&custom_text);

    
  //fetches ssid and pass from eeprom and tries to connect
  //if it does not connect it starts an access point with the specified name
  //here  "AutoConnectAP"
  //and goes into a blocking loop awaiting configuration
  wifiManager.autoConnect(APName);
  //or use this for auto generated name ESP + ChipID
  //wifiManager.autoConnect();
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



