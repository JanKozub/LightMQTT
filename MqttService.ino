void setup_wifi() {
  delay(10);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("WiFi connected");
}

void reconnect() {
  Serial.print("Attempting MQTT connection...");
  String clientId = "Light";

  clientId += String(random(0xffff), HEX);
  if (client.connect(clientId.c_str())) {
    Serial.println("connected");

    client.publish("light", "light,1");

    client.subscribe("lightSub");
  } else {
    Serial.print("failed, rc=");
    Serial.print(client.state());
    Serial.println(" try again in 10 seconds");
  }
}

void callback(char* topic, byte* payload, int length) {
  String output = "";
  for (int i = 0; i < length; i++) {
    output = output + (char)payload[i];
  }
  if (output.equals("TOGGLE")) {
    state = !state;
    publishStatus();
    digitalWrite(relay, state);
    Serial.println("------------------");
    Serial.print("Toggled via app -> ");
    Serial.print("State= ");
    Serial.println(state);
    Serial.println("------------------");
  }
}

void publishStatus() {
  String msg = "light," + String(state);
  Serial.print("Publish message: ");
  Serial.println(msg);
  client.publish("light", msg.c_str());
}

boolean changeState(boolean state) {
  long clickTime = millis();
  do {
    if (millis() - clickTime > 10000) {
      Serial.println("Restarting....");
      ESP.restart();
    }
    ESP.wdtFeed();
    delay(10);
  } while (digitalRead(btn) == 0);

  Serial.println("------------------");
  Serial.println("Toggled via switch -> ");
  Serial.print("State= ");
  Serial.println(state);
  Serial.println("------------------");

  return !state;
}
