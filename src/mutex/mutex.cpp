#include "mutex/mutex.h"

SemaphoreHandle_t mutex;
bool isCreated = false;

void MutexController::create() {
  if (!isCreated) {
    mutex = xSemaphoreCreateMutex();
    isCreated = true;
  }
}

void MutexController::take() {
  xSemaphoreTake(mutex, portMAX_DELAY);
}

void MutexController::give() {
  xSemaphoreGive(mutex);
}