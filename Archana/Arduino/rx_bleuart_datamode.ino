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

#include <Arduino.h>
#include <SPI.h>
#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_SPI.h"
#include "Adafruit_BluefruitLE_UART.h"

#include "BluefruitConfig.h"

#if SOFTWARE_SERIAL_AVAILABLE
  #include <SoftwareSerial.h>
#endif

/*=========================================================================
    APPLICATION SETTINGS

    FACTORYRESET_ENABLE       Perform a factory reset when running this sketch
   
                              Enabling this will put your Bluefruit LE module
                              in a 'known good' state and clear any config
                              data set in previous sketches or projects, so
                              running this at least once is a good idea.
   
                              When deploying your project, however, you will
                              want to disable factory reset by setting this
                              value to 0.  If you are making changes to your
                              Bluefruit LE device via AT commands, and those
                              changes aren't persisting across resets, this
                              is the reason why.  Factory reset will erase
                              the non-volatile memory where config data is
                              stored, setting it back to factory default
                              values.
       
                              Some sketches that require you to bond to a
                              central device (HID mouse, keyboard, etc.)
                              won't work at all with this feature enabled
                              since the factory reset will clear all of the
                              bonding data stored on the chip, meaning the
                              central device won't be able to reconnect.
    MINIMUM_FIRMWARE_VERSION  Minimum firmware version to have some new features
    MODE_LED_BEHAVIOUR        LED activity, valid options are
                              "DISABLE" or "MODE" or "BLEUART" or
                              "HWUART"  or "SPI"  or "MANUAL"
    -----------------------------------------------------------------------*/
    #define FACTORYRESET_ENABLE         1
    #define MINIMUM_FIRMWARE_VERSION    "0.6.6"
    #define MODE_LED_BEHAVIOUR          "MODE"
/*=========================================================================*/

// Create the bluefruit object, either software serial...uncomment these lines
/*
SoftwareSerial bluefruitSS = SoftwareSerial(BLUEFRUIT_SWUART_TXD_PIN, BLUEFRUIT_SWUART_RXD_PIN);

Adafruit_BluefruitLE_UART ble(bluefruitSS, BLUEFRUIT_UART_MODE_PIN,
                      BLUEFRUIT_UART_CTS_PIN, BLUEFRUIT_UART_RTS_PIN);
*/

/* ...or hardware serial, which does not need the RTS/CTS pins. Uncomment this line */
// Adafruit_BluefruitLE_UART ble(BLUEFRUIT_HWSERIAL_NAME, BLUEFRUIT_UART_MODE_PIN);

/* ...hardware SPI, using SCK/MOSI/MISO hardware SPI pins and then user selected CS/IRQ/RST */
Adafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_CS, BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);

/* ...software SPI, using SCK/MOSI/MISO user-defined SPI pins and then user selected CS/IRQ/RST */
//Adafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_SCK, BLUEFRUIT_SPI_MISO,
//                             BLUEFRUIT_SPI_MOSI, BLUEFRUIT_SPI_CS,
//                             BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);


// A small helper
void error(const __FlashStringHelper*err) {
  Serial.println(err);
  while (1);
}

// pins to use
bool lights_on = 0;
bool door_closed = 1;
const int DOOR_SENSOR_PIN =20; // teensy pin connected to door sensor's pin
const int RELAY_PIN       =15;  // teensy pin connected to the IN pin of relay
//const int RPI_SIGNAL_PIN  =17 ;  //teensy pin that recieve the signal from arduino
//bool rpiState ;
bool doorState;



