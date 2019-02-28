// // Code for pushing data and getting status from the cloud service.
// // 
// // based on the ESP8266 HttpRequest example
// //
// // Will do the following:
// // - connect up to wifi
// // - relay messages from sprense to cloud and back
// // - use I2C to communicate with the sprense

// #include <Arduino.h>

// #include <ESP8266WiFi.h>
// #include <ESP8266WiFiMulti.h>

// #include <ESP8266HTTPClient.h>

// #include <WiFiClient.h>
// #include <Wire.h>


// ESP8266WiFiMulti WiFiMulti;

// byte SlaveReceive = 0;

// void setup() {

//   Wire.begin(8);
//   Wire.onReceive(receiveEvent);
//   Wire.onRequest(requestEvent);

//   Serial.begin(115200);
//   // Serial.setDebugOutput(true);

//   Serial.println();
//   Serial.println();
//   Serial.println();

//   for (uint8_t t = 4; t > 0; t--) {
//     Serial.printf("[SETUP] WAIT %d...\n", t);
//     Serial.flush();
//     delay(1000);
//   }

//   WiFi.mode(WIFI_STA);
//   WiFiMulti.addAP("haus", "0422196400");
//   WiFiMulti.addAP("Kalahari Resorts Guest");

// }

// void loop() {
//   // wait for WiFi connection
//   // if ((WiFiMulti.run() == WL_CONNECTED)) {

//   //   WiFiClient client;

//   //   HTTPClient http;

//   //   Serial.print("[HTTP] begin...\n");
//   //   if (http.begin(client, "http://jigsaw.w3.org/HTTP/connection.html")) {  // HTTP


//   //     Serial.print("[HTTP] GET...\n");
//   //     // start connection and send HTTP header
//   //     int httpCode = http.GET();

//   //     // httpCode will be negative on error
//   //     if (httpCode > 0) {
//   //       // HTTP header has been send and Server response header has been handled
//   //       Serial.printf("[HTTP] GET... code: %d\n", httpCode);

//   //       // file found at server
//   //       if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
//   //         String payload = http.getString();
//   //         Serial.println(payload);
//   //       }
//   //     } else {
//   //       Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
//   //     }

//   //     http.end();
//   //   } else {
//   //     Serial.printf("[HTTP} Unable to connect\n");
//   //   }
//   // }
//   Serial.println(SlaveReceive);
//   delay(1000);
// }

// void requestEvent() 
// {
//   Serial.printf("Requested message...");
// }

// void receiveEvent(int byteCount)
// {
//   SlaveReceive = Wire.read();
//   Serial.printf("Receive event message...");
// }


// Wire Slave Receiver
// by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the Wire library
// Receives data as an I2C/TWI slave device
// Refer to the "Wire Master Writer" example for use with this

// Created 29 March 2006

// This example code is in the public domain.


#include <Wire.h>

void setup() {
  Wire.pins(4, 5);
  Wire.begin(0x08);                // join i2c bus with address #8
  Wire.onReceive(receiveEvent); // register event
  Serial.begin(115200);           // start serial for output 
}

void loop() {
  // Serial.println(".");
  // delay(100);
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany) {
  Serial.println("Blast this thing....");
  while (1 < Wire.available()) { // loop through all but the last
    char c = Wire.read(); // receive byte as a character
    Serial.print(c);         // print the character
  }
  int x = Wire.read();    // receive byte as an integer
  Serial.println(x);         // print the integer
}

// #include <Wire.h>

// void setup() {
//   Wire.begin(); // join i2c bus (address optional for master)
//   Serial.begin(115200);           // start serial for output
// }

// byte x = 1;

// void loop() {
//   Serial.println("..");
  
//   Wire.beginTransmission(0x08); // transmit to device #8
//   //Wire.write("x is ");        // sends five bytes
//   Wire.write(x);              // sends one byte
//   Wire.endTransmission();    // stop transmitting

//   //x++;
//   delay(1000);
// }