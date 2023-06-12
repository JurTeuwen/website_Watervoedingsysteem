/* importeer library */
#include <AltSoftSerial.h>      // voor seriële communicatie met NPK-sensor
#include <Wire.h>               // voor I2C-communicatie met lcd-scherm
#include <LiquidCrystal_I2C.h>  // voor LCD-scherm aan te sturen

LiquidCrystal_I2C lcd(0x27, 20, 4);  // definieer adres en afmetingen van LCD-scherm

/* difineer pinnen voor I2C-communicatie */
#define RE 6
#define DE 7

/* RO to pin 8 & DI to pin 9 when using AltSoftSerial */

/* de informatie die word doorgestuurd naar de NPK-sensor in hexadecimaal */
const byte nitro[] = { 0x01, 0x03, 0x00, 0x1e, 0x00, 0x01, 0xe4, 0x0c };
const byte phos[] = { 0x01, 0x03, 0x00, 0x1f, 0x00, 0x01, 0xb5, 0xcc };
const byte pota[] = { 0x01, 0x03, 0x00, 0x20, 0x00, 0x01, 0x85, 0xc0 };

byte values[11];    // maak een array voor waarden op te slaan
AltSoftSerial mod;  // Maak een AltSoftSerial-object aan met de naam mod

int waterpompPin = 5;               // waterpomp zit op pin 5
int vochtigheidssensorPin = A0;     // vochtigheidssensor zit op pin A0
int vochtigheidssensorValue;        // Variabele om de waarde van de vochtigheidssensor op te slaan
int vochtpercentage;                // variabele om vochtpercentage op te slaan
int fout;                           // Variabele om foutindicatie bij te houden
int MinimunNiveau = 40;             // minimum waterpersentage is 40 %
unsigned long starttimer = 0;       // timer voor stoppen van pomp
int ingestelde_water_tijd = 10000;  // tijd voordat pomp moet stoppen
int pomp_aan = 0;                   // de default waarde van de pomp is uit



/* dit doet de Arduino 1 keer */
void setup() {
  mod.begin(9600);                        // de seriële communicatie zit op 9600
  pinMode(vochtigheidssensorPin, INPUT);  // vochtigheidssensor is een ingang
  pinMode(RE, OUTPUT);                    // RE is een uitgang
  pinMode(DE, OUTPUT);                    // DE is een uitgang
  pinMode(waterpompPin, OUTPUT);          // waterpomp is een uitgang
  digitalWrite(DE, LOW);                  // zet DE laag
  digitalWrite(RE, LOW);                  // zet RE laag
  lcd.init();                             // initialiseer het LCD-scherm
  lcd.backlight();                        // zet de achtergrondverlichting van het LCD-scherm aan
}

