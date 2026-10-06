// Smart Home Automation - ESP32 + Blynk (simulated in Wokwi)
// V0 = Living Room Light, V1 = Bedroom Light (switches in the Blynk dashboard)
// V2 = Temperature, V3 = Humidity (sent to the dashboard every 5 seconds)
//
// IMPORTANT: replace YOUR_AUTH_TOKEN with the auth token of your own Blynk device.
// Never commit a real token to a public repository.

#define BLYNK_TEMPLATE_ID "TMPL3XUruXB8H"
#define BLYNK_TEMPLATE_NAME "Smart. Home"
#define BLYNK_AUTH_TOKEN "YOUR_AUTH_TOKEN"
#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <DHTesp.h>

// Wokwi's built-in WiFi network (no password)
char ssid[] = "Wokwi-GUEST";
char pass[] = "";

const int LIVING_ROOM_LED = 26;
const int BEDROOM_LED = 27;
const int DHT_PIN = 15;

DHTesp dht;
BlynkTimer timer;

// Called when the Living Room switch in the dashboard changes
BLYNK_WRITE(V0) {
  int state = param.asInt();
  digitalWrite(LIVING_ROOM_LED, state);
  Serial.print("Living room light: ");
  Serial.println(state ? "ON" : "OFF");
}

// Called when the Bedroom switch in the dashboard changes
BLYNK_WRITE(V1) {
  int state = param.asInt();
  digitalWrite(BEDROOM_LED, state);
  Serial.print("Bedroom light: ");
  Serial.println(state ? "ON" : "OFF");
}

// When connected, ask the dashboard for the current switch positions
BLYNK_CONNECTED() {
  Blynk.syncVirtual(V0, V1);
}

// Reads the DHT22 and sends the values to the dashboard
void sendSensorData() {
  TempAndHumidity data = dht.getTempAndHumidity();
  if (dht.getStatus() != DHTesp::ERROR_NONE) {
    Serial.println("DHT22 read failed");
    return;
  }
  Blynk.virtualWrite(V2, data.temperature);
  Blynk.virtualWrite(V3, data.humidity);
  Serial.print("Temperature: ");
  Serial.print(data.temperature);
  Serial.print(" C, Humidity: ");
  Serial.print(data.humidity);
  Serial.println(" %");
}

void setup() {
  Serial.begin(115200);
  pinMode(LIVING_ROOM_LED, OUTPUT);
  pinMode(BEDROOM_LED, OUTPUT);
  dht.setup(DHT_PIN, DHTesp::DHT22);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  timer.setInterval(5000L, sendSensorData);
}

void loop() {
  Blynk.run();
  timer.run();
}