/**************************************************************************/
/*!
    @brief  Sets up the HW an the BLE module (this function is called
            automatically on startup)
*/
/**************************************************************************/
void setup(void)
{
  while (!Serial);  // required for Flora & Micro
  delay(500);

  // setting pin modes
  pinMode(DOOR_SENSOR_PIN, INPUT_PULLUP); // set teensy pin to input pull-up mode
  pinMode(RELAY_PIN, OUTPUT);  

  Serial.begin(115200);
  Serial.println(F("Adafruit Bluefruit Command <-> Data Mode Example"));
  Serial.println(F("------------------------------------------------"));

  /* Initialise the module */
  Serial.print(F("Initialising the Bluefruit LE module: "));

  if ( !ble.begin(VERBOSE_MODE) )
  {
    error(F("Couldn't find Bluefruit, make sure it's in CoMmanD mode & check wiring?"));
  }
  Serial.println( F("OK!") );

  if ( FACTORYRESET_ENABLE )
  {
    /* Perform a factory reset to make sure everything is in a known state */
    Serial.println(F("Performing a factory reset: "));
    if ( ! ble.factoryReset() ){
      error(F("Couldn't factory reset"));
    }
  }

  /* Disable command echo from Bluefruit */
  ble.echo(false);

  Serial.println("Requesting Bluefruit info:");
  /* Print Bluefruit information */
  ble.info();

  Serial.println(F("Please use Adafruit Bluefruit LE app to connect in UART mode"));
  Serial.println(F("Then Enter characters to send to Bluefruit"));
  Serial.println();

  ble.verbose(false);  // debug info is a little annoying after this point!

  /* Wait for connection */
 /* while (! ble.isConnected()) {
      delay(500);
  }

  Serial.println(F("******************************"));

  // LED Activity command is only supported from 0.6.6
  if ( ble.isVersionAtLeast(MINIMUM_FIRMWARE_VERSION) )
  {
    // Change Mode LED Activity
    Serial.println(F("Change LED activity to " MODE_LED_BEHAVIOUR));
    ble.sendCommandCheckOK("AT+HWModeLED=" MODE_LED_BEHAVIOUR);
  }

  // Set module to DATA mode
  Serial.println( F("Switching to DATA mode!") );
  ble.setMode(BLUEFRUIT_MODE_DATA);

  Serial.println(F("******************************"));
  */
}

/**************************************************************************/
/*!
    @brief  Constantly poll for new command or response data
*/
/**************************************************************************/
void loop(void)
{

    //doorState = digitalRead(DOOR_SENSOR_PIN); // read state
    //bool lights_on = 0;
    if(!lights_on)
    {
        doorState = digitalRead(DOOR_SENSOR_PIN); // read state
        // wait until door gets opened
        while(!doorState)
        { 
          delay(100);
          doorState = digitalRead(DOOR_SENSOR_PIN); // read state
        }
        if(doorState == HIGH)
        {
              Serial.println("The door is open");
              //door_closed = 0;
              //delay(2000);
              // Turn ON lights
              digitalWrite(RELAY_PIN, HIGH);
              lights_on = 1;
              
        }
    }
    else
    {
        /* Wait for connection */
      while (! ble.isConnected()) {
      delay(500);
      }


      // LED Activity command is only supported from 0.6.6
      if ( ble.isVersionAtLeast(MINIMUM_FIRMWARE_VERSION) )
      {
        // Change Mode LED Activity
        //Serial.println(F("Change LED activity to " MODE_LED_BEHAVIOUR));
        ble.sendCommandCheckOK("AT+HWModeLED=" MODE_LED_BEHAVIOUR);
      }

      ble.setMode(BLUEFRUIT_MODE_DATA);      

      while(!ble.available())
      {
        delay(100);
      }

        char c = ble.read();
        Serial.print(c);
        if(c == 'a')
        {
            // turn OFF lights
            delay(2000);
            digitalWrite(RELAY_PIN, LOW);
            Serial.println("switching off the light");
            // wait for the door to get closed
            delay(10000);
            lights_on = 0;
            //door_closed=1;
    
        }
       
    }
}



/*
  
  doorState = digitalRead(DOOR_SENSOR_PIN); // read state
  while(doorState == HIGH)
  {
    Serial.println("The door is open");
    //delay(2000);
    digitalWrite(RELAY_PIN, HIGH);
   // rpiState = digitalRead(RPI_SIGNAL_PIN);
    while(!ble.available())
    {
      delay(300);
    }
    goto label;
  }
    

    label: 
   {
    if(ble.available())
    {
         char c = ble.read();
         Serial.print(c);
        if(c == 'a')
        {
            digitalWrite(RELAY_PIN, LOW);
            Serial.println("switching off the light");
            doorState=LOW;
    //delay(2000);
        }
    }
  
   }

     
  // Echo received data
  while ( ble.available() )
  {
    char c = ble.read();

    Serial.print(c);
    //Serial.print(c);

    // Hex output too, helps w/debugging!
    Serial.print(" [0x");
    if (c <= 0xF) Serial.print(F("0"));
    Serial.print(c, HEX);
    Serial.print("] ");
  */
  
