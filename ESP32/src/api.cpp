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

/*
The WebServer library expects type THandlerFunction (which itself is std::function<void()>) for request handlers.
To remove the implicit `this` argument from class member functions, it is bound to the object instance.
*/
#define WRAP_MEMBER_API_HANDLER(func) WebServer::THandlerFunction(std::bind(func, this))

void APIServer::registerAPIEndpoints() {
    if (isLoggingEnabled) Serial.println(F("Registering API endpoints..."));

    for (const APIEndpoint &endpoint : endpoints) {
        webServer->on(endpoint.path, endpoint.method, endpoint.handler);

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
                default:
                    Serial.print(F("UNSUPPORTED METHOD"));
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

APIServer::APIServer(const int port) {
    webServer = new WebServer(port);

    // Enable CORS to make the API accessible to other hosts.
    webServer->enableCORS();

    // Define API request handlers.
    webServer->onNotFound(WRAP_MEMBER_API_HANDLER(&APIServer::notFound));

    endpoints.push_back({
        F("/"), 
        HTTP_GET, 
        WRAP_MEMBER_API_HANDLER(&APIServer::getRoot), 
        F("Root path."), 
        {}
    });
    endpoints.push_back({
        F("/logs"), 
        HTTP_GET, 
        WRAP_MEMBER_API_HANDLER(&APIServer::getLogs), 
        F("Server logs."),
        {}
    });
    endpoints.push_back({
        F("/output/raw"), 
        HTTP_GET, 
        WRAP_MEMBER_API_HANDLER(&APIServer::getOutputRaw), 
        F("Raw image output."), 
        {}
    });
    endpoints.push_back({
        F("/output/marked"), 
        HTTP_GET, 
        WRAP_MEMBER_API_HANDLER(&APIServer::getOutputMarked), 
        F("Marked image output.")
    });

    endpoints.push_back({
        F("/settings/manualcontrol"), 
        HTTP_GET, 
        WRAP_MEMBER_API_HANDLER(&APIServer::setManualControl),
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

void APIServer::handleClient() {
    webServer->handleClient();
}

void APIServer::getRoot() {
    webServer->send(200, F("text/html"), F("<!DOCTYPE html><html><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\"></head><body><h1>Hey there!</h1></body></html>"));
}

void APIServer::getLogs() {
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

    webServer->send(200, F("text/json"), responseBody);
}

void APIServer::getOutputRaw() {

}

void APIServer::getOutputMarked() {

}

void APIServer::setManualControl() {
    String value = webServer->arg(F("value"));
}

void APIServer::notFound() {
    webServer->send(404, F("text/plain"), F("Not found"));
}

void APIServer::log(String content) {
    logs.push_back(ServerLog(content));
}