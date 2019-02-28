// // Main routine for the Make It Better Challenge
// // #include <Wire.h>

// // byte SendToSlave = 1;
// // byte ReceiveFromSlave = -1;

// // void setup() {
// //   // put your setup code here, to run once:
// //   Wire.begin();

// //   Serial.begin(115200);

// // }

// // void loop() {
// //   // put your main code here, to run repeatedly:

// //   Serial.print("Sending: ");
// //   Serial.print(SendToSlave);
// //   Serial.print(" Receive: ");
// //   Serial.println(ReceiveFromSlave);

// //   delay(1000);
// // }

//  // --------------------------------------
// // i2c_scanner
// //
// // Version 1
// //    This program (or code that looks like it)
// //    can be found in many places.
// //    For example on the Arduino.cc forum.
// //    The original author is not know.
// // Version 2, Juni 2012, Using Arduino 1.0.1
// //     Adapted to be as simple as possible by Arduino.cc user Krodal
// // Version 3, Feb 26  2013
// //    V3 by louarnold
// // Version 4, March 3, 2013, Using Arduino 1.0.3
// //    by Arduino.cc user Krodal.
// //    Changes by louarnold removed.
// //    Scanning addresses changed from 0...127 to 1...119,
// //    according to the i2c scanner by Nick Gammon
// //    http://www.gammon.com.au/forum/?id=10896
// // Version 5, March 28, 2013
// //    As version 4, but address scans now to 127.
// //    A sensor seems to use address 120.
// // Version 6, November 27, 2015.
// //    Added waiting for the Leonardo serial communication.
// // 
// //
// // This sketch tests the standard 7-bit addresses
// // Devices with higher bit address might not be seen properly.
// //

// #include <Wire.h>


// void setup()
// {
//   Wire.begin();

//   Serial.begin(115200);
//   while (!Serial);             // Leonardo: wait for serial monitor
//   Serial.println("\nI2C Scanner");
// }


// void loop()
// {
//   byte error, address;
//   int nDevices;

//   Serial.println("Scanning...");

//   nDevices = 0;
//   for(address = 1; address < 127; address++ ) 
//   {
//     // The i2c_scanner uses the return value of
//     // the Write.endTransmisstion to see if
//     // a device did acknowledge to the address.
//     Wire.beginTransmission(address);
//     error = Wire.endTransmission();

//     if (error == 0)
//     {
//       Serial.print("I2C device found at address 0x");
//       if (address<16) 
//         Serial.print("0");
//       Serial.print(address,HEX);
//       Serial.println("  !");

//       nDevices++;
//     }
//     else if (error==4) 
//     {
//       Serial.print("Unknown error at address 0x");
//       if (address<16) 
//         Serial.print("0");
//       Serial.println(address,HEX);
//     }    
//   }
//   if (nDevices == 0)
//     Serial.println("No I2C devices found\n");
//   else
//     Serial.println("done\n");

//   delay(5000);           // wait 5 seconds for next scan
// }
// Wire Master Writer
// by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the Wire library
// Writes data to an I2C/TWI slave device
// Refer to the "Wire Slave Receiver" example for use with this

// Created 29 March 2006

// This example code is in the public domain.


#include <Wire.h>

void setup() {
  Wire.begin(); // join i2c bus (address optional for master)
  Serial.begin(115200);           // start serial for output
}

byte x = 12;

void loop() {
  Serial.println("..");
  
  Wire.beginTransmission(8); // transmit to device #8
  //Wire.write("x is ");        // sends five bytes
  Wire.write(x);              // sends one byte
  Wire.endTransmission();    // stop transmitting

  x++;
  delay(1000);
}