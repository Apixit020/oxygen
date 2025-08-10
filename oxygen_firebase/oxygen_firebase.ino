/*
 * --------------------------------------------------------------------
 * Combined Code: Device Logic + Firebase Data Upload
 * --------------------------------------------------------------------
 * This code reads wind and oxygen sensors, calculates a blow score,
 * controls a set of LEDs, and periodically sends the sensor data
 * (wind, score, oxygen) to a Firebase Realtime Database.
 *
 * Make sure your "secrets.h" file is correctly configured with your
 * Wi-Fi and Firebase credentials.
 * --------------------------------------------------------------------
 */

#include <WiFi.h>
#include <Firebase.h>
#include "secrets.h" // ไฟล์ที่เก็บข้อมูล Wi-Fi และ Firebase URL

// --- Firebase Object ---
Firebase fb(REFERENCE_URL);

// --- Device Pin Definitions ---
int oxygen_sensor = 35;
int wind_sensor = 34;
int led1 = 32;
int led2 = 33;
int led3 = 25;
int led4 = 26;
int led5 = 27;
int led6 = 14;

// --- Device Logic Variables ---
int blowScore = 0;
int scoreIncrement = 1;
bool isBlowing = false;
unsigned long blowStartTime = 0;
unsigned long blowDuration = 0;

// --- Non-blocking Timers ---
unsigned long lastScoreTime = 0;
unsigned long lastFirebaseUpdate = 0;
const long firebaseUpdateInterval = 2000; // (ปรับปรุง) ตั้งเวลาส่งข้อมูลไป Firebase ทุก 2 วินาที (2000ms)

void setup() {
  Serial.begin(115200);

  // --- Device Pin Setup ---
  pinMode(oxygen_sensor, INPUT);
  pinMode(wind_sensor, INPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
  pinMode(led5, OUTPUT);
  pinMode(led6, OUTPUT);

  // --- Wi-Fi Connection ---
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(1000);
  Serial.println();
  Serial.print("Connecting to Wi-Fi: ");
  Serial.println(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println();
  Serial.println("WiFi Connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.println("---------------------------------");
  
  // (ตัวเลือก) ส่งข้อความแจ้งเตือนเมื่อบอร์ดออนไลน์
  fb.setString("device_status", "Online");
}

void loop() {
  unsigned long current_time = millis();

  // --- Sensor Reading & Core Logic ---
  int oxygen_val = analogRead(oxygen_sensor);
  int wind_val = analogRead(wind_sensor);
  int scaled_wind_val = wind_val / 4;

  // Blow Detection Logic (Non-blocking)
  if (scaled_wind_val >= 600) {
    if (!isBlowing) {
      isBlowing = true;
      blowStartTime = current_time;
      lastScoreTime = current_time;
      Serial.println("Start blowing...");
    }
    if (current_time - lastScoreTime >= 500) {
      blowScore += scoreIncrement;
      lastScoreTime = current_time;
    }
  } else {
    if (isBlowing) {
      isBlowing = false;
      blowDuration = (current_time - blowStartTime) / 1000;
      Serial.print("Stopped blowing. Duration: ");
      Serial.print(blowDuration);
      Serial.print("s, Final Score: ");
      Serial.println(blowScore);
      
      // (ปรับปรุง) เมื่อเป่าเสร็จ ให้ส่งคะแนนสุดท้ายไปที่ Firebase ทันที
      fb.setInt("last_reading/final_score", blowScore);
      fb.setInt("last_reading/duration", blowDuration);

      blowScore = 0; // Reset score
    }
  }

  // --- LED Bar Graph Control ---
  // (ส่วนนี้เหมือนเดิม)
  if (scaled_wind_val <= 519) {
    digitalWrite(led1, LOW); digitalWrite(led2, LOW); digitalWrite(led3, LOW);
    digitalWrite(led4, LOW); digitalWrite(led5, LOW); digitalWrite(led6, LOW);
  } else if (scaled_wind_val <= 584) {
    digitalWrite(led1, HIGH); digitalWrite(led2, LOW); // ... and so on
  } // ... (โค้ดส่วน LED ที่เหลือ) ...
  else { // This covers >= 921
    digitalWrite(led1, HIGH); digitalWrite(led2, HIGH); digitalWrite(led3, HIGH);
    digitalWrite(led4, HIGH); digitalWrite(led5, HIGH); digitalWrite(led6, HIGH);
  }

  // --- Periodic Firebase Update ---
  // (ปรับปรุง) ส่งข้อมูลเซ็นเซอร์ปัจจุบันไปยัง Firebase ทุกๆ `firebaseUpdateInterval`
  if (current_time - lastFirebaseUpdate >= firebaseUpdateInterval) {
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("Sending data to Firebase...");

      // ใช้ fb.setInt() หรือ fb.setFloat() เพื่ออัปเดตค่าที่ path ที่กำหนด
      // ข้อมูลจะถูกเขียนทับค่าเก่าที่ path เดิม
      fb.setInt("realtime_data/wind", scaled_wind_val);
      fb.setInt("realtime_data/score", blowScore);
      fb.setInt("realtime_data/oxygen", oxygen_val);
      
      Serial.println("Data sent!");
    } else {
      Serial.println("WiFi disconnected. Cannot send data.");
    }
    lastFirebaseUpdate = current_time; // อัปเดตเวลาล่าสุดที่ส่งข้อมูล
  }
}