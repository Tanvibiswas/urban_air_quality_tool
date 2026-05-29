#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// LCD address (0x27 or 0x3F)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Pin definitions
#define SOIL_PIN A0
#define MQ_AO_PIN A1
#define MQ_DO_PIN 3
#define VIB_PIN 2
#define LED_PIN 13 // onboard LED

// Thresholds
#define GAS_THRESHOLD 300

void setup() {
  // Internal pull-up for 2-pin soil probe
  pinMode(SOIL_PIN, INPUT_PULLUP);

  pinMode(VIB_PIN, INPUT);
  pinMode(MQ_DO_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);

  lcd.init();
  lcd.backlight();

  Serial.begin(9600);

  lcd.setCursor(0, 0);
  lcd.print("Air Quality Hub");
  lcd.setCursor(0, 1);
  lcd.print("Warming up...");
  delay(3000);
}

void loop() {
  int dustValue = analogRead(SOIL_PIN);
  int gasValue  = analogRead(MQ_AO_PIN);  // analog value
  int gasDO    = digitalRead(MQ_DO_PIN);  // digital threshold state
  int vibration = digitalRead(VIB_PIN);

  Serial.print("Dust: "); Serial.print(dustValue);
  Serial.print(" | Gas(AO): "); Serial.print(gasValue);
  Serial.print(" | Gas(DO): "); Serial.print(gasDO);
  Serial.print(" | Vib: "); Serial.println(vibration);

  // Control onboard LED when analog gas > threshold
  if (gasValue > GAS_THRESHOLD) {
    digitalWrite(LED_PIN, HIGH);
  } else {
    digitalWrite(LED_PIN, LOW);
  }

  // Update LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("D:");
  lcd.print(dustValue);
  lcd.print(" G:");
  lcd.print(gasValue);

  lcd.setCursor(0, 1);
  if (gasValue > GAS_THRESHOLD) {
    lcd.print("GAS DETECTED  ");
  }
  else if (dustValue < 500 && vibration == HIGH) {
    lcd.print("INDUSTRIAL AIR");
  }
  else if (dustValue < 500) {
    lcd.print("DUSTY AIR     ");
  }
  else {
    lcd.print("AIR CLEAN     ");
  }

  delay(1000);
}