/*
  To upload through terminal you can use: curl -F "image=@firmware.bin" esp8266-webupdate.local/update
*/

#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <Update.h>
#include <esp_wifi.h>


#define AP_HOST "ESP32S3_Web"
#define HTTP_PORT 80
#define serverIndex "<form method='POST' action='/update' enctype='multipart/form-data'><input type='file' name='update'><input type='submit' value='Update'></form>"


void initSoftAP();
void initWebServer();
uint32_t getChipId();



WebServer server(HTTP_PORT);


void setup()
{
    Serial.begin(115200);
    Serial.println();
    Serial.println("Booting Sketch...");

    initSoftAP();
    initWebServer();
}

void loop()
{
    server.handleClient();
    delay(2);//allow the cpu to switch to other tasks
}

uint32_t getChipId()
{
    uint32_t chip_id = 0;
    for (int i = 0; i < 17; i = i + 8)
    {
        chip_id |= (ESP.getEfuseMac() >> (40 - i) & 0xff) << i;
    }
    return chip_id;
}


void initSoftAP()
{
//    WiFi.softAPConfig();
    String random_ssid = "ESP_KIT_AP_";
    String chip_id = String(getChipId());
    random_ssid = random_ssid + chip_id;
    Serial.print("Chip ID: ");
    Serial.println(chip_id);
    WiFi.softAP(random_ssid.c_str());
    IPAddress ap_ip = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(ap_ip);
    if (WiFi.softAP(random_ssid.c_str()))
    {
        Serial.println("ESP-S3-32S SoftAP is running.");
        Serial.print("ESP-S3-32S SoftAP IP Address: ");
        Serial.println(WiFi.softAPIP());
    } else
    {
        Serial.println("ESP-S3-32S AP Failed");
        delay(1000);
        Serial.println("restart now...");
        ESP.restart();
    }

}

void initWebServer()
{
    Serial.println("Server started");

    MDNS.begin(AP_HOST);
    server.on("/", HTTP_GET, []()
    {
        server.sendHeader("Connection", "close");
        server.send(200, "text/html", serverIndex);
    });

    server.on("/update", HTTP_POST, []()
    {
        server.sendHeader("Connection", "close");
        server.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
        ESP.restart();
    }, []()
              {
                  HTTPUpload &upload = server.upload();
                  if (upload.status == UPLOAD_FILE_START)
                  {
                      Serial.setDebugOutput(true);
                      Serial.printf("Update: %s\n", upload.filename.c_str());
                      if (!Update.begin())
                      { //start with max available size
                          Update.printError(Serial);
                      }
                  } else if (upload.status == UPLOAD_FILE_WRITE)
                  {
                      if (Update.write(upload.buf, upload.currentSize) != upload.currentSize)
                      {
                          Update.printError(Serial);
                      }
                  } else if (upload.status == UPLOAD_FILE_END)
                  {
                      if (Update.end(true))
                      { //true to set the size to the current progress
                          Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
                      } else
                      {
                          Update.printError(Serial);
                      }
                      Serial.setDebugOutput(false);
                  } else
                  {
                      Serial.printf("Update Failed Unexpectedly (likely broken connection): status=%d\n",
                                    upload.status);
                  }
              });
    server.begin();
    MDNS.addService("http", "tcp", HTTP_PORT);

    Serial.printf("Ready! Open http://%s.local in your browser\n", AP_HOST);
}
