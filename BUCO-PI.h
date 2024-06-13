#ifndef BUCO_PI_H
#define BUCO_PI_H

#include <Arduino.h>

class BUCO_PI {
  public:
    BUCO_PI(int pin);
    float readTemperature();
  private:
    int _pin;
};

#endif
