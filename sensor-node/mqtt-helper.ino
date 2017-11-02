//
//// Motion
//homeassistant/sensor/vtsensor1/pir/config
//{
//  "name":"VTSensor1 Motion",
//  "device_class":"motion",
//  "expire_after":"1",
//  "state_topic":"homeassistant/sensor/vtsensor1/state",
//  "value_template":"{{ value_json.motion}}"
//}


//homeassistant/binary_sensor/vtsensor2pir/config
//{
//  "name":"VTSensor2 Motion" ,
//  "device_class":"motion",
//  "state_topic":"homeassistant/sensor/vtsensor1/state",
//  "payload_on":"motion detected",
//  "payload_off":"standby",
//  "value_template":"{{ value_json.motion}}"
//}
//
//homeassistant/sensor/vtsensor1/state
//{
//  "motion":"motion detected"
//}

void setupMQTT() {
  client.setServer(mqtt_server, atoi(mqtt_port));
  client.setCallback(callback);
  
  reconnect();
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(host_name, mqtt_username, mqtt_password)) {
      Serial.println("connected");
      client.publish("outTopic","hello world");
      client.subscribe("inTopic");  


      // Setup the Motion sensor in HA
      client.publish(String("homeassistant/sensor/" + String(host_name) + "/pir/config").c_str(),String("{\r\n  \"name\":\"" + String(host_name) + " motion test\" ,\r\n  \"device_class\":\"motion\",\r\n  \"expire_after\":\"1\",\r\n  \"state_topic\":\"homeassistant/sensor/" + String(host_name) + "/state\",\r\n  \"value_template\":\"{{ value_json.motion}}\"\r\n}").c_str());
     
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
//
void sendState() {
  StaticJsonBuffer<BUFFER_SIZE> jsonBuffer;

  JsonObject& root = jsonBuffer.createObject();

  root["state"] = (stateOn) ? on_cmd : off_cmd;
  JsonObject& color = root.createNestedObject("color");
  color["r"] = red;
  color["g"] = green;
  color["b"] = blue;


  root["brightness"] = brightness;
  root["humidity"] = (String)humValue;
  root["motion"] = (String)motionStatus;
  root["ldr"] = (String)LDR;
  root["temperature"] = (String)tempValue;
  root["heatIndex"] = (String)calculateHeatIndex(humValue, tempValue);


  char buffer[root.measureLength() + 1];
  root.printTo(buffer, sizeof(buffer));
  Serial.println("NEW TOPIC");
  Serial.println(light_state_topic);
  Serial.println(buffer);
  client.publish(String(light_state_topic).c_str(), buffer, true);
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is acive low on the ESP-01)
  } else {
    digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
  }

}
