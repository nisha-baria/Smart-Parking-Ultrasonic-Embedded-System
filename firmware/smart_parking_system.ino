#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Display Configuration
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Network Credentials
const char* WIFI_SSID = "YOUR_WIFI_SSID";
const char* WIFI_PASS = "YOUR_WIFI_PASSWORD";
WebServer server(80);

// Actuators & Pins
const int SERVO_PIN = 18;
const int SERVO_CH  = 3;
const int SERVO_FREQ = 50;
const int SERVO_RES  = 10;
const int BUZZER_PIN = 19;

struct SensorPair { int trig; int echo; };
SensorPair sensors[4] = {
  {5, 17}, {16, 4}, {27, 26}, {25, 33}
};

struct LedPair { int green; int red; };
LedPair leds[4] = {
  {12, 2}, {14, 13}, {32, 23}, {15, 0} // GPIO 0 used for slot 4 red LED
};

struct SlotState {
  bool occupied;
  bool pending;
  unsigned long lastChange;
  float lastAvg;
};

SlotState slots[4];
const float THRESHOLD_CM[4] = {35.0, 35.0, 35.0, 35.0};
const int READS = 5;
const unsigned long DEBOUNCE_MS = 800;

int servoAngleToDuty(int angle) {
  float duty_ms = 0.5 + (angle / 180.0) * 2.0;
  return (int)((duty_ms / 20.0) * ((1 << SERVO_RES) - 1));
}

long readPulseCM(int trig, int echo) {
  digitalWrite(trig, LOW); delayMicroseconds(3);
  digitalWrite(trig, HIGH); delayMicroseconds(10);
  digitalWrite(trig, LOW);
  long us = pulseIn(echo, HIGH, 30000);
  if (us == 0) return 9999;
  return (long)(us / 58.0);
}

float averageDistance(int idx) {
  float vals[READS];
  for (int i = 0; i < READS; i++) {
    vals[i] = readPulseCM(sensors[idx].trig, sensors[idx].echo);
    delay(20);
  }
  float sum = 0, maxv = vals[0], minv = vals[0];
  for (int i = 0; i < READS; i++) {
    if (vals[i] > maxv) maxv = vals[i];
    if (vals[i] < minv) minv = vals[i];
    sum += vals[i];
  }
  sum -= (maxv + minv);
  return sum / (READS - 2);
}

void updateIndicators(int idx, bool occupied) {
  digitalWrite(leds[idx].green, occupied ? LOW : HIGH);
  digitalWrite(leds[idx].red,   occupied ? HIGH : LOW);
}

void beepFullAlert() {
  digitalWrite(BUZZER_PIN, HIGH); delay(150);
  digitalWrite(BUZZER_PIN, LOW);  delay(200);
  digitalWrite(BUZZER_PIN, HIGH); delay(150);
  digitalWrite(BUZZER_PIN, LOW);
}

void setServoAngle(int angle) {
  ledcWrite(SERVO_CH, servoAngleToDuty(angle));
}

void drawOLED(int freeCnt) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.printf("Parking Free: %d/4\n", freeCnt);
  display.setCursor(0, 18);
  display.printf("S1: %s  S2: %s\n", slots[0].occupied ? "OCC" : "FREE", slots[1].occupied ? "OCC" : "FREE");
  display.setCursor(0, 32);
  display.printf("S3: %s  S4: %s\n", slots[2].occupied ? "OCC" : "FREE", slots[3].occupied ? "OCC" : "FREE");
  if (freeCnt == 0) {
    display.setCursor(0, 48);
    display.print(">> PARKING FULL <<");
  }
  display.display();
}

String generateHtml(int freeCnt) {
  String page = "<!doctype html><html><head><meta http-equiv='refresh' content='3'/>";
  page += "<meta name='viewport' content='width=device-width,initial-scale=1'>";
  page += "<style>body{font-family:sans-serif;text-align:center;padding:20px;} .card{display:inline-block;padding:15px;margin:10px;border-radius:8px;color:#fff;} .free{background:#28a745;} .occ{background:#dc3545;}</style></head><body>";
  page += "<h2>Smart Parking Status</h2><h3>Available Slots: " + String(freeCnt) + " / 4</h3><div>";
  for (int i = 0; i < 4; i++) {
    page += "<div class='card " + String(slots[i].occupied ? "occ" : "free") + "'>Slot " + String(i + 1) + "<br>" + (slots[i].occupied ? "Occupied" : "Free") + "</div>";
  }
  page += "</div></body></html>";
  return page;
}

void handleRoot() {
  int occ = 0;
  for (int i = 0; i < 4; i++) if (slots[i].occupied) occ++;
  server.send(200, "text/html", generateHtml(4 - occ));
}

void setup() {
  Serial.begin(115200);
  pinMode(BUZZER_PIN, OUTPUT);
  for (int i = 0; i < 4; i++) {
    pinMode(sensors[i].trig, OUTPUT);
    pinMode(sensors[i].echo, INPUT);
    pinMode(leds[i].green, OUTPUT);
    pinMode(leds[i].red, OUTPUT);
    slots[i] = {false, false, 0, 9999};
    updateIndicators(i, false);
  }

  ledcSetup(SERVO_CH, SERVO_FREQ, SERVO_RES);
  ledcAttachPin(SERVO_PIN, SERVO_CH);
  setServoAngle(0);

  Wire.begin(21, 22);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.display();

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  unsigned long startWi = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - startWi < 8000) {
    delay(250);
  }
  server.on("/", handleRoot);
  server.begin();
  drawOLED(4);
}

void loop() {
  server.handleClient();
  unsigned long now = millis();

  for (int i = 0; i < 4; i++) {
    float avg = averageDistance(i);
    bool occDecision = avg < THRESHOLD_CM[i];

    if (occDecision != slots[i].occupied) {
      if (!slots[i].pending) {
        slots[i].pending = true;
        slots[i].lastChange = now;
      } else if (now - slots[i].lastChange > DEBOUNCE_MS) {
        slots[i].occupied = occDecision;
        slots[i].pending = false;
        updateIndicators(i, slots[i].occupied);
      }
    } else {
      slots[i].pending = false;
    }
    slots[i].lastAvg = avg;
  }

  int occ = 0;
  for (int i = 0; i < 4; i++) if (slots[i].occupied) occ++;
  int freeCnt = 4 - occ;

  static unsigned long lastUi = 0;
  if (millis() - lastUi > 800) {
    drawOLED(freeCnt);
    lastUi = millis();
  }

  static unsigned long lastGate = 0;
  if (millis() - lastGate > 5000) {
    if (freeCnt > 0) {
      setServoAngle(90);
      delay(2000);
      setServoAngle(0);
    } else {
      beepFullAlert();
    }
    lastGate = millis();
  }
}