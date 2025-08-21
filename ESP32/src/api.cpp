#include "api.hpp"

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
        webServer->on(endpoint.path.c_str(), endpoint.method, endpoint.handler);

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
                    Serial.print("    ");
                    Serial.print(parameter.key);
                    Serial.print(" ");
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

APIServer::APIServer(const int port, Settings *userSettings) {
    settings = userSettings;

    webServer = new AsyncWebServer(port);

    // Enable CORS to make the API accessible to other hosts.
    webServer->addMiddleware(new AsyncCorsMiddleware());

    // Define API request handlers.
    webServer->onNotFound([this](AsyncWebServerRequest *request){notFound(request);});

    endpoints.push_back({
        F("/"), 
        HTTP_GET, 
        [this](AsyncWebServerRequest *request) {
            getRoot(request);
        }, 
        F("Root path.")
    });
    endpoints.push_back({
        F("/logs"), 
        HTTP_GET, 
        [this](AsyncWebServerRequest *request) {
            getLogs(request);
        }, 
        F("Server logs.")
    });
    endpoints.push_back({
        F("/output/raw"), 
        HTTP_GET, 
        [this](AsyncWebServerRequest *request) {
            getOutputRaw(request);
        }, 
        F("Raw image output.")
    });
    endpoints.push_back({
        F("/output/marked"), 
        HTTP_GET, 
        [this](AsyncWebServerRequest *request) {
            getOutputMarked(request);
        }, 
        F("Marked image output.")
    });
    endpoints.push_back({
        F("/settings/manualcontrol"),
        HTTP_OPTIONS,
        [this](AsyncWebServerRequest *request) {
            request->send(204);
        },
        F("Query for CORS pre-flight requests.")
    });
    endpoints.push_back({
        F("/settings/manualcontrol"), 
        HTTP_PATCH, 
        [this](AsyncWebServerRequest *request) {
            setManualControl(request);
        },
        F("Set manual control for the robot."),
        {
            {F("value"), F("Set to true for manual control, false otherwise.")}
        }
    });
}

void APIServer::begin() {
    registerAPIEndpoints();

    webServer->begin();
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

}

void APIServer::getOutputMarked(AsyncWebServerRequest *request) {

}

void APIServer::setManualControl(AsyncWebServerRequest *request) {
    String value = "";


    if (value == "true") {
        settings->manualControl = true;
        log("Manual control set to true.");

        request->send(204);
    }
    else if (value == "false") {
        settings->manualControl = false;
        log("Manual control set to false.");

        request->send(204);
    }
    // Argument not found or malformed request.
    else {
        settings->manualControl = true;
        log("Error when handling request to set manual control, defaulting to manual control.");

        request->send(204);
    }
}

void APIServer::notFound(AsyncWebServerRequest *request) {
    request->send(404, F("text/plain"), F("Not found"));
}

void APIServer::log(String content) {
    logs.push_back(ServerLog(content));
}