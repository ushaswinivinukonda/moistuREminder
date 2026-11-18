#include <WiFi.h>
#include "FirebaseESP32.h"

// Replace with your own Firebase credentials
#define FIREBASE_HOST "your-project-id-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "YOUR_FIREBASE_DATABASE_SECRET"

// Replace with your Wi-Fi credentials
#define WIFI_SSID "YOUR_WIFI_SSID"
#define WIFI_PASSWORD "YOUR_WIFI_PASSWORD"

FirebaseData firebaseData;

int val = 0;          // Motor status: 1 (Excess Water/Off), -1 (Dry/On), 0 (Optimal/Off)
float smvalue = 0.0;  // Calibrated soil moisture reading
long duration;
int distance;

// GPIO Pin Definitions
const int trigPin = 25;
const int echoPin = 26;
const int relayPin = 13;
const int moisturePin = 34;

// Firebase Realtime Database Paths
String tagMotor = "IOTLAB/smart_garden/Value";
String tagMoisture = "IOTLAB/smart_garden/Sensor_Value";

void setup() {
  Serial.begin(115200);
  pinMode(relayPin, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  WiFiInit();
  FirebaseInit();
}

void loop() {
  // Measure water reservoir level via Ultrasonic Sensor
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;

  // Read and invert analog soil moisture value (ESP32 12-bit ADC: 0-4095)
  smvalue = 4095 - analogRead(moisturePin);

  Serial.print("Reservoir Distance (cm): ");
  Serial.println(distance);
  Serial.print("Soil Moisture Value: ");
  Serial.println(smvalue);

  // Safety & Irrigation Control Logic
  if (distance < 1) {
    digitalWrite(relayPin, HIGH); // Turn off motor if reservoir condition triggers cutoff
    Serial.println("Motor OFF (Reservoir check)");
  }

  if (smvalue > 300) {
    Serial.println("Status: Excess water -> Motor OFF");
    digitalWrite(relayPin, LOW);
    val = 1;
  } else if (smvalue < 10) {
    Serial.println("Status: Soil dry -> Motor ON");
    digitalWrite(relayPin, HIGH);
    val = -1;
  } else {
    Serial.println("Status: Optimal moisture -> Motor OFF");
    digitalWrite(relayPin, LOW);
    val = 0;
  }

  FirebaseWrite();
  delay(1000);
}

void WiFiInit() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
}

void FirebaseInit() {
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
}

void FirebaseWrite() {
  Firebase.setFloat(firebaseData, tagMotor, val);
  Firebase.setFloat(firebaseData, tagMoisture, smvalue);
}