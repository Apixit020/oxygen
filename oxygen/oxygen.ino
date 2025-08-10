int oxygen_sensor = 35;
int wind_sensor = 34;
int led1 = 32;
int led2 = 33;
int led3 = 25;
int led4 = 26;
int led5 = 27;
int led6 = 14;
int frequency = 0;
unsigned long last_update = 0;
unsigned long last_reset_time = 0;
bool isBlowing = false;
unsigned long blowStartTime = 0;
unsigned long blowDuration = 0;
int f = 1;


void setup() {
  // put your setup code here, to run once:

  pinMode(oxygen_sensor, INPUT);
  pinMode(wind_sensor, INPUT);
  pinMode(led1, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led4, OUTPUT);
  pinMode(led5, OUTPUT);
  pinMode(led6, OUTPUT);
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  int oxygen_val = analogRead(oxygen_sensor);
  int wind_val = analogRead(wind_sensor);
  unsigned long current_time = millis();

  if (wind_val / 4 >= 600) {
    if (!isBlowing) {
      isBlowing = true;                  // เริ่มเป่า
      blowStartTime = millis();         // เริ่มจับเวลา
      Serial.println(" Start blowing...");
    }
    frequency += f;                      // เพิ่มความถี่ทีละ 1 ระหว่างเป่า
    delay(500);

  }

  else {
    if (isBlowing) {
      isBlowing = false;                // หยุดเป่า
      blowDuration = (millis() - blowStartTime) / 1000; // คำนวณเวลาที่เป่า
      Serial.print(" Stopped blowing. Duration: ");
      Serial.print(blowDuration);
      Serial.println(" seconds");

      frequency = 0;                    // รีเซ็ต frequency หลังเป่าจบ
    }
  }


  if (millis() - last_update >= 500) {
    Serial.print("  Wind Analog: ");
    Serial.print(wind_val / 4);
    Serial.print("  Frequency: ");
    Serial.println(frequency);
    Serial.print("Oxygen Analog: ");
    Serial.print(oxygen_val);
    last_update = millis();
  }


  if (wind_val / 4 <= 519) {
    analogWrite(led1, 0);
    analogWrite(led2, 0);
    analogWrite(led3, 0);
    analogWrite(led4, 0);
    analogWrite(led5, 0);
    analogWrite(led6, 0);
  }
  if (wind_val / 4 >= 520 && wind_val / 4 <= 584) {
    analogWrite(led1, 255);
    analogWrite(led2, 0);
    analogWrite(led3, 0);
    analogWrite(led4, 0);
    analogWrite(led5, 0);
    analogWrite(led6, 0);
  }

  if (wind_val / 4 >= 585 && wind_val / 4 <= 668) {
    analogWrite(led1, 255);
    analogWrite(led2, 255);
    analogWrite(led3, 0);
    analogWrite(led4, 0);
    analogWrite(led5, 0);
    analogWrite(led6, 0);
  }

  if (wind_val / 4 >= 669 && wind_val / 4 <= 752) {
    analogWrite(led1, 255);
    analogWrite(led2, 255);
    analogWrite(led3, 255);
    analogWrite(led4, 0);
    analogWrite(led5, 0);
    analogWrite(led6, 0);
  }
  if (wind_val / 4 >= 753 && wind_val / 4 <= 836) {
    analogWrite(led1, 255);
    analogWrite(led2, 255);
    analogWrite(led3, 255);
    analogWrite(led4, 255);
    analogWrite(led5, 0);
    analogWrite(led6, 0);
  }
  if (wind_val / 4 >= 837 && wind_val / 4 <= 920) {
    analogWrite(led1, 255);
    analogWrite(led2, 255);
    analogWrite(led3, 255);
    analogWrite(led4, 255);
    analogWrite(led5, 255);
    analogWrite(led6, 0);
  }
  if (wind_val / 4 >= 921 && wind_val / 4 <= 1023) {
    analogWrite(led1, 255);
    analogWrite(led2, 255);
    analogWrite(led3, 255);
    analogWrite(led4, 255);
    analogWrite(led5, 255);
    analogWrite(led6, 255);
  }

}