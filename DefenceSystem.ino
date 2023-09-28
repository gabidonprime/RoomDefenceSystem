#include <Arduino.h>
#include "Adafruit_VL53L0X.h"
#include <IoAbstractionWire.h>
#include <LiquidCrystalIO.h>
#include <Wire.h>

LiquidCrystalI2C_RS_EN(lcd, 0x3F, false);
Adafruit_VL53L0X lox = Adafruit_VL53L0X();
int intruderCount = 0;

void setup() {
  Serial.begin(115200);
  Wire.begin();                  // This must be called to start I2C.
  lcd.begin(16, 2);              // Number of chars in a row and how many rows.
  lcd.configureBacklightPin(3);  // Backlight brightness (0 is off).
  lcd.backlight();               // Set backlight.
  lcd.clear();                   // Clear display of t
  // wait until serial port opens for native USB devices
  while (! Serial) {
    delay(1);
  }
  
  Serial.println("Adafruit VL53L0X test");
  if (!lox.begin()) {
    Serial.println(F("Failed to boot VL53L0X"));
    while(1);
  }
  // power 
  Serial.println(F("VL53L0X API Simple Ranging example\n\n")); 
}


void loop() {
  VL53L0X_RangingMeasurementData_t measure;
    
  Serial.println("Defence system on standby");
  lox.rangingTest(&measure, false); // pass in 'true' to get debug data printout!

  if (measure.RangeStatus != 4 && measure.RangeMilliMeter < 500) {  // phase failures have incorrect data
    Serial.print("Intruder detected ");
    intruderCount++; 
    Serial.print("Intruder count @ "); Serial.println(intruderCount);

    lcd.setCursor(0,0);
    lcd.print(intruderCount);
    delay(1000);
  }
  
  delay(100);
}
