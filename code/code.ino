#include <AltSoftSerial.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 4);

// RO to pin 8 & DI to pin 9 when using AltSoftSerial
#define RE 6
#define DE 7

const byte nitro[] = {0x01, 0x03, 0x00, 0x1e, 0x00, 0x01, 0xe4, 0x0c};
const byte phos[] = {0x01, 0x03, 0x00, 0x1f, 0x00, 0x01, 0xb5, 0xcc};
const byte pota[] = {0x01, 0x03, 0x00, 0x20, 0x00, 0x01, 0x85, 0xc0};

byte values[11];
AltSoftSerial mod;

int motorPin = 5;
int sensorPin = A0;
int sensorValue; 
int vocht;
int fout;
void setup() {
  Serial.begin(19200);
  mod.begin(9600);
  pinMode(sensorValue, INPUT);
  pinMode(RE, OUTPUT);
  pinMode(DE, OUTPUT);
  pinMode(motorPin, OUTPUT);
  digitalWrite(DE, LOW);
  digitalWrite(RE, LOW);
  lcd.init();
  lcd.backlight();
}

void loop() {
 Serial.println("test oke");
  digitalWrite(13,HIGH);
  delay(1000);
  digitalWrite(13, LOW);
  delay(1000);
  sensorValue = analogRead(sensorPin);
  vocht = map(sensorValue, 826, 454, 0, 100);
  byte val1, val2, val3;

  val1 = nitrogen();
  delay(250);

  val2 = phosphorous();
  delay(250);

  val3 = potassium();
  delay(250);
if ((val1 == 255)& (val2==255)&(val3==255)){
    fout = 1;
    lcd.clear();
    lcd.setCursor(5,1);
    lcd.print("loading");
    lcd.setCursor(12,1);
    lcd.print(".");
    delay(500);
    lcd.print(".");
    delay(500);
    lcd.print(".");
    delay(500);

  }else{
    fout =0;
  }
  if (fout == 0){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("vochtigheid : ");
    lcd.setCursor(14,0);
    lcd.print(vocht);
    if (vocht <10){
      lcd.setCursor(16,0);
      lcd.print("%");
    }
    else if(vocht <100) {
      lcd.setCursor(17,0);
      lcd.print("%");
    }
    else if(vocht = 100){
      lcd.setCursor(18,0);
      lcd.print("%");
    }

    lcd.setCursor(0, 1);
    lcd.print("stikstof : ");
    lcd.setCursor(11,1);
    lcd.print(val1);
    if (val1 <10){
      lcd.setCursor(13,1);
      lcd.print("mg/kg");
    }
    else if (val1 < 100) {
      lcd.setCursor(14,1);
      lcd.print("mg/kg");
    }
    else if (val1 >100){
      lcd.setCursor(15,1);
      lcd.print("mg/kg");   
    }

    lcd.setCursor(0, 2);
    lcd.print("fosfaat : ");
    lcd.setCursor(11,2);
    lcd.print(val2);
    if (val2 <10){
      lcd.setCursor(13,2);
      lcd.print("mg/kg");
    }
    else if (val2 < 100) {
      lcd.setCursor(14,2);
      lcd.print("mg/kg");
    }
    else if (val2 >100){
      lcd.setCursor(15,2);
      lcd.print("mg/kg");   
    }

    lcd.setCursor(0, 3);
    lcd.print("kalium : ");
    lcd.setCursor(11,3);
    lcd.print(val3);
    if (val3 <10){
      lcd.setCursor(13,3);
      lcd.print("mg/kg");
    }
    else if (val3 < 100) {
      lcd.setCursor(14,3);
      lcd.print("mg/kg");
    }
    else if (val3 >100){
      lcd.setCursor(15,3);
      lcd.print("mg/kg");   
    }
  }

}

byte nitrogen() {
  mod.flushInput();
  digitalWrite(DE, HIGH);
  digitalWrite(RE, HIGH);
  delay(1);
  for (uint8_t i = 0; i < sizeof(nitro); i++ ) mod.write( nitro[i] );
  mod.flush();
  digitalWrite(DE, LOW);
  digitalWrite(RE, LOW);
  delay(200);
  for (byte i = 0; i < 7; i++) {
    values[i] = mod.read();
    Serial.print(values[i], HEX);
    Serial.print(' ');
  }
  return values[4];
}

byte phosphorous() {
  mod.flushInput();
  digitalWrite(DE, HIGH);
  digitalWrite(RE, HIGH);
  delay(1);
  for (uint8_t i = 0; i < sizeof(phos); i++ ) mod.write( phos[i] );
  mod.flush();
  digitalWrite(DE, LOW);
  digitalWrite(RE, LOW);
  delay(200);
  for (byte i = 0; i < 7; i++) {
    values[i] = mod.read();
    Serial.print(values[i], HEX);
    Serial.print(' ');
  }
  return values[4];
}

byte potassium() {
  mod.flushInput();
  digitalWrite(DE, HIGH);
  digitalWrite(RE, HIGH);
  delay(1);
  for (uint8_t i = 0; i < sizeof(pota); i++ ) mod.write( pota[i] );
  mod.flush();
  digitalWrite(DE, LOW);
  digitalWrite(RE, LOW);
  delay(200);
  for (byte i = 0; i < 7; i++) {
    values[i] = mod.read();
    Serial.print(values[i], HEX);
    Serial.print(' ');
  }
  return values[4];
}