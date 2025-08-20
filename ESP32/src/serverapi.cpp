#include "serverapi.hpp"

// ServerLog implementations.
ServerLog::ServerLog(std::string userContent) {
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

/*
The WebServer library expects type THandlerFunction (which itself is std::function<void()>) for request handlers.
To remove the implicit `this` argument from class member functions, it is bound to the object instance.
*/
#define WRAP_REQ_HANDLER(func) WebServer::THandlerFunction(std::bind(func, this))

ServerAPI::ServerAPI(const int port) {
    webServer = new WebServer(port);

    // Enable CORS to make API accessible to other hosts.
    webServer->enableCORS();

    // Register API request handlers.
    webServer->on("/", HTTP_GET, WRAP_REQ_HANDLER(&ServerAPI::getRoot));
    webServer->on("/logs", HTTP_GET, WRAP_REQ_HANDLER(&ServerAPI::getLogs));
    webServer->on("/output/raw", HTTP_GET, WRAP_REQ_HANDLER(&ServerAPI::getOutputRaw));
    webServer->on("/output/marked", HTTP_GET, WRAP_REQ_HANDLER(&ServerAPI::getOutputMarked));
    webServer->onNotFound(WRAP_REQ_HANDLER(&ServerAPI::notFound));
}

void ServerAPI::begin() {
    webServer->begin();
}

void ServerAPI::handleClient() {
    webServer->handleClient();
}

void ServerAPI::getRoot() {
    webServer->send(200, "text/html", "<!DOCTYPE html><html><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\"></head><body><h1>Hey there!</h1></body></html>");
}

void ServerAPI::getLogs() {
    // Holy mother of response formatting.
    std::string responseBody;

    // Initial reservation to prevent too many memory reallocations.
    responseBody.reserve(2048);

    responseBody += "{\n\"count\": ";
    responseBody += std::to_string(logs.size());
    responseBody += ",\n\"logs\": [\n";
    
    for (ServerLog log : logs) {
        std::tm *calendarTimestamp = std::localtime(&(log.timestamp));

        responseBody += "{\n\"timestamp\": {\n";
        responseBody += "\"hours\": ";
        responseBody += std::to_string(calendarTimestamp->tm_hour);
        responseBody += ",\n\"minutes\": ";
        responseBody += std::to_string(calendarTimestamp->tm_min);
        responseBody += ",\n\"seconds\": ";
        responseBody += std::to_string(calendarTimestamp->tm_sec);
        responseBody += "\n},\n\"content\": \"";
        responseBody += log.content;
        responseBody += "\"\n}";
        

        if (log != logs.back()) {responseBody += ", ";}
    }

    responseBody += "\n]\n}";

    webServer->send(200, "text/json", responseBody.c_str());
}

void ServerAPI::getOutputRaw() {

}

void ServerAPI::getOutputMarked() {

}

void ServerAPI::notFound() {
    webServer->send(404, "text/plain", "Not found");
}

void ServerAPI::log(std::string content) {
    logs.push_back(ServerLog(content));
}