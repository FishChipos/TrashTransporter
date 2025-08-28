#include "api.hpp"

#include <optional>

// ServerLog implementations.
ServerLog::ServerLog(String userContent) {
    timestamp = std::time(NULL);
    content = userContent;
}

inline bool ServerLog::operator==(ServerLog &other) {
    if (timestamp == other.timestamp && content == other.content) {
        return true;
    }
    else {
        return false;
    }
}

inline bool ServerLog::operator!=(ServerLog &other) {
    return !(*this == other);
}

// APIServer implementations.
void APIServer::enableLogging(bool on) {
    isLoggingEnabled = on;
}

void APIServer::registerAPIEndpoints() {
    if (isLoggingEnabled) Serial.println(F("Registering API endpoints..."));

    for (const APIEndpoint &endpoint : endpoints) {
        webServer->on(endpoint.path.c_str(), endpoint.method, endpoint.requestHandler, endpoint.uploadHandler, endpoint.bodyHandler);

        if (isLoggingEnabled) {
            Serial.print(endpoint.path);
            Serial.print(F(" "));

            switch (endpoint.method) {
                case HTTP_GET:
                    Serial.print(F("GET"));
                    break;
                case HTTP_POST:
                    Serial.print(F("POST"));
                    break;
                case HTTP_PATCH:
                    Serial.print(F("PATCH"));
                    break;
                case HTTP_OPTIONS:
                    Serial.print(F("OPTIONS"));
                    break;
                default:
                    Serial.print(F("UNKNOWN METHOD"));
                    break;
            }

            Serial.print(F(" "));
            Serial.print(endpoint.description);
            
            if (endpoint.parameters.size() > 0) {
                Serial.println();
                for (const APIEndpointParameter &parameter : endpoint.parameters) {
                    Serial.print("    [");
                    Serial.print(parameter.key);
                    Serial.print("] ");
                    Serial.print(parameter.description);
                    Serial.println();
                }
            }
            else {
                Serial.println();
            }
        }
    }
}

APIServer::APIServer(const int port, Settings *userSettings, uint8_t *userCameraOutputBuffer, size_t userCameraOutputBufferSize) {
    settings = userSettings;
    cameraOutputBuffer = userCameraOutputBuffer;
    cameraOutputBufferSize = userCameraOutputBufferSize;

    webServer = new AsyncWebServer(port);

    // Add CORS middleware to make the API accessible to other hosts.
    webServer->addMiddleware(new AsyncCorsMiddleware());

    // Define API request handlers.
    webServer->onNotFound([this](AsyncWebServerRequest *request){notFound(request);});

    endpoints.push_back({
        F("/"), 
        HTTP_GET, 
        F("Root path."),
        [this](AsyncWebServerRequest *request) {
            getRoot(request);
        }
    });
    endpoints.push_back({
        F("/logs"), 
        HTTP_GET, 
        F("Server logs."),
        [this](AsyncWebServerRequest *request) {
            getLogs(request);
        }
    });
    endpoints.push_back({
        F("/output/raw"), 
        HTTP_GET, 
        F("Raw image output."),
        [this](AsyncWebServerRequest *request) {
            getOutputRaw(request);
        }
    });
    endpoints.push_back({
        F("/output/marked"), 
        HTTP_GET, 
        F("Marked image output."),
        [this](AsyncWebServerRequest *request) {
            getOutputMarked(request);
        }
    });
    endpoints.push_back({
        F("/settings/manualcontrol"), 
        HTTP_PATCH, 
        F("Set manual control for the robot."),
        [this](AsyncWebServerRequest *request) {
            setManualControl(request);
        },
        nullptr,
        nullptr,
        {
            {F("value"), F("Set to true for manual control, false otherwise.")}
        }
    });
    endpoints.push_back({
        F("/controls/move"),
        HTTP_PATCH,
        F("Move the robot."),
        [this](AsyncWebServerRequest *request) {
            move(request);
        },
        nullptr,
        nullptr,
        {
            {F("direction"), F("Direction of the desired movement. Can be forward, back, left, right, or brake.")},
            {F("on"), F("Whether to move in that direction.")}
        }
    });
}

void APIServer::begin() {
    registerAPIEndpoints();

    webServer->begin();
}

// You better write your Booleans in lower case.
static inline bool stringIsBool(String string) {
    if (string == "true" || string == "false") {
        return true;
    }
    return false;
}

static inline bool stringToBool(String string) {
    if (string == "true") {
        return true;
    }
    else if (string == "false") {
        return false;
    }

    return {};
}

void APIServer::getRoot(AsyncWebServerRequest *request) {
    request->send(200, F("text/html"), F("<!DOCTYPE html><html><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\"></head><body><h1>Hey there!</h1></body></html>"));
}

void APIServer::getLogs(AsyncWebServerRequest *request) {
    // Holy mother of response formatting.
    String responseBody;

    // Initial reservation to prevent too many memory reallocations.
    responseBody.reserve(2048);

    responseBody += F("{\n\"count\": ");
    responseBody += String(logs.size());
    responseBody += F(",\n\"logs\": [\n");
    
    for (ServerLog log : logs) {
        std::tm *calendarTimestamp = std::localtime(&(log.timestamp));

        responseBody += F("{\n\"timestamp\": {\n");
        responseBody += F("\"hours\": ");
        responseBody += String(calendarTimestamp->tm_hour);
        responseBody += F(",\n\"minutes\": ");
        responseBody += String(calendarTimestamp->tm_min);
        responseBody += F(",\n\"seconds\": ");
        responseBody += String(calendarTimestamp->tm_sec);
        responseBody += F("\n},\n\"content\": \"");
        responseBody += log.content;
        responseBody += F("\"\n}");
        

        if (log != logs.back()) {responseBody += ", ";}
    }

    responseBody += F("\n]\n}");

    request->send(200, F("text/json"), responseBody);
}

void APIServer::getOutputRaw(AsyncWebServerRequest *request) {
    String responseBody(cameraOutputBuffer, cameraOutputBufferSize);
    AsyncWebServerResponse *response = request->beginResponse(200, F("image/jpeg"), responseBody);
    response->addHeader("Cache-Control", "max-age=0, must-revalidate, no-store");
    request->send(response);
}

void APIServer::getOutputMarked(AsyncWebServerRequest *request) {

}

void APIServer::setManualControl(AsyncWebServerRequest *request) {
    // No error checking because we ball.
    const AsyncWebParameter *value = request->getParam(F("value"));

    settings->manualControl = stringToBool(value->value());
    log(String("Set manual control to ") + value->value() + String("."));
    request->send(204);
}

void APIServer::move(AsyncWebServerRequest *request) {
    // No error checking either except maybe for overlapping inputs.
    const AsyncWebParameter *direction = request->getParam(F("direction"));
    const AsyncWebParameter *on = request->getParam(F("on"));

    if (direction->value() == "forward") {
        log(String("Set forward movement to ") + on->value() + String("."));
    }
    else if (direction->value() == "back") {
        log(String("Set back movement to ") + on->value() + String("."));
    }
    else if (direction->value() == "left") {
        log(String("Set left movement to ") + on->value() + String("."));
    }
    else if (direction->value() == "right") {
        log(String("Set right movement to ") + on->value() + String("."));
    }

    request->send(204);
}

void APIServer::notFound(AsyncWebServerRequest *request) {
    request->send(404, F("text/plain"), F("Not found"));
}

void APIServer::log(String content) {
    logs.push_back(ServerLog(content));
}