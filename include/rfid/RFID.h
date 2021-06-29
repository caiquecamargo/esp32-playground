#include <MFRC522.h>
#include <string>
#include "utils/Utils.h"

#ifndef RFID_H
#define RFID_H

class RFID {
  private:
    MFRC522 mfrc522;
  public:
    std::string cardId;
    RFID(MFRC522 mfrc522);
    int read();
};

#endif