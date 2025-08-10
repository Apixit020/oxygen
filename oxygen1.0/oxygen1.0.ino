// --- Global Variables ---
int oxygen_sensor = 35;
int wind_sensor = 34;
int led1 = 32;
int led2 = 33;
int led3 = 25;
int led4 = 26;
int led5 = 27;
int led6 = 14;

// (ปรับปรุง) เปลี่ยนชื่อ 'frequency' เป็น 'blowScore' เพื่อให้สื่อความหมายมากขึ้น
int blowScore = 0;
int scoreIncrement = 1;

// ตัวแปรสำหรับ non-blocking timers
unsigned long last_update = 0;
unsigned long lastScoreTime = 0;  // (ปรับปรุง) ตัวแปรใหม่สำหรับจับเวลาการเพิ่มคะแนน

// ตัวแปรสถานะการเป่า
bool isBlowing = false;
unsigned long blowStartTime = 0;
unsigned long blowDuration = 0;

void setup() {
  // ส่วน setup ยังคงเดิม ไม่มีการเปลี่ยนแปลง
  pinMode(oxygen_sensor, INPUT);
  pinMode(wind_sensor, INPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
  pinMode(led5, OUTPUT);
  pinMode(led6, OUTPUT);
  Serial.begin(115200);
}

void loop() {
  // อ่านค่าต่างๆ ในตอนต้นของ loop
  unsigned long current_time = millis();
  int oxygen_val = analogRead(oxygen_sensor);
  int wind_val = analogRead(wind_sensor);
  int scaled_wind_val = wind_val / 4;  // สร้างตัวแปรใหม่เพื่อให้อ่านง่าย

  // --- Blow Detection Logic (ปรับปรุงโดยไม่ใช้ delay()) ---
  if (scaled_wind_val >= 600) {
    if (!isBlowing) {
      isBlowing = true;
      blowStartTime = current_time;
      lastScoreTime = current_time;  // เริ่มจับเวลาสำหรับนับคะแนน
      Serial.println("Start blowing...");
    }

    // (ปรับปรุง) ใช้ millis() แทน delay() เพื่อเพิ่มคะแนนทุก 500ms
    // ทำให้โปรแกรมไม่หยุดชะงักและส่วนอื่นๆ (เช่น LED) ยังทำงานได้ปกติ
    if (current_time - lastScoreTime >= 500) {
      blowScore += scoreIncrement;
      lastScoreTime = current_time;  // อัปเดตเวลาที่เพิ่มคะแนนล่าสุด
    }

  } else {
    if (isBlowing) {
      isBlowing = false;
      blowDuration = (current_time - blowStartTime) / 1000;
      Serial.print("Stopped blowing. Duration: ");
      Serial.print(blowDuration);
      Serial.println(" seconds");

      // พิมพ์คะแนนสุดท้ายก่อนรีเซ็ต
      Serial.print("Final Score: ");
      Serial.println(blowScore);

      blowScore = 0;  // รีเซ็ตคะแนน
    }
  }

  // --- Serial Monitor Output (ปรับปรุงชื่อตัวแปร) ---
  if (current_time - last_update >= 500) {
    Serial.print("  Wind Analog: ");
    Serial.print(scaled_wind_val);
    Serial.print("  Score: ");  // เปลี่ยนจาก Frequency
    Serial.println(blowScore);
    Serial.print("Oxygen Analog: ");
    Serial.println(oxygen_val);
    last_update = current_time;
  }

  // --- LED Bar Graph Control (ปรับปรุง Logic และการเขียนโค้ด) ---
  // ใช้ if-else if-else เพื่อประสิทธิภาพที่ดีกว่า (เมื่อเงื่อนไขหนึ่งถูก, จะไม่เช็คเงื่อนไขที่เหลือ)
  // และใช้ digitalWrite() ซึ่งเหมาะสมกับการเปิด/ปิด LED มากกว่า
  if (scaled_wind_val <= 519) {
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
    digitalWrite(led3, LOW);
    digitalWrite(led4, LOW);
    digitalWrite(led5, LOW);
    digitalWrite(led6, LOW);
  } else if (scaled_wind_val <= 584) {
    digitalWrite(led1, HIGH);
    digitalWrite(led2, LOW);
    digitalWrite(led3, LOW);
    digitalWrite(led4, LOW);
    digitalWrite(led5, LOW);
    digitalWrite(led6, LOW);
  } else if (scaled_wind_val <= 668) {
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
    digitalWrite(led3, LOW);
    digitalWrite(led4, LOW);
    digitalWrite(led5, LOW);
    digitalWrite(led6, LOW);
  } else if (scaled_wind_val <= 752) {
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
    digitalWrite(led3, HIGH);
    digitalWrite(led4, LOW);
    digitalWrite(led5, LOW);
    digitalWrite(led6, LOW);
  } else if (scaled_wind_val <= 836) {
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
    digitalWrite(led3, HIGH);
    digitalWrite(led4, HIGH);
    digitalWrite(led5, LOW);
    digitalWrite(led6, LOW);
  } else if (scaled_wind_val <= 920) {
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
    digitalWrite(led3, HIGH);
    digitalWrite(led4, HIGH);
    digitalWrite(led5, HIGH);
    digitalWrite(led6, LOW);
  } else {  // ค่ามากกว่า 920
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
    digitalWrite(led3, HIGH);
    digitalWrite(led4, HIGH);
    digitalWrite(led5, HIGH);
    digitalWrite(led6, HIGH);
  }
}