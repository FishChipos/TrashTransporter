#ifndef SERVERAPI_HPP
#define SERVERAPI_HPP

#include <WebServer.h>

// Class representing a webserver and the APIs for it.
class ServerAPI {
    private:
        WebServer *webServer;
        
        void getRoot();
        void getOutputRaw();
        void getOutputMarked();
        void notFound();

    public:
        ServerAPI(const int port);

        void begin();
        void handleClient();
};

#endif