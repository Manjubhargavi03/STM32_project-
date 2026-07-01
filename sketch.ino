#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal.h>

#define SS_PIN 10
#define RST_PIN 2

MFRC522 rfid(SS_PIN, RST_PIN);

// LCD Pins
LiquidCrystal lcd(8, 9, 4, 5, A0, 7);

const int buzzerPin = 3;
const int ledPin = 6;

void setup() {

  Serial.begin(115200);

  SPI.begin();
  rfid.PCD_Init();

  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  lcd.begin(16,2);

  showWelcome();

  Serial.println("SMART CAMPUS FACULTY LOCATOR");
  Serial.println("Tap RFID Card...");
}

void loop() {

  if (!rfid.PICC_IsNewCardPresent())
    return;

  if (!rfid.PICC_ReadCardSerial())
    return;

  String uid = "";

  for (byte i = 0; i < rfid.uid.size; i++) {

    if (rfid.uid.uidByte[i] < 0x10)
      uid += "0";

    uid += String(rfid.uid.uidByte[i], HEX);
  }

  uid.toUpperCase();

  Serial.print("UID : ");
  Serial.println(uid);

  if (uid == "01020304") {

    facultyDetected("Dr. Smith","Room 402");

  }

  else if (uid == "11223344") {

    facultyDetected("Prof. Davis","Lab 1A");

  }

  else if (uid == "55667788") {

    facultyDetected("Dr. Taylor","Library");

  }

  else if (uid == "AABBCCDD") {

    facultyDetected("Prof. Wilson","Dean Office");

  }

  else {

    invalidCard();

  }

  rfid.PICC_HaltA();
}

void showWelcome() {

  lcd.clear();

  lcd.setCursor(0,0);
  lcd.print("WELCOME TO");

  lcd.setCursor(0,1);
  lcd.print("FACULTY LOCATOR");
}

void facultyDetected(String name,String room){

  digitalWrite(ledPin,HIGH);

  tone(buzzerPin,1000,200);

  lcd.clear();

  lcd.setCursor(0,0);
  lcd.print(name);

  lcd.setCursor(0,1);
  lcd.print(room);

  Serial.println("Faculty Found");
  Serial.println(name);
  Serial.println(room);

  delay(3000);

  digitalWrite(ledPin,LOW);

  showWelcome();

}

void invalidCard(){

  digitalWrite(ledPin,HIGH);

  tone(buzzerPin,400,500);

  lcd.clear();

  lcd.setCursor(0,0);
  lcd.print("Invalid Card");

  Serial.println("Access Denied");

  delay(2000);

  digitalWrite(ledPin,LOW);

  showWelcome();

}