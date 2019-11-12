#ifndef WEBSERVER_hh
#define WEBSERVER_hh

#include <Arduino.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

#include "pages/mainPage.hh"
#include "pages/mainJS.hh"
#include "pages/mainCSS.hh"

namespace Car
{
    class webServer
    {
        public:
        webServer(uint16_t port = 80);
        void startServer();

        QueueHandle_t* getOutputQueue();
        QueueHandle_t* getInputQueue();

        private:
        void handleData(AsyncWebServerRequest *request);
        
        AsyncWebServer server_;
        QueueHandle_t outputQueue_;
        QueueHandle_t inputQueue_;
    };
};
#endif