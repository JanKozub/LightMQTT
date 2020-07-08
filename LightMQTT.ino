#include "ESP8266WiFi.h"
#include "PubSubClient.h"
#include "Config.h"

WiFiClient espClient;
PubSubClient client(espClient);

long lastRefresh = 0;
long lastReconnect = 0;

boolean state = false;

void setup() {
  pinMode(relay, OUTPUT);
  pinMode(btn, INPUT_PULLUP);
  digitalWrite(relay, state);

  Serial.begin(115200);

  setup_wifi();

  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}
void loop() {
  long now = millis();

  if (!client.connected()) {
    if (now - lastRefresh > 10000) {
      lastRefresh = now;
      reconnect();
      ESP.wdtFeed();
    }
  } else {
    client.loop();

    if (now - lastRefresh > 5000) {
      lastRefresh = now;
      publishStatus();
      ESP.wdtFeed();
    }
  }

  if (digitalRead(btn) == 0) {
    state = changeState(state);
    digitalWrite(relay, state);
    delay(200);
  }
}
