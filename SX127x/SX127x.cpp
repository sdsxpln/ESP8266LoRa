#include "Arduino.h"
#include <SPI.h>
#include <HardwareSerial.h>
#include "SX127x.h"


SX127x::SX127x(uint8_t serialDebug)
//! don't know why no serial print out if debug enable. Serial.begins works!
{


    _sx1276SelectPinCounter = 0;
    _serialDebug = serialDebug;
    //_sx1276SelectPin = 255; // start up setting
    //! reserves and resets 8 pins for modules
    _SelectPins[0] = 255; // Used as counter, set to 0 at first init
    _SelectPins[1] = 255; // first initiated  module
    _SelectPins[2] = 255;
    _SelectPins[3] = 255;
    _SelectPins[4] = 255;
    _SelectPins[5] = 255;
    _SelectPins[6] = 255;
    _SelectPins[7] = 255;
    _SelectPins[8] = 255; // eight initiated Module


    if (serialDebug == 1 )
    {
        //delay(1000); // wait a bit for ESP boot process serial printout
        Serial.begin(115200);
        Serial.println("Starting serial debug"); // don't know why this part does not work in Arduino setup()

    }



}


uint8_t SX127x::init(uint8_t sx1276SelectPin)
//! Use SX127x.init(NSSpin max 8 modules is supported by the driver
{

    if (_serialDebug == 1)
    {
        Serial.println();
        Serial.println("Starting init function: ");
    }
    if (_SelectPins[0] == 9) // max amount of modules without expanding the array
    {
        //! this is a faulty state, will it ever happen ?


    }
    else if ( _SelectPins[0] == 255 ) // first modeule init
    {
        //! First module init
        _SelectPins[0] = 1;

        if (_serialDebug == 1)
        {
            Serial.print("First init _SelectPins[0]= ");
            Serial.print(_SelectPins[0],DEC);
        }

        _SelectPins[_SelectPins[0]] = sx1276SelectPin ;

        pinMode(_SelectPins[_SelectPins[0]], OUTPUT);
        digitalWrite (_SelectPins[_SelectPins[0]], HIGH);

        if (_serialDebug == 1)
        {
            Serial.print(" pinMode and digitalWrite LOW on pin: ");
            Serial.println(_SelectPins[_SelectPins[0]],DEC);
        }
        state = 1 ;
        return state;
    }

    else if (_SelectPins[0] <= 8) // Adding pin for each module init
    {
        _SelectPins[0] ++;

        if (_serialDebug == 1)
        {
            Serial.print("Next init  _SelectPins[0]= ");
            Serial.print(_SelectPins[0],DEC);
        }

        _SelectPins[_SelectPins[0]] = sx1276SelectPin ;
        pinMode(_SelectPins[_SelectPins[0]], OUTPUT);
        digitalWrite (_SelectPins[_SelectPins[0]], HIGH);

        if (_serialDebug == 1)
        {
            Serial.print(" pinMode and digitalWrite LOW on pin: ");
            Serial.println(_SelectPins[_SelectPins[0]],DEC);
        }

    }

}

uint8_t SX127x::single(uint8_t moDule, uint8_t address, uint8_t value, uint8_t Write)
//! (Module#, address byte, data byte, 0 = Read or 1 = write from or to register)
{

    //! Set up SPI for SX127x
/*
    SPI.begin();
    SPI.setBitOrder(MSBFIRST);
    SPI.setFrequency(10000000); // MAX 10000000Hz (10MHz) for SX127x
    SPI.setDataMode(SPI_MODE0);
*/
    SPI.beginTransaction(SPISettings(10000000, MSBFIRST, SPI_MODE0));
    if (Write == 1)
    {
        bitSet(address, 7);			//! Bit 7 Write to registers
    }

    //! Enable SX127x

    if (_serialDebug == 1)
    {
    Serial.print("Message to Module: ");
    Serial.print(_moDule);
    Serial.print(address, HEX);
    Serial.print(" using NSS pin : ");
    Serial.print(_SelectPins[moDule]);
    }

    //! Do the transfer on SPI
    digitalWrite (_SelectPins[moDule], LOW); // NSS setup time, From NSS falling edge to SCK rising edge. 30ns SX1276-79 spec

    //! SPI.transfer16(message);  is one micro second slower than doing two 8bit at 10MHz clock speed
    SPI.transfer(address);
    _replySPIsingle = SPI.transfer(value);

    //! Release SX127x
    digitalWrite (_SelectPins[moDule], HIGH);  //! NSS hold time, From SCK falling edge to NSS rising edge, normal mode 100ns SX1276-79 spec
    if (_serialDebug == 1)
    {
    Serial.print("    Got reply:");
    Serial.println(_replySPIsingle, HEX);
    }
    state = 1;
    SPI.endTransaction();
    return _replySPIsingle;
}

uint8_t SX127x::burst()
{
    SPI.beginTransaction(SPISettings(2000000, MSBFIRST, SPI_MODE0)); // set up hardware SPI for SX127x
    if (serialDebug == 1)
    {
        Serial.println("SX127x::TX  :");
    }
    state = 1;
    delay(250);
    return state;
}

uint8_t SX127x::fifo()
{
    SPI.beginTransaction(SPISettings(2000000, MSBFIRST, SPI_MODE0)); // set up hardware SPI for SX127x
    if (serialDebug == 1)
    {
        Serial.println("SX127x::TX  :");
    }
    state = 1;
    delay(250);
    return state;
}


