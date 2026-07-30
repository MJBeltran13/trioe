#include "Trioe.h"
#include <esp_system.h>
#include <math.h>

Trioe::Trioe() {}

bool Trioe::postData(const char* serverURL, const char* jsonPayload) {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi not connected");
    return false;
  }

  HTTPClient http;
  http.begin(serverURL);
  http.addHeader("Content-Type", "application/json");
  const int httpResponseCode = http.POST(jsonPayload);
  http.end();
  return httpResponseCode >= 200 && httpResponseCode < 300;
}

int Trioe::postDataWithApiKey(
    const char* serverURL,
    const char* jsonPayload,
    const char* apiKey) {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi not connected");
    return -1;
  }

  HTTPClient http;
  http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
  http.setTimeout(10000);
  http.begin(serverURL);
  http.addHeader("Content-Type", "application/json");
  http.addHeader("Authorization", String("Bearer ") + apiKey);
  const int httpResponseCode = http.POST(jsonPayload);
  http.end();
  return httpResponseCode;
}

namespace { constexpr uint8_t kNumber = 0, kInteger = 1, kBoolean = 2, kString = 3; }

TrioeClient::TrioeClient(const char* endpoint, const char* apiKey)
    : _endpoint(endpoint ? endpoint : ""), _apiKey(apiKey ? apiKey : "") {}

