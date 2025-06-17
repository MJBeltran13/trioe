#ifndef Trioe_H
#define Trioe_H

#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>

class Trioe
{
public:
  Trioe();
  bool postData(const char *serverURL, const char *jsonPayload);
  int postDataWithApiKey(const char *serverURL, const char *jsonPayload, const char *apiKey);

private:
};

#endif
