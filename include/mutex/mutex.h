#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>

#ifndef MUTEX_H
#define MUTEX_H

class MutexController {
  public:
    static void take();
    static void give();
    static void create();
};

#endif