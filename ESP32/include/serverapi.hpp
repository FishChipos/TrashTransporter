#ifndef SERVERAPI_HPP
#define SERVERAPI_HPP

#include <ctime>
#include <string>
#include <vector>

#include <WebServer.h>

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
    HTTPMethod method;
    WebServer::THandlerFunction handler;
    // For logging purposes.
    String description;

    // List of parameters for logging purposes.
    std::vector<APIEndpointParameter> parameters;
};

// Class representing a webserver and the APIs for it.
class APIServer {
    private:
        // I'm pretty sure this has to be a pointer because it binds a network port to it so the web server cannot be copied.
        WebServer *webServer;

        bool isLoggingEnabled;

        std::vector<APIEndpoint> endpoints;

        std::vector<ServerLog> logs;
        
        void registerAPIEndpoints();
        void getRoot();
        void getLogs();
        void getOutputRaw();
        void getOutputMarked();
        void setManualControl();
        void notFound();

    public:
        APIServer(const int port);

        void enableLogging(bool on);

        void begin();
        void handleClient();

        void log(String content);
};

#endif