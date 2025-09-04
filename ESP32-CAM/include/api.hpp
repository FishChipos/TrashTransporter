#ifndef SERVERAPI_HPP
#define SERVERAPI_HPP

#include <ctime>
#include <string>
#include <vector>

#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

#include "settings.hpp"

struct ServerLog {
    std::time_t timestamp;
    String content;

    ServerLog(String userContent);

    inline bool operator==(ServerLog &other);
    inline bool operator!=(ServerLog &other);
};

// Exists for logging.
struct APIEndpointParameter {
    String key;
    String description;
};

struct APIEndpoint {
    String path;
    WebRequestMethod method;
    // For logging purposes.
    String description;
    ArRequestHandlerFunction requestHandler;
    ArUploadHandlerFunction uploadHandler;
    ArBodyHandlerFunction bodyHandler;

    // List of parameters for logging purposes.
    std::vector<APIEndpointParameter> parameters;
};

// Class representing a webserver and the APIs for it.
class APIServer {
    protected:
        // I'm pretty sure this has to be a pointer because it binds a network port to it so the web server cannot be copied.
        AsyncWebServer *webServer;
        Settings *settings;
        uint8_t *cameraOutputBuffer;
        size_t cameraOutputBufferSize;

        bool isLoggingEnabled;

        std::vector<APIEndpoint> endpoints;

        std::vector<ServerLog> logs;
        
        void registerAPIEndpoints();
        void getRoot(AsyncWebServerRequest *request);
        void getLogs(AsyncWebServerRequest *request);
        void getOutputRaw(AsyncWebServerRequest *request);
        void getOutputMarked(AsyncWebServerRequest *request);

        void setManualControl(AsyncWebServerRequest *request);

        void move(AsyncWebServerRequest *request);

        void notFound(AsyncWebServerRequest *request);

    public:
        APIServer(const int port, Settings *userSettings);

        void enableLogging(bool on);

        void begin();

        void log(String content);
};

#endif