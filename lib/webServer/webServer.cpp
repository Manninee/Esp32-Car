#include "webServer.hh"

namespace Car
{
    webServer::webServer(uint16_t port):
    server_(AsyncWebServer(port)),
    outputQueue_(xQueueCreate(30, sizeof(uint16_t[2]))),
    inputQueue_(xQueueCreate(30, sizeof(uint8_t[2])))
    {
        server_.on("/", HTTP_GET, [](AsyncWebServerRequest *request){request->send_P(200, "text/html", pages::mainPage);});
        server_.on("/mainJs", HTTP_GET, [](AsyncWebServerRequest *request){request->send_P(200, "text/javascript", pages::mainPageJs);});
        server_.on("/mainCSS", HTTP_GET, [](AsyncWebServerRequest *request){request->send_P(200, "text/css", pages::mainPageCSS);});
        server_.on("/data", HTTP_GET, std::bind(&webServer::handleData, this, std::placeholders::_1));
        server_.onNotFound([](AsyncWebServerRequest *request){request->send(404, "text/html", "Not found. Try again");});
    }

    void webServer::startServer(){ server_.begin(); }

    QueueHandle_t* webServer::getOutputQueue(){return &outputQueue_;}
    QueueHandle_t* webServer::getInputQueue(){return &inputQueue_;}

    void webServer::handleData(AsyncWebServerRequest *request){
        for(uint8_t i = 0; i < request->args(); ++i){
            uint16_t inData[2] = {request->argName(i).charAt(0), (uint16_t)request->arg(i).toInt()};
            xQueueSend(outputQueue_, &inData, 5);
        }

        String data = "";
        for(;;){
            uint8_t outData[2];
            if(xQueueReceive(inputQueue_, &outData, 0) == pdFALSE)
                break;
            
            data += String((char)outData[0]) + "=" + String(outData[1]) + ";";
        }
        data.remove(data.length() - 1);
        request->send(200, "text/plain", data);
    }
};