/* dit blijft de arduino herhalen */
void loop() {
  vochtigheidssensorValue = analogRead(vochtigheidssensorPin);       // lees de sensorwaarde van de sensorPin
  vochtpercentage = map(vochtigheidssensorValue, 826, 454, 0, 100);  // map de sensorwaarde naar een bereik van 0 tot 100

  byte val1, val2, val3;  // maak 3 nieuwe array's aan voor waarden van NPK-sensor
  val1 = nitrogen();      // zet de waarden van nitrogen in val1
  delay(250);             // wacht 250 milliseconden
  val2 = phosphorous();   // zet de waarden van phosphorous in val2
  delay(250);             // wacht 250 milliseconden
  val3 = potassium();     // zet de waarden van potassium in val3
  delay(250);             // wacht 250 milliseconden

  if ((val1 == 255) & (val2 == 255) & (val3 == 255)) {  // als de NPK-waarde het miximum zijn, dan ...
    fout = 1;                                           // maak fout gelijk aan 1
    lcd.clear();                                        // maak het LCD-scherm leeg
    lcd.setCursor(5, 1);                                // zet de cursor van het LCD-scherm op positie (5, 1)
    lcd.print("loading");                               // toon "loading" op het LCD-scherm
    lcd.setCursor(12, 1);                               // zet de cursor van het LCD-scherm op positie (12, 1)
    lcd.print(".");                                     // toon "." op het LCD-scherm
    delay(500);                                         // wacht 500 milliseconden
    lcd.print(".");                                     // toon "." op het LCD-scherm
    delay(500);                                         // wacht 500 milliseconden
    lcd.print(".");                                     // toon "." op het LCD-scherm
    delay(500);                                         // wacht 500 milliseconden

  } else {     // ander, dan ...
    fout = 0;  // maak fout gelijk aan 0
  }
  if (fout == 0) {                       // als fout gelijk is aan 0, dan ...
    lcd.clear();                         // maak het LCD-scherm leeg
    lcd.setCursor(0, 0);                 // zet de cursor van het LCD-scherm op positie (0, 0)
    lcd.print("vochtigheid : ");         // toon "vochtigheid : " op het LCD-scherm
    lcd.setCursor(14, 0);                // zet de cursor van het LCD-scherm op positie (14, 0)
    lcd.print(vochtpercentage);          // toon het vochtpercentage op het LCD-scherm
    if (vochtpercentage < 10) {          // als het vochtpercentage lager is als 10, dan ...
      lcd.setCursor(16, 0);              // zet de cursor van het LCD-scherm op positie (16, 0)
      lcd.print("%");                    // toon "%" op het LCD-scherm
    } else if (vochtpercentage < 100) {  // anders als het vochtpercentage lager is als 100, dan ...
      lcd.setCursor(17, 0);              // zet de cursor van het LCD-scherm op positie (17, 0)
      lcd.print("%");                    // toon "%" op het LCD-scherm
    } else if (vochtpercentage = 100) {  // anders als het vochtpercentage gelijk is aan 100, dan ...
      lcd.setCursor(18, 0);              // zet de cursor van het LCD-scherm op positie (18, 0)
      lcd.print("%");                    // toon "%" op het LCD-scherm
    }

    lcd.setCursor(0, 1);       // zet de cursor van het LCD-scherm op positie (0, 1)
    lcd.print("stikstof : ");  // toon "stikstof : " op het LCD-scherm
    lcd.setCursor(11, 1);      // zet de cursor van het LCD-scherm op positie (11, 1)
    lcd.print(val1);           // toon de waarde van val1 op het LCD-scherm
    if (val1 < 10) {           // als de waarde van val1 lager is als 10, dan ...
      lcd.setCursor(13, 1);    // zet de cursor van het LCD-scherm op positie (13, 1)
      lcd.print("mg/kg");      // toon "mg/kg" op het LCD-scherm
    } else if (val1 < 100) {   // anders als de waarde van val1 lager is als 100, dan ...
      lcd.setCursor(14, 1);    // zet de cursor van het LCD-scherm op positie (14, 1)
      lcd.print("mg/kg");      // toon "mg/kg" op het LCD-scherm
    } else if (val1 >= 100) {  // anders als de waarde van val1 hoger is als of gelijk is aan 100, dan ...
      lcd.setCursor(15, 1);    // zet de cursor van het LCD-scherm op positie (15, 1)
      lcd.print("mg/kg");      // toon "mg/kg" op het LCD-scherm
    }

    lcd.setCursor(0, 2);       // zet de cursor van het LCD-scherm op positie (0, 2)
    lcd.print("fosfaat : ");   // toon "fosfaat : " op het LCD-scherm
    lcd.setCursor(11, 2);      // zet de cursor van het LCD-scherm op positie (11, 2)
    lcd.print(val2);           // toon de waarde van val2 op het LCD-scherm
    if (val2 < 10) {           // als de waarde van val2 lager is als 10, dan ...
      lcd.setCursor(13, 2);    // zet de cursor van het LCD-scherm op positie (13, 2)
      lcd.print("mg/kg");      // toon "mg/kg" op het LCD-scherm
    } else if (val2 < 100) {   // anders als de waarde van val2 lager is als 100, dan ...
      lcd.setCursor(14, 2);    // zet de cursor van het LCD-scherm op positie (14, 2)
      lcd.print("mg/kg");      // toon "mg/kg" op het LCD-scherm
    } else if (val2 >= 100) {  // anders als de waarde van val2 hoger is als of gelijk is aan 100, dan ...
      lcd.setCursor(15, 2);    // zet de cursor van het LCD-scherm op positie (15, 2)
      lcd.print("mg/kg");      // toon "mg/kg" op het LCD-scherm
    }

    lcd.setCursor(0, 3);       // zet de cursor van het LCD-scherm op positie (0, 3)
    lcd.print("kalium : ");    // toon "kalium : " op het LCD-scherm
    lcd.setCursor(11, 3);      // zet de cursor van het LCD-scherm op positie (11, 3)
    lcd.print(val3);           // toon de waarde van val3 op het LCD-scherm
    if (val3 < 10) {           // als de waarde van val3 lager is als 10, dan ...
      lcd.setCursor(13, 3);    // zet de cursor van het LCD-scherm op positie (13, 3)
      lcd.print("mg/kg");      // toon "mg/kg" op het LCD-scherm
    } else if (val3 < 100) {   // anders als de waarde van val3 lager is als 100, dan ...
      lcd.setCursor(14, 3);    // zet de cursor van het LCD-scherm op positie (14, 3)
      lcd.print("mg/kg");      // toon "mg/kg" op het LCD-scherm
    } else if (val3 >= 100) {  // anders als de waarde van val3 hoger is als of gelijk is aan 100, dan ...
      lcd.setCursor(15, 3);    // zet de cursor van het LCD-scherm op positie (15, 2)
      lcd.print("mg/kg");      // toon "mg/kg" op het LCD-scherm
    }
  }
  if ((vochtpercentage < MinimunNiveau) && pomp_aan == 0) {  // als het vochtpercentage lager is als het minimum en pomp_aan is 0, dan ...
    lcd.noDisplay();                                         // zet lcd-scherm uit
    lcd.noBacklight();                                       // zet achtergrondlicht uit
    pomp_aan = 1;                                            // zet pomp_aan op 1
    starttimer = millis();                                   // starttimer is de huidige tijd
    digitalWrite(waterpompPin, HIGH);                        // zet pomp aan
  }
  if (pomp_aan == 1 && millis() - starttimer > ingestelde_water_tijd) {  // als pomp_aan 1 is en als de huidige tijd min de starttijd groter is als de ingestelde tijd, dan ...
    digitalWrite(waterpompPin, LOW);                                     // zet pomp uit
    lcd.display();                                                       // zet lcd-scherm aan
    lcd.backlight();                                                     // zet achtergrondlicht aan
    lcd.clear();                                                         // maak het LCD-scherm leeg
    lcd.setCursor(2, 1);                                                 // zet de cursor van het LCD-scherm op positie (2, 1)
    lcd.print("controleer tank");                                        // toon "controleer tank" op het LCD-scherm
    lcd.setCursor(2, 2);                                                 // zet de cursor van het LCD-scherm op positie (2, 2)
    lcd.print("of controleer voor");                                     // toon "of controleer voor" op het LCD-scherm
    lcd.setCursor(2, 3);                                                 // zet de cursor van het LCD-scherm op positie (2, 3)
    lcd.print("lekkage");                                                // toon "lekkage" op het LCD-scherm
  }
  if (vochtpercentage > (MinimunNiveau + 20)) {  // als het vochtpercentage groter is als het minimum plus 20, dan ...
    digitalWrite(waterpompPin, LOW);             // zet pomp uit
    lcd.display();                               // zet lcd-scherm aan
    lcd.backlight();                             // zet achtergrondlicht aan
    pomp_aan = 0;                                // zet pomp_aan op 0
  }
}

