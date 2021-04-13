/*********************************************************************
 This is an example for our nRF51822 based Bluefruit LE modules

 Pick one up today in the adafruit shop!

 Adafruit invests time and resources providing this open source code,
 please support Adafruit and open-source hardware by purchasing
 products from Adafruit!

 MIT license, check LICENSE for more information
 All text above, and the splash screen below must be included in
 any redistribution
*********************************************************************/

/*
    Please note the long strings of data sent mean the *RTS* pin is
    required with UART to slow down data sent to the Bluefruit LE!
*/

#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include "Adafruit_MCP9808.h"
#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_SPI.h"
#include "Adafruit_BluefruitLE_UART.h"

#include "BluefruitConfig.h"

#if SOFTWARE_SERIAL_AVAILABLE
  #include <SoftwareSerial.h>
#endif

// Create the MCP9808 temperature sensor object
Adafruit_MCP9808 tempsensor = Adafruit_MCP9808();


// Define GUIDs for custom service and characteristic
// These are not standard BLE sevice type, but just random GUIDs
#define TEMP_SERVICE_UUID128 "89-81-e9-0d-ba-e8-4f-e7-83-54-fd-5e-b0-5d-60-ee"
#define TEMP_CHAR_UUID128 "af-3a-f4-6c-c8-b5-4e-10-87-fe-8b-b8-a2-31-27-a5"

// Create the bluefruit object, either software serial...uncomment these lines
/*
SoftwareSerial bluefruitSS = SoftwareSerial(BLUEFRUIT_SWUART_TXD_PIN, BLUEFRUIT_SWUART_RXD_PIN);

Adafruit_BluefruitLE_UART ble(bluefruitSS, BLUEFRUIT_UART_MODE_PIN,
                      BLUEFRUIT_UART_CTS_PIN, BLUEFRUIT_UART_RTS_PIN);
*/

/* ...or hardware serial, which does not need the RTS/CTS pins. Uncomment this line */
// Adafruit_BluefruitLE_UART ble(BLUEFRUIT_HWSERIAL_NAME, BLUEFRUIT_UART_MODE_PIN);

/* ...hardware SPI, using SCK/MOSI/MISO hardware SPI pins and then user selected CS/IRQ/RST */
//Adafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_CS, BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);

/* ...software SPI, using SCK/MOSI/MISO user-defined SPI pins and then user selected CS/IRQ/RST */
Adafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_SCK, BLUEFRUIT_SPI_MISO,
                             BLUEFRUIT_SPI_MOSI, BLUEFRUIT_SPI_CS,
                             BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);


// A small helper
void error(const __FlashStringHelper*err) {
  Serial.println(err);
  while (1);
}

/* The service information */

int32_t tmpServiceId;
int32_t tmpMeasureCharId;

