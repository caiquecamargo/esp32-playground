#include "rfid/RFID.h"

RFID::RFID(MFRC522 mfrc522) : mfrc522(mfrc522) {}

int RFID::read() {
  if (!mfrc522.PICC_IsNewCardPresent())
    return 0;

  if (!mfrc522.PICC_ReadCardSerial())
    return 0;

  byte readCard[4];

  for (uint8_t i = 0; i < 4; i++) {
    readCard[i] = mfrc522.uid.uidByte[i];
  }

  cardId = Utils::make_hex_string(std::begin(readCard), std::end(readCard));
    
  mfrc522.PICC_HaltA();

  return 1;
}