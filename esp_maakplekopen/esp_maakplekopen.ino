/**
 *
 */

#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ESP8266HTTPClient.h>

#define USE_SERIAL Serial

ESP8266WiFiMulti WiFiMulti;

#define sensorPin A0
#define ledOUTPUTPin 2
int sensorValue = 0;  
int percent = 0;

void setup() {

    USE_SERIAL.begin(115200);
   // USE_SERIAL.setDebugOutput(true);

    USE_SERIAL.println();
    USE_SERIAL.println();
    USE_SERIAL.println();

    for(uint8_t t = 4; t > 0; t--) {
        USE_SERIAL.printf("[SETUP] WAIT %d...\n", t);
        USE_SERIAL.flush();
        delay(1000);
    }

    WiFiMulti.addAP("Maakplek", "ikbennietdeslimste@maakplek.nl");
    WiFiMulti.addAP("thuisWIFI", "thuisWachtWoord");
    pinMode(ledOUTPUTPin, OUTPUT);
}

void loop() {
    // wait for WiFi connection
    if((WiFiMulti.run() == WL_CONNECTED)) {

        HTTPClient http;

        USE_SERIAL.print("[HTTP] begin...\n");
        // configure traged server and url
        //http.begin("https://192.168.1.12/test.html", "7a 9c f4 db 40 d3 62 5a 6e 21 bc 5c cc 66 c8 3e a1 45 59 38"); //HTTPS
        http.begin("http://maakplek.nl/open/"); //HTTP

        USE_SERIAL.print("[HTTP] GET...\n");
        // start connection and send HTTP header
        int httpCode = http.GET();

        // httpCode will be negative on error
        if(httpCode > 0) {
            // HTTP header has been send and Server response header has been handled
            USE_SERIAL.printf("[HTTP] GET... code: %d\n", httpCode);

            // file found at server
            if(httpCode == HTTP_CODE_OK) {
                String payload = http.getString();
                // De pagina maakplek.nl/open bevat 1 karakter, spatie of enter.
                // Als spatie is de Maakplek open:
                if (payload == " ") {
                  // Via de seriele debugpoort delen.
                  USE_SERIAL.printf("open");
                  // De LED aanzetten :)
                  digitalWrite(ledOUTPUTPin, 1);
                } else {
                  // In alle andere gevallen is de Maakplek dicht.
                  // Via de seriele debugpoort delen.
                  USE_SERIAL.printf("closed");
                  // De LED dan maar weer uitzetten.
                  digitalWrite(ledOUTPUTPin, 0);
                }
                USE_SERIAL.println(payload);
            }
        } else {
            USE_SERIAL.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }

        http.end();
    }
    // 10 seconden wachten...
    delay(10000);
}