/**************************************************************************/
/*!
    @brief  Sets up the HW an the BLE module (this function is called
            automatically on startup)
*/
/**************************************************************************/
void setup(void)
{
  while (!Serial); // required for Flora & Micro
  delay(500);

  boolean success;

  Serial.begin(115200);
  Serial.println(F("Temperature Monitor with MCP9808 using BLE SPI friend"));
  Serial.println(F("--------------------------------------------------------"));

  randomSeed(micros());

  /* Initialise the module */
  Serial.print(F("Initialising the Bluefruit LE module: "));

  if ( !ble.begin(VERBOSE_MODE) )
  {
    error(F("Couldn't find Bluefruit, make sure it's in CoMmanD mode & check wiring?"));
  }
  Serial.println( F("OK!") );

  /* Perform a factory reset to make sure everything is in a known state */
  Serial.println(F("Performing a factory reset: "));
  if (! ble.factoryReset() ){
       error(F("Couldn't factory reset"));
  }

  /* Disable command echo from Bluefruit */
  ble.echo(false);

  Serial.println("Requesting Bluefruit info:");
  /* Print Bluefruit information */
  ble.info();

// Make sure the sensor is found, you can also pass in a different i2c
  // address with tempsensor.begin(0x19) for example, also can be left in blank for default address use
  // Also there is a table with all addres possible for this sensor, you can connect multiple sensors
  // to the same i2c bus, just configure each sensor with a different address and define multiple objects for that
  //  A2 A1 A0 address
  //  0  0  0   0x18  this is the default address
  //  0  0  1   0x19
  //  0  1  0   0x1A
  //  0  1  1   0x1B
  //  1  0  0   0x1C
  //  1  0  1   0x1D
  //  1  1  0   0x1E
  //  1  1  1   0x1F
if (!tempsensor.begin(0x18)) {
    Serial.println("Couldn't find MCP9808! Check your connections and verify the address is correct.");
    while (1);
  }
    
   Serial.println("Found MCP9808!");

 tempsensor.setResolution(3); // sets the resolution mode of reading, the modes are defined in the table bellow:
  // Mode Resolution SampleTime
  //  0    0.5°C       30 ms
  //  1    0.25°C      65 ms
  //  2    0.125°C     130 ms
  //  3    0.0625°C    250 ms


  /* Change the device name to make it easier to find */
  Serial.println(F("Setting device name to 'Bluefruit Temperature monitor': "));

  if (! ble.sendCommandCheckOK(F("AT+GAPDEVNAME=Bluefruit Temperature Monitor")) ) {
    error(F("Could not set device name?"));
  }

  /* Add the Temp sensor service */
  /* Service ID should be 1 */
  Serial.println(F("Adding the Temperature read service definition : "));
  success = ble.sendCommandWithIntReply( F("AT+GATTADDSERVICE=UUID128=" TEMP_SERVICE_UUID128), &tmpServiceId);
  if (! success) {
    error(F("Could not add Temp service"));
  }

  /* Add the Temperature read characteristic */
  /* Chars ID for Measurement should be 1 */
  Serial.println(F("Adding the Temperature read characteristic : "));
  success = ble.sendCommandWithIntReply( F("AT+GATTADDCHAR=UUID128=" TEMP_CHAR_UUID128", PROPERTIES=0x12, MIN_LEN=1, MAX_LEN=10"), &tmpMeasureCharId);
    if (! success) {
    error(F("Could not add Temp characteristic"));
  }


  /* Reset the device for the new service setting changes to take effect */
  Serial.print(F("Performing a SW reset (service changes require a reset): "));
  ble.reset();

  Serial.println();
}

void setCharValue(int32_t charId, float value, int precision=2){
  // Set the specified characteristic to a floating point value.
  /* Command is sent when \n (\r) or println is called */
  /* AT+GATTCHAR=CharacteristicID,value */
 ble.print(F("AT+GATTCHAR="));
 ble.print(charId, DEC);
 ble.print(F(","));
 ble.println(value, precision);
 if ( !ble.waitForOK())
 {
  Serial.println(F("Failed to get response!"));
 }
}

/** Send randomized heart rate data continuously **/
void loop(void)
{
  Serial.println("wake up MCP9808.... "); // wake up MCP9808 - power consumption ~200 mikro Ampere
  tempsensor.wake();   // wake up, ready to read!

  // Read out the temperature.
 /* Serial.print("Resolution in mode: ");
  Serial.println (tempsensor.getResolution());*/
  float temp_c = tempsensor.readTempC();
  float temp_f = tempsensor.readTempF();
 /* Serial.print("Temp: "); 
  Serial.print(c, 4); Serial.print("*C\t and "); 
  Serial.print(f, 4); Serial.println("*F.");
  */
  
 // Serial.println("");
 // delay(200);
 
 // Set the characteristic values.
 setCharValue(tmpMeasureCharId, temp_c); 

 //wait 2 seconds (minimum sampling rate) and repeat!
 delay(2000);
 Serial.println("Shutdown MCP9808.... ");
 tempsensor.shutdown_wake(1); // shutdown MSP9808 - power consumption ~0.1 mikro Ampere, stops temperature sampling

}
