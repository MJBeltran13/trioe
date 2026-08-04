#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

#ifndef TRIOE_MAX_QUEUED_READINGS
#define TRIOE_MAX_QUEUED_READINGS 16
#endif

#ifndef TRIOE_JSON_DOCUMENT_SIZE
#define TRIOE_JSON_DOCUMENT_SIZE 8192
#endif

#ifndef TRIOE_MAX_READING_HANDLERS
#define TRIOE_MAX_READING_HANDLERS 8
#endif

// Classic API retained for backward compatibility with existing TRIOE
// learning-kit sketches.
class Trioe {
 public:
  Trioe();
  bool postData(const char* serverURL, const char* jsonPayload);
  int postDataWithApiKey(const char* serverURL, const char* jsonPayload, const char* apiKey);
};

// Secure Hub API introduced in v3.2.6.
class TrioeClient {
 public:
  enum class ConnectionStatus : uint8_t { Disconnected, Connecting, Connected };
  enum class DeliveryStatus : uint8_t { Idle, Queued, Sending, Delivered, Retrying, Failed };
  using CommandHandler = void (*)(JsonVariantConst command);
  using StateHandler = void (*)(JsonObjectConst stream);
  using NumericReadingHandler = void (*)(float value);
  using TextReadingHandler = void (*)(const char* value);
  using BooleanReadingHandler = void (*)(bool value);

  TrioeClient(const char* endpoint, const char* apiKey);
  bool begin();
  void loop();

  bool addReading(const char* name, float value, const char* unit = nullptr);
  bool addReading(const char* name, int value, const char* unit = nullptr);
  bool addReading(const char* name, bool value, const char* unit = nullptr);
  bool addReading(const char* name, const char* value, const char* unit = nullptr);
  bool publish();
  bool publishBatch();
  bool pollCommands();
  bool fetchState();

  void setCommandEndpoint(const char* endpoint);
  void setCommandHandler(CommandHandler handler);
  void setStateEndpoint(const char* endpoint);
  void setStateHandler(StateHandler handler);
  bool onReading(const char* name, NumericReadingHandler handler);
  bool onText(const char* name, TextReadingHandler handler);
  bool onBoolean(const char* name, BooleanReadingHandler handler);
  void setReportingInterval(uint32_t intervalMs);
  void setCommandPollInterval(uint32_t intervalMs);
  void setStatePollInterval(uint32_t intervalMs);
  void setChangeThreshold(float threshold);
  void setRetryDelays(uint32_t baseDelayMs, uint32_t maxDelayMs);
  void setRequestTimeout(uint16_t timeoutMs);
  void setCACert(const char* certificate);

  ConnectionStatus connectionStatus() const;
  DeliveryStatus deliveryStatus() const;
  bool isConnected() const;
  bool isDeliveryPending() const;
  uint8_t queuedCount() const;
  uint32_t lastAcceptedSequence() const;
  int lastHttpStatus() const;

 private:
  static constexpr size_t kNameLength = 65;
  static constexpr size_t kUnitLength = 51;
  static constexpr size_t kTextLength = 129;
  struct Reading {
    char name[kNameLength];
    char unit[kUnitLength];
    char text[kTextLength];
    double number;
    uint8_t type;
  };
  struct ReadingSubscription {
    char name[kNameLength];
    NumericReadingHandler handler;
  };
  struct TextSubscription {
    char name[kNameLength];
    TextReadingHandler handler;
  };
  struct BooleanSubscription {
    char name[kNameLength];
    BooleanReadingHandler handler;
  };

  bool queueNumeric(const char* name, double value, uint8_t type, const char* unit);
  bool queueText(const char* name, const char* value, uint8_t type, const char* unit);
  bool valueChanged(const Reading& candidate) const;
  void rememberDelivered(const Reading& reading);
  bool postBatch();
  bool shouldRetry(int httpStatus) const;
  void discardPendingBatch();
  void scheduleRetry();
  bool isDue(uint32_t now, uint32_t dueAt) const;
  void copyText(char* destination, size_t destinationSize, const char* source);

  String _endpoint, _commandEndpoint, _stateEndpoint, _apiKey, _sessionId;
  WiFiClientSecure _secureClient;
  HTTPClient _http;
  Reading _queue[TRIOE_MAX_QUEUED_READINGS];
  Reading _delivered[TRIOE_MAX_QUEUED_READINGS];
  ReadingSubscription _readingSubscriptions[TRIOE_MAX_READING_HANDLERS];
  TextSubscription _textSubscriptions[TRIOE_MAX_READING_HANDLERS];
  BooleanSubscription _booleanSubscriptions[TRIOE_MAX_READING_HANDLERS];
  uint8_t _queueCount = 0, _deliveredCount = 0, _retryAttempt = 0;
  uint8_t _readingSubscriptionCount = 0;
  uint8_t _textSubscriptionCount = 0, _booleanSubscriptionCount = 0;
  uint32_t _sequence = 0, _pendingSequence = 0, _lastAcceptedSequence = 0;
  uint32_t _reportingIntervalMs = 30000, _commandPollIntervalMs = 60000;
  uint32_t _statePollIntervalMs = 5000;
  uint32_t _nextPublishAt = 0, _nextCommandPollAt = 0, _nextStatePollAt = 0, _retryAt = 0;
  uint32_t _retryBaseDelayMs = 1000, _retryMaxDelayMs = 60000;
  uint16_t _requestTimeoutMs = 10000;
  float _changeThreshold = 0.0f;
  int _lastHttpStatus = 0;
  bool _started = false, _certificateConfigured = false;
  ConnectionStatus _connectionStatus = ConnectionStatus::Disconnected;
  DeliveryStatus _deliveryStatus = DeliveryStatus::Idle;
  CommandHandler _commandHandler = nullptr;
  StateHandler _stateHandler = nullptr;
};
