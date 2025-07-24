#include "serverapi.hpp"

/*
The WebServer library expects type THandlerFunction (which itself is std::function<void()>) for request handlers.
To remove the implicit `this` argument from class member functions, it is bound to the object instance.
*/
#define WRAP_REQ_HANDLER(func) WebServer::THandlerFunction(std::bind(func, this))

ServerAPI::ServerAPI(const int port) {
    webServer = new WebServer(port);

    // Register API request handlers.
    webServer->on("/", HTTP_GET, WRAP_REQ_HANDLER(&ServerAPI::getRoot));
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

void ServerAPI::getOutputRaw() {

}

void ServerAPI::getOutputMarked() {

}

void ServerAPI::notFound() {
    webServer->send(404, "text/plain", "Not found");
}