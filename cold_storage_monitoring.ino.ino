#include <WiFi.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "DHT.h"
#include <ThingSpeak.h>
#include "Pushover.h"

// ----------- WiFi -----------
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

// ----------- ThingSpeak -----------
WiFiClient client;
unsigned long channelID = YOUR_CHANNEL_ID;
const char* writeAPIKey = "YOUR_THINGSPEAK_API_KEY";

// ----------- Pushover -----------
const char* pushoverToken = "YOUR_PUSHOVER_TOKEN";
const char* pushoverUserKey = "YOUR_PUSHOVER_USER_KEY";

// ----------- DHT11 -----------
#define DHTPIN 4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// ----------- MQ135 -----------
#define MQ135_PIN 34

// ----------- Outputs -----------
#define BUZZER 25
#define RELAY 26

// ----------- LCD -----------
LiquidCrystal_I2C lcd(0x27, 16, 2);

// ----------- Thresholds -----------
int gasThreshold = 1500;
float tempThreshold = 37.0;
float humidityLow = 40.0;

// ----------- Alert Timing -----------
unsigned long lastAlertTime = 0;
int alertInterval = 60000; // 1 min

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting WiFi...");
  }
  Serial.println("WiFi Connected");

  ThingSpeak.begin(client);
  dht.begin();

  pinMode(BUZZER, OUTPUT);
  pinMode(RELAY, OUTPUT);

  lcd.init();
  lcd.backlight();
  lcd.print("System Init...");
  delay(2000);
}

// ================= PUSHOVER =================
void sendPushoverAlert(String message) {
  Pushover po(pushoverToken, pushoverUserKey, UNSAFE);
  po.setMessage(message);
  po.send();
}

void loop() {

  float temp = dht.readTemperature();
  float hum  = dht.readHumidity();
  if (isnan(temp) || isnan(hum)) return;

  // MQ135 averaging
  int sum = 0;
  for (int i = 0; i < 10; i++) {
    sum += analogRead(MQ135_PIN);
    delay(20);
  }
  int gasValue = sum / 10;

  // Conditions
  bool gasAlert = gasValue > gasThreshold;
  bool tempHigh = temp > tempThreshold;
  bool humLow   = hum < humidityLow;

  // UPDATED RELAY LOGIC
  bool relayStatus = (tempHigh || gasAlert);
  digitalWrite(RELAY, relayStatus);

  // Buzzer
  bool buzzerStatus = (gasAlert || tempHigh || humLow);
  digitalWrite(BUZZER, buzzerStatus);

  // LCD
  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print(temp);
  lcd.print(" H:");
  lcd.print(hum);
  lcd.print("   ");

  lcd.setCursor(0, 1);
  lcd.print("G:");
  lcd.print(gasValue);

  if (gasAlert) lcd.print(" GAS!");
  else if (tempHigh) lcd.print(" TEMP!");
  else if (humLow) lcd.print(" HUM!");
  else lcd.print(" OK   ");

  // ThingSpeak Fields
  ThingSpeak.setField(1, temp);
  ThingSpeak.setField(2, hum);
  ThingSpeak.setField(3, gasValue);
  ThingSpeak.setField(4, buzzerStatus);
  ThingSpeak.setField(5, relayStatus);
  ThingSpeak.setField(6, 1); // heartbeat

  ThingSpeak.writeFields(channelID, writeAPIKey);

  // Pushover Alerts
  if ((gasAlert || tempHigh || humLow) &&
      (millis() - lastAlertTime > alertInterval)) {

    String message = "COLD STORAGE ALERT \n\n";

    if (gasAlert) {
      message += "GAS LEVEL HIGH!\nValue: " + String(gasValue) + "\n\n";
    }

    if (tempHigh) {
      message += "TEMPERATURE HIGH!\nTemp: " + String(temp) + " C\n\n";
    }

    if (humLow) {
      message += "LOW HUMIDITY!\nHumidity: " + String(hum) + " %\n\n";
    }

    message += "Immediate action required!";

    sendPushoverAlert(message);
    lastAlertTime = millis();
  }

  
  // Serial Monitor
  Serial.println("------ DATA ------");
  Serial.print("Temp: "); Serial.println(temp);
  Serial.print("Hum : "); Serial.println(hum);
  Serial.print("Gas : "); Serial.println(gasValue);
  Serial.print("Relay: "); Serial.println(relayStatus);
  Serial.print("Buzzer: "); Serial.println(buzzerStatus);
  Serial.println("------------------");

  delay(5000);
}
