#include "rfid/RFID.h"

#define SS_PIN 21
#define RST_PIN 22

MFRC522 mfrc522(SS_PIN, RST_PIN);

RFID::RFID() : Log("RFID")  {}

std::string RFID::read() {
  if (!mfrc522.PICC_IsNewCardPresent())
    return "";

  if (!mfrc522.PICC_ReadCardSerial())
    return "";

  byte readCard[4];

  for (uint8_t i = 0; i < 4; i++) {
    readCard[i] = mfrc522.uid.uidByte[i];
  }

  std::string id = Utils::makeHexString(std::begin(readCard), std::end(readCard));

  log("Card read with ID -> " + id);
    
  mfrc522.PICC_HaltA();

  return id;
}

void RFID::init() {
  mfrc522.PCD_Init();
}