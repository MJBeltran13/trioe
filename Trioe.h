#ifndef Trioe_H
#define Trioe_H

#include <Arduino.h>

class Trioe {
  public:
    BUCO_PI(int pin);
    float readTemperature();
  private:
    int _pin;
};

#endif
