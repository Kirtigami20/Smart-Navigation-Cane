#include <WiFi.h>
#include <HTTPClient.h>
#include <TinyGPSPlus.h>

// ---------- WiFi credentials ----------
const char* ssid = "wifi name ";
const char* password = "your password ";

// ---------- Local server URL ----------
const char* server = "yout server ";
 // change to your PC's IP address

// ---------- GPS setup ----------
#define RXD2 16
#define TXD2 17
#define GPS_BAUD 9600
HardwareSerial gpsSerial(2);
TinyGPSPlus gps;

// ---------- SOS Button ----------
#define SOS_BUTTON 12

// ---------- Ultrasonic setup ----------
#define TRIG_PIN 4
#define ECHO_PIN 2
#define WARNING_DISTANCE 30  // cm

// ---------- Vibration motor setup ----------
#define VIBRATION_PIN 27

// ---------- Timing ----------
unsigned long lastSend = 0;
const unsigned long updateInterval = 10000; // send every 10s
unsigned long lastDistanceCheck = 0;

void setup() {
  Serial.begin(115200);
  gpsSerial.begin(GPS_BAUD, SERIAL_8N1, RXD2, TXD2);

  pinMode(SOS_BUTTON, INPUT_PULLUP);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(VIBRATION_PIN, OUTPUT);
  digitalWrite(VIBRATION_PIN, LOW);

  // Connect Wi-Fi
  Serial.print("Connecting to WiFi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n✅ WiFi Connected!");
  Serial.print("ESP32 IP: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // GPS read
  while (gpsSerial.available() > 0) gps.encode(gpsSerial.read());

  // Ultrasonic check every 1 sec
  if (millis() - lastDistanceCheck > 1000) {
    lastDistanceCheck = millis();
    measureDistance();
  }

  // Send GPS data every few seconds
  if (gps.location.isUpdated() && millis() - lastSend > updateInterval) {
    lastSend = millis();
    sendToLocalServer(false);
  }

  // SOS
  if (digitalRead(SOS_BUTTON) == LOW) {
    Serial.println("🚨 SOS button pressed!");
    sendToLocalServer(true);
    delay(3000);
  }
}

void measureDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 30000);
  float distance = duration * 0.0343 / 2;

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  if (distance > 0 && distance <= WARNING_DISTANCE) {
    Serial.println("⚠️ Object detected! Vibrating...");
    vibrate(300);
  } else {
    digitalWrite(VIBRATION_PIN, LOW);
  }
}

void vibrate(int durationMs) {
  digitalWrite(VIBRATION_PIN, HIGH);
  delay(durationMs);
  digitalWrite(VIBRATION_PIN, LOW);
}

void sendToLocalServer(bool alert) {
  float lat = gps.location.isValid() ? gps.location.lat() : 0.0;
  float lon = gps.location.isValid() ? gps.location.lng() : 0.0;
  String message = alert ? "ALERT_SOS_PRESSED" : "SAFE_UPDATE";

  Serial.println("---- GPS DATA ----");
  Serial.print("Latitude: "); Serial.println(lat, 6);
  Serial.print("Longitude: "); Serial.println(lon, 6);
  Serial.print("Message: "); Serial.println(message);

  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = String(server) + "?lat=" + String(lat, 6) +
                 "&lon=" + String(lon, 6) +
                 "&msg=" + message;

    http.begin(url);
    int code = http.GET();
    Serial.print("HTTP Response Code: ");
    Serial.println(code);
    http.end();
  } else {
    Serial.println("⚠️ Wi-Fi not connected!");
  }
}
