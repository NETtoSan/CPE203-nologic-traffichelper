#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// ---------------- Display ----------------
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// ---------------- Input ----------------
const int buttonPin = 15;     // ปุ่มกดคนข้าม
const int trigPin   = 16;      // Ultrasonic trigger
const int echoPin   = 17;      // Ultrasonic echo
const int ldrPin    = 34;     // LDR analog input

// ---------------- Output ----------------
const int greenPin = 5;      // ไฟเขียวรถ
const int yellowPin = 18;     // ไฟเหลืองรถ
const int redPin = 19;        // ไฟแดงรถ

// ---------------- Setting ----------------
const int distanceThreshold = 100;  // cm ถ้ามีรถใกล้
const int ldrThreshold = 1000;      // ค่าประมาณว่ามืด (ขึ้นกับ LDR จริงๆ)
const int crossDayTime = 15;        // เวลากลางวัน
const int crossNightTime = 5;      // เวลากลางคืน
const int globalTime = 3;           // เวลาไฟเหลือง 3 วิ

// ---------------- Variables ----------------
bool crossingActive = false;
unsigned long crossStart = 0;
int crossDuration = 0;

long readDistanceCM() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH, 30000); // timeout 30ms
  return duration * 0.034 / 2;
}

void setup() {
  Serial.begin(9600);

  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(ldrPin, INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(greenPin, OUTPUT);
  pinMode(yellowPin, OUTPUT);
  pinMode(redPin, OUTPUT);

  // เริ่มไฟปกติเป็นไฟเขียว
  digitalWrite(greenPin, HIGH);
  digitalWrite(yellowPin, LOW);
  digitalWrite(redPin, LOW);

  // OLED
  Wire.begin();
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 init failed"));
    for(;;);
  }
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  // Draw a simple car shape on the OLED
  display.drawRect(20, 30, 80, 20, SSD1306_WHITE);    // car body
  display.fillRect(30, 40, 60, 10, SSD1306_WHITE);    // car lower body
  display.fillCircle(35, 55, 5, SSD1306_WHITE);       // left wheel
  display.fillCircle(93, 55, 5, SSD1306_WHITE);       // right wheel
  display.setCursor(35, 10);
  display.println("Traffic");
  display.display();
  Serial.println("Setup done");
  delay(1000);
}

void loop() {
  int buttonState = digitalRead(buttonPin);
  long distance = readDistanceCM();
  int ldrValue = analogRead(ldrPin);

  // ถ้ามีการกดปุ่ม และไม่มีรถใกล้ และยังไม่อยู่ในโหมดข้าม
  Serial.printf("%d %d %d\n", buttonState, distance, ldrValue);
  delay(100);
  if (buttonState == LOW && !crossingActive) {

    // If there are cars
    if (distance < 30){
      globtime = 45;
    }
    // otherwise....
    else{
      globtime = 3;
    }

    // เปิดไฟgreenและนับถอยหลัง แล้วแต่เวลาที่ตั้งไว้
    digitalWrite(greenPin, HIGH);
    for (int i = globtime; i > 0; i--) {
      display.clearDisplay();
      display.setTextSize(2);
      display.setCursor(20, 20);
      display.print("Green ");
      display.print(i);
      display.display();
      delay(1000);
    }
    digitalWrite(greenPin, LOW);

    // เปิดไฟเหลืองและนับถอยหลัง 3 วิ
    digitalWrite(greenPin, LOW);
    digitalWrite(yellowPin, HIGH);
    for (int i = globalTime; i > 0; i--) {
      display.clearDisplay();
      display.setTextSize(2);
      display.setCursor(20, 20);
      display.print("Yellow ");
      display.print(i);
      display.display();
      delay(1000);
    }
    digitalWrite(yellowPin, LOW);

    // เปลี่ยนเป็นไฟแดง
    digitalWrite(redPin, HIGH);

    // กำหนดเวลาให้คนข้าม
    if (ldrValue < ldrThreshold) {
      crossDuration = crossNightTime;  // กลางคืน
    } else {
      crossDuration = crossDayTime;    // กลางวัน
    }

    crossingActive = true;
    crossStart = millis();
  }

  // ถ้าอยู่ในโหมดข้าม
  if (crossingActive) {
    unsigned long elapsed = (millis() - crossStart) / 1000;
    int remaining = crossDuration - elapsed;

    // แสดงเวลานับถอยหลังบน OLED
    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(10, 20);
    display.print("Cross: ");
    display.print(remaining);
    
    display.display();

    if (remaining <= 0) {
      // จบโหมดข้าม
      crossingActive = false;

      // เปลี่ยนกลับเป็นไฟเขียว
      digitalWrite(redPin, LOW);
      digitalWrite(greenPin, HIGH);

      display.clearDisplay();
      display.setCursor(20, 20);
      display.println("Go Car");
      display.display();
    }
  }
}
