/******************************************************************************
 * @file simpleExample.cpp
 *
 * @page Example sketch showing how to use the aaCFlash class. 
 * 
 * @section intro_sec Introduction
 *
 * This is an example sketch provided with the aaFlash library that shows how
 * to read and write to flash memory so the information  is not lost during reboot.
 *
 * @section dependencies Dependencies
 * 
 * This sketch class depends on on the following libraries:
 * * <a href="https://github.com/espressif/arduino-esp32">Arduino.h</a>. This 
 * is the Arduino core library that comes bundled with PlatformIO.
 *
 * @section author Author(s)
 *
 * Written by Old Squire for the Aging Apprentice.
 *
 * @section license license
 *
 * Copyright 2021 the Aging Apprentice 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to 
 * deal in the Software without restriction, including without limitation the 
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is 
 * furnished to do so, subject to the following conditions: The above copyright
 * notice and this permission notice shall be included in all copies or 
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.  
 *****************************************************************************/ 
#include <Arduino.h> // Arduino Core for ESP32. Comes with Platform.io.
#include <aaWeb.h> // Store values that persist past reboot.
#include <aaNetwork.h> // Wifi functions. 

/**
 * Define global objects.
 * =================================================================================*/
aaWeb myWebServer; // Explain what this object reference is for. 
aaNetwork network(HOST_NAME_PREFIX); // WiFi session management.
const char* WEB_APP_TITLE = "Example"; // App name for web page titles.
aaWeb localWebService(WEB_APP_TITLE); // Webserver hosted by microcontroller.

/**
 * @brief Initialize the serial output with the specified baud rate measured in bits 
 * per second.
 * =================================================================================*/
void setupSerial()
{
   unsigned long serialBaudRate = 115200; // Speed we want for serial output (bps).
   Serial.begin(serialBaudRate); // Open a serial connection at specified baud rate. 
   while (!Serial); // Wait for Serial port to be ready.
} //setupSerial()

/** 
 * @brief Start up the web server.
 * @details Web server supports configuring the MQTT broker IP without needing to 
 *          rebuild. It also provides a web interface for doing OTA code downloads.
 * =================================================================================*/
void startWebServer()
{
   char uniqueName[HOST_NAME_SIZE]; // Contain unique name for Wifi network purposes. 
   char *uniqueNamePtr = &uniqueName[0]; // Pointer to starting address of name. 
   network.getUniqueName(uniqueNamePtr); // Get unique name. 
   Serial.print("<startWebServer> Unique Name: "); Serial.println(uniqueName);
   Serial.print("<startWebServer> Name length: "); Serial.println(strlen(uniqueName));
   isWebServer = localWebService.start(uniqueNamePtr); // Start web server and track result.
   if(isWebServer)
   {
      Serial.println("<startWebServer> Web server successfully started.");
   } //if
   else
   {
      Serial.println("<startWebServer> Web server failed to start.");
   } //else
} //startWebServer()

/**
 * @brief Monitor local web service to see if there are any client requests.
 * @details Call to checkForClientRequest() does two things. First, it causes the 
 * localWebServer service to process any new binay downloads. Second, it returns a 
 * boolean, that when TRUE, indicates there is a new IP address for the MQTT broker 
 * that needs to be saved to NV RAM.
 * =================================================================================*/
void monitorWebServer()
{
   if(localWebService.checkForClientRequest()) // New binary or broker IP?
   {
      IPAddress tmpIP = localWebService.getBrokerIP(); // Get awaiting IP address.
      Serial.print("<monitorWebServer> Set broker IP to "); Serial.println(tmpIP);
      flash.writeBrokerIP(tmpIP); // Write address to flash.
   } //if
} //monitorWebServer()

/** 
 * @brief Arduino mandatory function #1. Runs once at boot. 
 * =================================================================================*/
void setup()
{
   // Declare variables.
   setupSerial(); // Set serial baud rate. 
   Serial.println("<setup> Start of setup");
   // Call stuff here.
   Serial.println("<setup> End of setup");
} // setup()

/**
 * @brief Arduino mandatory function #2. Runs continually.
 * =================================================================================*/
void loop()
{

} //loop()