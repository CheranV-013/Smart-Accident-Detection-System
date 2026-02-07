#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Wire.h>
#include <MPU6050.h>
#include <SoftwareSerial.h>
#include <TinyGPS++.h>
#include <ESP_Mail_Client.h>
#include <math.h>

// ------------------- Wi-Fi Credentials -------------------
#define WIFI_SSID     "iPhone"
#define WIFI_PASSWORD "Cherancheran"

// ------------------- Email Settings ----------------------
#define SMTP_HOST "smtp.gmail.com"
#define SMTP_PORT 587
#define AUTHOR_EMAIL    "theche200699@gmail.com"
#define AUTHOR_PASSWORD "qcowlajxzrxxoyvj"
#define RECIPIENT_EMAIL "kit28.24bam013@gmail.com"

// ------------------- Objects & Variables -----------------
MPU6050 mpu;
SMTPSession smtp;
TinyGPSPlus gps;
SoftwareSerial gpsSerial(D6, D7);
ESP8266WebServer server(80);

const int ledPin = D4;
const float ACC_THRESHOLD = 1.4;

double lastLat = 0.0;
double lastLng = 0.0;
bool gpsFixed = false;

// ----------- Web Variables -----------
float webTotalG = 0;
bool webAccident = false;

// ----------- LATCH VARIABLES (ADDED) -----------
bool accidentLatched = false;
unsigned long accidentStartTime = 0;
const unsigned long ACC_HOLD_TIME = 8000; // 8 sec

// ------------------- WEB API -------------------
void handleData() {
  server.sendHeader("Access-Control-Allow-Origin", "*");

  String json = "{";
  json += "\"totalG\":" + String(webTotalG, 2) + ",";
  json += "\"accident\":" + String(webAccident ? "true" : "false") + ",";
  json += "\"lat\":" + String(lastLat, 6) + ",";
  json += "\"lng\":" + String(lastLng, 6);
  json += "}";

  server.send(200, "application/json", json);
}

void setup() {
  Serial.begin(9600);
  gpsSerial.begin(9600);

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  // WiFi
  Serial.println("Connecting to WiFi...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n✅ Connected to WiFi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Web server
  server.on("/data", handleData);
  server.begin();
  Serial.println("Web Server Started");

  // MPU6050
  Wire.begin(D2, D1);
  Wire.setClock(100000);
  delay(100);

  mpu.initialize();
  if (mpu.testConnection()) {
    Serial.println("✅ MPU6050 connected");
  } else {
    Serial.println("❌ MPU6050 connection failed");
    while (1) delay(2000);
  }
}

void loop() {

  server.handleClient();

  // GPS
  while (gpsSerial.available()) {
    char c = gpsSerial.read();
    gps.encode(c);

    if (gps.location.isValid()) {
      lastLat = gps.location.lat();
      lastLng = gps.location.lng();
      gpsFixed = true;
    }
  }

  // MPU
  int16_t ax, ay, az;
  mpu.getAcceleration(&ax, &ay, &az);

  float gX = ax / 16384.0;
  float gY = ay / 16384.0;
  float gZ = az / 16384.0;
  float totalG = sqrt(gX*gX + gY*gY + gZ*gZ);

  webTotalG = totalG;

  // ------------ Accident Detection ------------
  if (totalG > ACC_THRESHOLD && !accidentLatched) {

    webAccident = true;
    accidentLatched = true;
    accidentStartTime = millis();

    String direction = getImpactDirection(gX, gY, gZ);
    Serial.println("🚨 Accident detected! Direction: " + direction);
    digitalWrite(ledPin, HIGH);

    String emailBody = "🚨 Accident Detected!\n\n";
    emailBody += "Total G: " + String(totalG, 2) + "\n";
    emailBody += "Direction: " + direction + "\n";

    sendEmail("🚨 Accident Alert", emailBody);

    delay(200); // reduced only
  }

  // -------- LATCH HOLD --------
  if (accidentLatched) {
    webAccident = true;

    if (millis() - accidentStartTime > ACC_HOLD_TIME) {
      accidentLatched = false;
      webAccident = false;
      digitalWrite(ledPin, LOW);
    }
  }
}

// ------------------- Impact Direction -------------------
String getImpactDirection(float gX, float gY, float gZ) {
  if (abs(gX) > abs(gY)) return gX > 0 ? "Right" : "Left";
  return gY > 0 ? "Forward" : "Backward";
}

// ------------------- Send Email -------------------
void sendEmail(String subject, String body) {
  ESP_Mail_Session session;
  session.server.host_name = SMTP_HOST;
  session.server.port = SMTP_PORT;
  session.login.email = AUTHOR_EMAIL;
  session.login.password = AUTHOR_PASSWORD;
  session.secure.startTLS = true;

  if (!smtp.connect(&session)) return;

  SMTP_Message message;
  message.sender.email = AUTHOR_EMAIL;
  message.subject = subject;
  message.addRecipient("User", RECIPIENT_EMAIL);
  message.text.content = body;

  MailClient.sendMail(&smtp, &message);
}
