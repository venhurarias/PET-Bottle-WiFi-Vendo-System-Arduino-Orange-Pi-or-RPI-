#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <Streaming.h>
#include <Servo.h>
#include "HX711.h"
#include <Chrono.h>

#define PULSE_PIN 9
#define RPI_START_PIN 10
#define SERVO_PIN 11
#define BUZZER_PIN 8
#define PHOTOELECTRIC_PIN 5

#define IR_PIN 4
#define DT_PIN 3
#define SCK_PIN 2
#define OBJECT_PIN 6
#define OBJECT_PIN_1 7


LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo myservo;
HX711 scale;
Chrono myChrono;

int mode = 0;
float prevWeight;
float newWeight;
void setup() {
  Serial.begin(9600);
  Serial.setTimeout(100);

  Wire.begin();
  // findI2CAddress();
  lcd.init();
  lcd.backlight();

  pinMode(PULSE_PIN, OUTPUT);
  pinMode(RPI_START_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(PHOTOELECTRIC_PIN, INPUT_PULLUP);


  pinMode(IR_PIN, INPUT);
  pinMode(OBJECT_PIN, INPUT);
  pinMode(OBJECT_PIN_1, INPUT);


  myservo.attach(SERVO_PIN);

  scale.begin(DT_PIN, SCK_PIN);
  scale.set_scale(-743.8f);
  delay(2000);

  while (rpiOnStart())
    ;

  resetScale();
}

void loop() {
  //   Serial << "Is Bottle Accepted :: " << isBottleAccepted() << endl;
  //   Serial << "Is Bin Full :: " << isBinFull() << endl;
  //   Serial << "Is Object Detected :: " << isObjectDetected() << endl;
  // Serial << " IS RPI READY :: " << rpiOnStart() << endl;
  // Serial << "Weight :: " << getWeight() << endl;

  //testing();
  //lcdTest();
  normalProcess();
}
void lcdTest() {
  lcd.setCursor(0, 0), lcd << F("      Hello     ");
  lcd.setCursor(0, 1), lcd << F("     WORLD!!!   ");
}

void normalProcess() {
  if (isBinFull()) {
    Serial << F("Bin Full") << endl;
    lcd.setCursor(0, 0), lcd << F("     BIN IS     ");
    lcd.setCursor(0, 1), lcd << F("      FULL      ");
    return;
  }
  if (rpiOnStart()) {
    if (isObjectDetected()) {
      if (myChrono.hasPassed(2000)) {
        if (isBottleAccepted()) {
          newWeight = getWeight();
          if (newWeight <= 11 || newWeight >= 43) {
            while (isObjectDetected()) {
              lcd.setCursor(0, 0), lcd << F("   PET BOTTLE    ");
              lcd.setCursor(0, 0), lcd << F("    REJECTED     ");
              buzzerOn();
              delay(100);
            }
            buzzerOff();
            resetScale();
             myChrono.restart();
            return;
          }

          openLid();
          lcd.setCursor(0, 0), lcd << F("   PET BOTTLE    ");
          lcd.setCursor(0, 0), lcd << F("    ACCEPTED     ");
          //11-43
          if (newWeight <= 14) {
            writePulse(1);
          } else if (newWeight <= 17) {
            writePulse(2);
          } else if (newWeight <= 21) {
            writePulse(3);
          } else {
            writePulse(4);
          }

          delay(2000);
          closeLid();
          delay(1000);
          resetScale();
        } else {
          while (isObjectDetected()) {
            lcd.setCursor(0, 0), lcd << F("   PET BOTTLE    ");
            lcd.setCursor(0, 0), lcd << F("    REJECTED     ");
            buzzerOn();
            delay(100);
          }
           buzzerOff();
        }
        myChrono.restart();
      } else {
        lcd.setCursor(0, 0), lcd << F("  PLEASE INSERT ");
        lcd.setCursor(0, 1), lcd << F("   PET BOTTLE   ");
      }
    } else {
      lcd.setCursor(0, 0), lcd << F("  PLEASE INSERT ");
      lcd.setCursor(0, 1), lcd << F("   PET BOTTLE   ");
      myChrono.restart();
    }
    prevWeight = newWeight;
  } else {
    closeLid();

    Serial << "Waiting to Connect" << endl;
    lcd.setCursor(0, 0), lcd << F("   Bote-Link  ");
    lcd.setCursor(0, 1), lcd << F("================");
  }
}
void testing() {
  if (Serial.available()) {
    String reading = Serial.readString();
    Serial << reading << endl;

    reading.trim();
    if (reading == "0") {
      Serial << "Reset Scale" << endl;
      resetScale();
    } else if (reading == "1") {
      Serial << "Write Pulse" << endl;
      writePulse(1);
    } else if (reading == "5") {
      Serial << "Write Pulse" << endl;
      writePulse(5);
    } else if (reading == "q") {
      Serial << "Close Lid" << endl;
      closeLid();
    } else if (reading == "a") {
      Serial << "Open Lid" << endl;
      openLid();
    }
  }
}

bool rpiOnStart() {
  return digitalRead(RPI_START_PIN);
}
void buzzerOn() {
  digitalWrite(BUZZER_PIN, HIGH);
}

void buzzerOff() {
  digitalWrite(BUZZER_PIN, LOW);
}
void closeLid() {
  myservo.write(0);
}

void openLid() {
  myservo.write(180);
}

void writePulse(int x) {
  for (int i = 0; i < x; i++) {
    Serial << "Pulse" << endl;
    onePulse();
  }
}

void onePulse() {
  digitalWrite(PULSE_PIN, HIGH);
  delay(100);
  digitalWrite(PULSE_PIN, LOW);
  delay(500);
}

bool isBottleAccepted() {
  return digitalRead(PHOTOELECTRIC_PIN);
}

bool isBinFull() {
  return !digitalRead(IR_PIN);
}
bool isObjectDetected() {
  return !digitalRead(OBJECT_PIN) && !digitalRead(OBJECT_PIN_1);
}

void resetScale() {
  scale.tare();
}

float getWeight() {
  float x = scale.get_units(10);
  Serial << "WEIGHT :: " << x << endl;
  return x;
}

void findI2CAddress() {

  byte error, address;
  int nDevices;

  Serial.println("Scanning...");

  nDevices = 0;
  for (address = 1; address < 127; address++) {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0) {
      Serial.print("I2C device found at address 0x");
      if (address < 16)
        Serial.print("0");
      Serial.print(address, HEX);
      Serial.println("  !");

      nDevices++;
    } else if (error == 4) {
      Serial.print("Unknown error at address 0x");
      if (address < 16)
        Serial.print("0");
      Serial.println(address, HEX);
    }
  }
  if (nDevices == 0)
    Serial.println("No I2C devices found\n");
  else
    Serial.println("done\n");

  while (true)
    ;
}