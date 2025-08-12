#ifndef SERVERAPI_HPP
#define SERVERAPI_HPP

#include <ctime>
#include <string>
#include <vector>

#include <WebServer.h>

struct ServerLog {
    std::time_t timestamp;
    std::string content;

    ServerLog(std::string userContent);

    inline bool operator==(ServerLog &other);
    inline bool operator!=(ServerLog &other) {return !(*this == other);};
};

// Class representing a webserver and the APIs for it.
class ServerAPI {
    private:
        WebServer *webServer;

        std::vector<ServerLog> logs;
        
        void getRoot();
        void getLogs();
        void getOutputRaw();
        void getOutputMarked();
        void notFound();

    public:
        ServerAPI(const int port);

        void begin();
        void handleClient();

        void log(std::string content);
};

#endif