byte nitrogen() {                                                   // maak een functie genaamd nitrogen die een byte-waarde retourneert
  mod.flushInput();                                                 // Leeg de inputbuffer van de AltSoftSerial
  digitalWrite(DE, HIGH);                                           // zet DE hoog
  digitalWrite(RE, HIGH);                                           // zet RE hoog
  delay(1);                                                         // Wacht 1 milliseconde
  for (uint8_t i = 0; i < sizeof(nitro); i++) mod.write(nitro[i]);  // Stuur de commando's voor stikstofmeting naar de NPK-sensor
  mod.flush();                                                      // Wacht tot alle data verzonden is
  digitalWrite(DE, LOW);                                            // zet DE laag
  digitalWrite(RE, LOW);                                            // zet RE laag
  delay(200);                                                       // Wacht 200 milliseconden
  for (byte i = 0; i < 7; i++) {                                    // overloop dit 7 keer
    values[i] = mod.read();                                         // Lees wat je terug krijgt van de NPK-sensor en sla ze op in de array
  }
  return values[4];  // Geef de waarde op positie 4 terug
}

byte phosphorous() {                                              // maak een functie genaamd phosphorous die een byte-waarde retourneert
  mod.flushInput();                                               // Leeg de inputbuffer van de AltSoftSerial
  digitalWrite(DE, HIGH);                                         // zet DE hoog
  digitalWrite(RE, HIGH);                                         // zet RE hoog
  delay(1);                                                       // Wacht 1 milliseconde
  for (uint8_t i = 0; i < sizeof(phos); i++) mod.write(phos[i]);  // Stuur de commando's voor fosfaatmeting naar de NPK-sensor
  mod.flush();                                                    // Wacht tot alle data verzonden is
  digitalWrite(DE, LOW);                                          // zet DE laag
  digitalWrite(RE, LOW);                                          // zet RE laag
  delay(200);                                                     // Wacht 200 milliseconden
  for (byte i = 0; i < 7; i++) {                                  // overloop dit 7 keer
    values[i] = mod.read();                                       // Lees wat je terug krijgt van de NPK-sensor en sla ze op in de array
  }
  return values[4];  // Geef de waarde op positie 4 terug
}

byte potassium() {                                                // maak een functie genaamd potassium die een byte-waarde retourneert
  mod.flushInput();                                               // Leeg de inputbuffer van de AltSoftSerial
  digitalWrite(DE, HIGH);                                         // zet DE hoog
  digitalWrite(RE, HIGH);                                         // zet RE hoog
  delay(1);                                                       // Wacht 1 milliseconde
  for (uint8_t i = 0; i < sizeof(pota); i++) mod.write(pota[i]);  // Stuur de commando's voor kaliummeting naar de NPK-sensor
  mod.flush();                                                    // Wacht tot alle data verzonden is
  digitalWrite(DE, LOW);                                          // zet DE laag
  digitalWrite(RE, LOW);                                          // zet RE laag
  delay(200);                                                     // Wacht 200 milliseconden
  for (byte i = 0; i < 7; i++) {                                  // overloop dit 7 keer
    values[i] = mod.read();                                       // Lees wat je terug krijgt van de NPK-sensor en sla ze op in de array
  }
  return values[4];  // Geef de waarde op positie 4 terug
}
