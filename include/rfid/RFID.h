#include <MFRC522.h>
#include <string>
#include "utils/Utils.h"
#include "log/Log.h"

#ifndef RFID_H
#define RFID_H

class RFID: public Log {
  public:
    RFID();
    std::string read();
    void init();
};

#endif