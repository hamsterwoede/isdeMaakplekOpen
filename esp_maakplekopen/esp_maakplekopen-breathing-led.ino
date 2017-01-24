/**

*/

#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ESP8266HTTPClient.h>

#include <math.h>
#define USE_SERIAL Serial //lezen op 115200 baud

#define LED     D0        // Led in NodeMCU at pin GPIO16 (D0).
 
#define BRIGHT    350     //max led intensity (1-500)
#define INHALE    2500    //Inhalation time in milliseconds.
#define PULSE     INHALE*1000/BRIGHT
#define REST      1000    //Rest Between Inhalations.


ESP8266WiFiMulti WiFiMulti;

#define sensorPin A0
//#define ledOUTPUTPin 0
int sensorValue = 0;
int percent = 0;
int modeIs = 0;
unsigned long themillis = 0;
int wachttijd = 30000; //30 secondes of 30.000 milliSecondes

void setup() {
  USE_SERIAL.begin(115200);
  // USE_SERIAL.setDebugOutput(true);

  USE_SERIAL.println();
  USE_SERIAL.println();
  USE_SERIAL.println();

  for (uint8_t t = 4; t > 0; t--) {
    USE_SERIAL.printf("[SETUP] WAIT %d...\n", t);
    USE_SERIAL.flush();
    delay(10000);
  }

  WiFiMulti.addAP("Maakplek", "ikbennietdeslimste@maakplek.nl");
  WiFiMulti.addAP("Traag", "ifemkhhvve");
  // pinMode(ledOUTPUTPin, OUTPUT);
pinMode(LED, OUTPUT);   // LED pin as output.  

}

void loop() {
  // wait for WiFi connection
  if ((WiFiMulti.run() == WL_CONNECTED)) {
    switch (modeIs) {
      case 0:
        {
          //zet teller op huidig aantal millisecondes, meet de wachttijd
          int themillis = millis();
          modeIs = 1;
          USE_SERIAL.print("case 0 exit\n");
          break;

        }
      case 1:
        { //check connectie en zet open of gesloten modus
          HTTPClient http;

          USE_SERIAL.print("[HTTP] begin...\n");
          // configure traged server and url
          //http.begin("https://192.168.1.12/test.html", "7a 9c f4 db 40 d3 62 5a 6e 21 bc 5c cc 66 c8 3e a1 45 59 38"); //HTTPS
          http.begin("http://maakplek.nl/open/"); //HTTP

          USE_SERIAL.print("[HTTP] GET...\n");
          // start connection and send HTTP header
          int httpCode = http.GET();

          // httpCode will be negative on error
          if (httpCode > 0) {
            // HTTP header has been send and Server response header has been handled
            USE_SERIAL.printf("[HTTP] GET... code: %d\n", httpCode);

            // file found at server
            if (httpCode == HTTP_CODE_OK) {
              String payload = http.getString();
              // De pagina maakplek.nl/open bevat 1 karakter, spatie of enter.
              // Als spatie is de Maakplek open:
              if (payload == " ") {
                // Via de seriele debugpoort delen.
                USE_SERIAL.printf("open");
                //open zetten
                modeIs = 2;
              } else {
                // In alle andere gevallen is de Maakplek dicht.
                // Via de seriele debugpoort delen.
                USE_SERIAL.printf("closed");
                // De LED dan maar weer uitzetten.
                //digitalWrite(ledOUTPUTPin, 0);
                modeIs = 3;

              }
              USE_SERIAL.println(payload);
            }
          }
          else {
            USE_SERIAL.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
            modeIs = 0;
          }

          http.end();
          USE_SERIAL.print("case 1 exit\n");
          break;
        }


      case 2: // maakplek is open
        {
          // De LED aanzetten :)
          //digitalWrite(ledOUTPUTPin, 1);
           breathe();
          
          if ((millis() - themillis) > wachttijd)
          { //wachttijd groter dan 10 secondes? check dan opnieuw
            modeIs = 1;
            themillis = millis();
          }
          break;

        }
      case 3://maakplek gesloten
        {
          //tijdelijke test omdat maakplek gesloten is
   
    digitalWrite(LED, HIGH);
          if ((millis() - themillis) > wachttijd)
          { //wachttijd groter dan 10 secondes? check dan opnieuw
            modeIs = 1;
            themillis = millis();
          }

          break;
        }
    }
  }
}

void breathe() {
    //ramp increasing intensity, Inhalation: 
  for (int i=1;i<BRIGHT;i++){
    digitalWrite(LED, LOW);          // turn the LED on.
    delayMicroseconds(i*10);         // wait
    digitalWrite(LED, HIGH);         // turn the LED off.
    delayMicroseconds(PULSE-i*10);   // wait
    delay(0);                        //to prevent watchdog firing.
  }
  //ramp decreasing intensity, Exhalation (half time):
  for (int i=BRIGHT-1;i>0;i--){
    digitalWrite(LED, LOW);          // turn the LED on.
    delayMicroseconds(i*10);          // wait
    digitalWrite(LED, HIGH);         // turn the LED off.
    delayMicroseconds(PULSE-i*10);  // wait
    i--;
    delay(0);                        //to prevent watchdog firing.
  }
  delay(REST);                       //take a rest...
}
