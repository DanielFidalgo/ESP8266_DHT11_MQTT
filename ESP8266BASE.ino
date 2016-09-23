#include <DHT.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h> 
//-------- Customise these values -----------
const char* ssid = "[YOUR_SSID]";
const char* password = "[YOUR_PASSWORD]";
#define ORG "[YOUR_ORG]"
#define DEVICE_TYPE "[YOUR_DEVICE_TYPE]"
#define DEVICE_ID "[YOUR_DEVICE_ID]"
#define TOKEN "[YOUR_DEVICE_TOKEN]"
#define DHTPIN 2
#define DHTTYPE DHT11
//-------- Customise the above values --------
char server[] = ORG ".messaging.internetofthings.ibmcloud.com";
char topic[] = "iot-2/evt/status/fmt/json";
char authMethod[] = "use-token-auth";
char token[] = TOKEN;
char clientId[] = "d:" ORG ":" DEVICE_TYPE ":" DEVICE_ID;
WiFiClient wifiClient;
PubSubClient client(server, 1883, NULL, wifiClient);
DHT dht(DHTPIN, DHTTYPE);
void setup() {
  Serial.begin(9600);
  dht.begin();
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}
void loop() {
  if (!!!client.connected()) {
    while (!!!client.connect(clientId, authMethod, token)) {
      delay(500);
    }
  }
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float hic = dht.computeHeatIndex(t, h, false);
  String payload = "{\"d\":{\"myName\":\"";
  payload += DEVICE_ID;
  payload += "\",\"temp\":\"";
  payload += t;
  payload += "\",\"hum\":\"";
  payload += h;
  payload += "\",\"hi\":\"";
  payload += hic;
  payload += "\"}}";
  client.publish(topic, (char*) payload.c_str());
  Serial.printf((char*) payload.c_str());
  delay(2000);
}