bool TrioeClient::begin() {
  if (_endpoint.length() == 0 || _apiKey.length() == 0 || !_certificateConfigured) return false;
  char session[33];
  snprintf(session, sizeof(session), "%08lx%08lx%08lx%08lx",
           static_cast<unsigned long>(esp_random()),
           static_cast<unsigned long>(esp_random()),
           static_cast<unsigned long>(esp_random()),
           static_cast<unsigned long>(esp_random()));
  _sessionId = session;
  _http.setReuse(true); _http.setTimeout(_requestTimeoutMs);
  _started = true; _nextPublishAt = _nextCommandPollAt = _nextStatePollAt = millis(); return true;
}
void TrioeClient::loop() {
  if (!_started) return;
  _connectionStatus = WiFi.status() == WL_CONNECTED ? ConnectionStatus::Connected : ConnectionStatus::Disconnected;
  if (_connectionStatus != ConnectionStatus::Connected) return;
  const uint32_t now = millis();
  if (_queueCount && isDue(now, _nextPublishAt) && isDue(now, _retryAt)) publishBatch();
  if (_commandHandler && _commandEndpoint.length() && isDue(now, _nextCommandPollAt)) pollCommands();
  if ((_stateHandler || _readingSubscriptionCount || _textSubscriptionCount || _booleanSubscriptionCount) &&
      _stateEndpoint.length() && isDue(now, _nextStatePollAt)) fetchState();
}
bool TrioeClient::addReading(const char* name, float value, const char* unit) { return queueNumeric(name, value, kNumber, unit); }
bool TrioeClient::addReading(const char* name, int value, const char* unit) { return queueNumeric(name, value, kInteger, unit); }
bool TrioeClient::addReading(const char* name, bool value, const char* unit) { return queueText(name, value ? "true" : "false", kBoolean, unit); }
bool TrioeClient::addReading(const char* name, const char* value, const char* unit) { return queueText(name, value, kString, unit); }
bool TrioeClient::publish() { return publishBatch(); }
bool TrioeClient::publishBatch() {
  if (!_started || !_queueCount) return false;
  if (WiFi.status() != WL_CONNECTED) { _connectionStatus = ConnectionStatus::Disconnected; _deliveryStatus = DeliveryStatus::Queued; return false; }
  return isDue(millis(), _retryAt) ? postBatch() : false;
}
bool TrioeClient::pollCommands() {
  if (!_started || !_commandHandler || !_commandEndpoint.length() || WiFi.status() != WL_CONNECTED) return false;
  _nextCommandPollAt = millis() + _commandPollIntervalMs;
  if (!_http.begin(_secureClient, _commandEndpoint)) return false;
  _http.setReuse(true); _http.addHeader("Authorization", "Bearer " + _apiKey); _lastHttpStatus = _http.GET();
  if (_lastHttpStatus < 200 || _lastHttpStatus >= 300) { _http.end(); return false; }
  StaticJsonDocument<TRIOE_JSON_DOCUMENT_SIZE> doc;
  const DeserializationError error = deserializeJson(doc, _http.getStream()); _http.end();
  if (error) return false;
  for (JsonVariantConst command : doc["commands"].as<JsonArrayConst>()) _commandHandler(command);
  return true;
}
bool TrioeClient::fetchState() {
  if (!_started ||
      (!_stateHandler && !_readingSubscriptionCount && !_textSubscriptionCount && !_booleanSubscriptionCount) ||
      !_stateEndpoint.length() || WiFi.status() != WL_CONNECTED) return false;
  _nextStatePollAt = millis() + _statePollIntervalMs;
  if (!_http.begin(_secureClient, _stateEndpoint)) return false;
  _http.setReuse(true); _http.addHeader("Authorization", "Bearer " + _apiKey); _lastHttpStatus = _http.GET();
  if (_lastHttpStatus < 200 || _lastHttpStatus >= 300) { _http.end(); return false; }
  StaticJsonDocument<TRIOE_JSON_DOCUMENT_SIZE> doc;
  const DeserializationError error = deserializeJson(doc, _http.getStream()); _http.end();
  if (error || !doc["data"].is<JsonArray>()) return false;
  for (JsonObjectConst stream : doc["data"].as<JsonArrayConst>()) {
    if (_stateHandler) _stateHandler(stream);
    const char* name = stream["name"] | "";
    JsonVariantConst value = stream["current_value"];
    const char* type = stream["type"] | "";
    if (!name[0]) continue;
    for (uint8_t i = 0; i < _readingSubscriptionCount; ++i) {
      if ((!strcmp(type, "number") || !strcmp(type, "integer")) &&
          value.is<double>() && !strcmp(name, _readingSubscriptions[i].name) && _readingSubscriptions[i].handler) {
        _readingSubscriptions[i].handler(value.as<float>());
      }
    }
    for (uint8_t i = 0; i < _textSubscriptionCount; ++i) {
      if (!strcmp(type, "string") && value.is<const char*>() &&
          !strcmp(name, _textSubscriptions[i].name) && _textSubscriptions[i].handler) {
        _textSubscriptions[i].handler(value.as<const char*>());
      }
    }
    for (uint8_t i = 0; i < _booleanSubscriptionCount; ++i) {
      if (!strcmp(type, "boolean") && !strcmp(name, _booleanSubscriptions[i].name) &&
          _booleanSubscriptions[i].handler) {
        if (value.is<bool>()) {
          _booleanSubscriptions[i].handler(value.as<bool>());
        } else if (value.is<const char*>()) {
          const char* text = value.as<const char*>();
          if (!strcasecmp(text, "true") || !strcmp(text, "1")) _booleanSubscriptions[i].handler(true);
          else if (!strcasecmp(text, "false") || !strcmp(text, "0")) _booleanSubscriptions[i].handler(false);
        }
      }
    }
  }
  return true;
}
void TrioeClient::setCommandEndpoint(const char* endpoint) { _commandEndpoint = endpoint ? endpoint : ""; }
void TrioeClient::setCommandHandler(CommandHandler handler) { _commandHandler = handler; }
void TrioeClient::setStateEndpoint(const char* endpoint) { _stateEndpoint = endpoint ? endpoint : ""; }
void TrioeClient::setStateHandler(StateHandler handler) { _stateHandler = handler; }
bool TrioeClient::onReading(const char* name, NumericReadingHandler handler) {
  if (!name || !name[0] || !handler) return false;
  for (uint8_t i = 0; i < _readingSubscriptionCount; ++i) {
    if (!strcmp(name, _readingSubscriptions[i].name)) {
      _readingSubscriptions[i].handler = handler;
      return true;
    }
  }
  if (_readingSubscriptionCount >= TRIOE_MAX_READING_HANDLERS) return false;
  ReadingSubscription& subscription = _readingSubscriptions[_readingSubscriptionCount++];
  copyText(subscription.name, sizeof(subscription.name), name);
  subscription.handler = handler;
  return true;
}
bool TrioeClient::onText(const char* name, TextReadingHandler handler) {
  if (!name || !name[0] || !handler) return false;
  for (uint8_t i = 0; i < _textSubscriptionCount; ++i) {
    if (!strcmp(name, _textSubscriptions[i].name)) {
      _textSubscriptions[i].handler = handler;
      return true;
    }
  }
  if (_textSubscriptionCount >= TRIOE_MAX_READING_HANDLERS) return false;
  TextSubscription& subscription = _textSubscriptions[_textSubscriptionCount++];
  copyText(subscription.name, sizeof(subscription.name), name);
  subscription.handler = handler;
  return true;
}
bool TrioeClient::onBoolean(const char* name, BooleanReadingHandler handler) {
  if (!name || !name[0] || !handler) return false;
  for (uint8_t i = 0; i < _booleanSubscriptionCount; ++i) {
    if (!strcmp(name, _booleanSubscriptions[i].name)) {
      _booleanSubscriptions[i].handler = handler;
      return true;
    }
  }
  if (_booleanSubscriptionCount >= TRIOE_MAX_READING_HANDLERS) return false;
  BooleanSubscription& subscription = _booleanSubscriptions[_booleanSubscriptionCount++];
  copyText(subscription.name, sizeof(subscription.name), name);
  subscription.handler = handler;
  return true;
}
void TrioeClient::setReportingInterval(uint32_t ms) { _reportingIntervalMs = ms; }
void TrioeClient::setCommandPollInterval(uint32_t ms) { _commandPollIntervalMs = ms; }
void TrioeClient::setStatePollInterval(uint32_t ms) { _statePollIntervalMs = ms < 1000 ? 1000 : ms; }
void TrioeClient::setChangeThreshold(float value) { _changeThreshold = value < 0 ? 0 : value; }
void TrioeClient::setRetryDelays(uint32_t base, uint32_t maximum) { _retryBaseDelayMs = base; _retryMaxDelayMs = maximum < base ? base : maximum; }
void TrioeClient::setRequestTimeout(uint16_t timeoutMs) { _requestTimeoutMs = timeoutMs < 1000 ? 1000 : timeoutMs; _http.setTimeout(_requestTimeoutMs); }
void TrioeClient::setCACert(const char* certificate) { if (certificate && certificate[0]) { _secureClient.setCACert(certificate); _certificateConfigured = true; } }
TrioeClient::ConnectionStatus TrioeClient::connectionStatus() const { return _connectionStatus; }
TrioeClient::DeliveryStatus TrioeClient::deliveryStatus() const { return _deliveryStatus; }
bool TrioeClient::isConnected() const { return _connectionStatus == ConnectionStatus::Connected; }
bool TrioeClient::isDeliveryPending() const { return _queueCount > 0; }
uint8_t TrioeClient::queuedCount() const { return _queueCount; }
uint32_t TrioeClient::lastAcceptedSequence() const { return _lastAcceptedSequence; }
int TrioeClient::lastHttpStatus() const { return _lastHttpStatus; }
bool TrioeClient::queueNumeric(const char* name, double value, uint8_t type, const char* unit) {
  if (!name || !isfinite(value) || _pendingSequence != 0) return false; Reading reading{}; copyText(reading.name, sizeof(reading.name), name); copyText(reading.unit, sizeof(reading.unit), unit); reading.number = value; reading.type = type;
  if (!valueChanged(reading)) return false;
  for (uint8_t i = 0; i < _queueCount; ++i) if (!strcmp(_queue[i].name, reading.name)) { _queue[i] = reading; _deliveryStatus = DeliveryStatus::Queued; return true; }
  if (_queueCount >= TRIOE_MAX_QUEUED_READINGS) { _deliveryStatus = DeliveryStatus::Failed; return false; }
  _queue[_queueCount++] = reading; _deliveryStatus = DeliveryStatus::Queued; return true;
}
bool TrioeClient::queueText(const char* name, const char* value, uint8_t type, const char* unit) {
  if (!name || !value || _pendingSequence != 0) return false; Reading reading{}; copyText(reading.name, sizeof(reading.name), name); copyText(reading.unit, sizeof(reading.unit), unit); copyText(reading.text, sizeof(reading.text), value); reading.type = type;
  if (!valueChanged(reading)) return false;
  for (uint8_t i = 0; i < _queueCount; ++i) if (!strcmp(_queue[i].name, reading.name)) { _queue[i] = reading; _deliveryStatus = DeliveryStatus::Queued; return true; }
  if (_queueCount >= TRIOE_MAX_QUEUED_READINGS) { _deliveryStatus = DeliveryStatus::Failed; return false; }
  _queue[_queueCount++] = reading; _deliveryStatus = DeliveryStatus::Queued; return true;
}
bool TrioeClient::valueChanged(const Reading& candidate) const {
  for (uint8_t i = 0; i < _deliveredCount; ++i) { const Reading& old = _delivered[i]; if (strcmp(old.name, candidate.name) || old.type != candidate.type) continue; if (candidate.type <= kInteger) { const double difference = fabs(candidate.number - old.number); return difference > 0 && (_changeThreshold == 0 || difference >= _changeThreshold); } return strcmp(candidate.text, old.text) != 0; }
  return true;
}
void TrioeClient::rememberDelivered(const Reading& reading) {
  for (uint8_t i = 0; i < _deliveredCount; ++i) if (!strcmp(_delivered[i].name, reading.name)) { _delivered[i] = reading; return; }
  if (_deliveredCount < TRIOE_MAX_QUEUED_READINGS) _delivered[_deliveredCount++] = reading;
}
bool TrioeClient::postBatch() {
  _deliveryStatus = DeliveryStatus::Sending; if (_pendingSequence == 0) _pendingSequence = ++_sequence; StaticJsonDocument<TRIOE_JSON_DOCUMENT_SIZE> doc; doc["session_id"] = _sessionId; doc["sequence"] = _pendingSequence; JsonArray values = doc.createNestedArray("readings");
  for (uint8_t i = 0; i < _queueCount; ++i) { const Reading& r = _queue[i]; JsonObject item = values.createNestedObject(); item["name"] = r.name; item["unit"] = r.unit; if (r.type == kNumber) { item["type"] = "number"; item["value"] = r.number; } else if (r.type == kInteger) { item["type"] = "integer"; item["value"] = static_cast<int>(r.number); } else if (r.type == kBoolean) { item["type"] = "boolean"; item["value"] = !strcmp(r.text, "true"); } else { item["type"] = "string"; item["value"] = r.text; } }
  if (doc.overflowed()) { _deliveryStatus = DeliveryStatus::Failed; return false; }
  String payload; serializeJson(doc, payload); if (!_http.begin(_secureClient, _endpoint)) { scheduleRetry(); return false; }
  _http.setReuse(true); _http.addHeader("Content-Type", "application/json"); _http.addHeader("Authorization", "Bearer " + _apiKey); _lastHttpStatus = _http.POST(payload); _http.end();
  if (_lastHttpStatus < 200 || _lastHttpStatus >= 300) { if (shouldRetry(_lastHttpStatus)) scheduleRetry(); else discardPendingBatch(); return false; }
  for (uint8_t i = 0; i < _queueCount; ++i) rememberDelivered(_queue[i]); _queueCount = 0; _lastAcceptedSequence = _pendingSequence; _pendingSequence = 0; _retryAttempt = 0; _retryAt = 0; _nextPublishAt = millis() + _reportingIntervalMs; _deliveryStatus = DeliveryStatus::Delivered; return true;
}
bool TrioeClient::shouldRetry(int status) const { return status <= 0 || status == 408 || status == 425 || status == 429 || status >= 500; }
void TrioeClient::discardPendingBatch() { _queueCount = 0; _pendingSequence = 0; _retryAttempt = 0; _retryAt = 0; _nextPublishAt = millis() + _reportingIntervalMs; _deliveryStatus = DeliveryStatus::Failed; }
void TrioeClient::scheduleRetry() { const uint8_t exponent = _retryAttempt > 6 ? 6 : _retryAttempt++; uint32_t delayMs = _retryBaseDelayMs << exponent; if (delayMs > _retryMaxDelayMs || delayMs < _retryBaseDelayMs) delayMs = _retryMaxDelayMs; _retryAt = millis() + delayMs + (delayMs / 4 ? random(delayMs / 4) : 0); _deliveryStatus = DeliveryStatus::Retrying; }
bool TrioeClient::isDue(uint32_t now, uint32_t dueAt) const { return static_cast<int32_t>(now - dueAt) >= 0; }
void TrioeClient::copyText(char* target, size_t size, const char* source) { if (!source) { target[0] = '\0'; return; } strlcpy(target, source, size); }
