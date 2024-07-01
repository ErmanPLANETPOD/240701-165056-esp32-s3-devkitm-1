// globals.h
#ifndef GLOBALS_H
#define GLOBALS_H

// The needed libraries for the functions in this file are included
#include <SPI.h>
#include <mcp_can.h>
#include <Wire.h>
#include <PCF8574.h>

// Variables to use in calculations and computations
extern float DCVoltageRead;       // Declare the DC Voltage variable as extern
extern float ACVoltageRead;       // Declare the AC Voltage variable as extern
extern float ambTemperatureRead;  // Declare the ambient temperature variable as extern
extern float DCCurrentRead;       // Declare the DC Current variable as extern
extern float reverseSetCurrent;   // Declare the reverse Set Current variable as extern
extern float setCurrent;          // Declare the set current variable as extern
extern float reverseSetVoltage;   // Declare the reverse Set voltage variable as extern
extern float setVoltage;          // Declare the set voltage variable as extern

extern float batteryCurrent;        // Declare the battery Current variable as extern
extern float batteryVoltage;        // Declare the battery voltage variable as extern
extern int batteryFailureLevel;   // Declare the battery failure level variable as extern

extern int  tempB0hw;             // Declare the T-ALARM BIC 0 hardware bit as extern
extern int  faultB0hw;            // Declare the FAULT BIC 0 hardware bit as extern
extern int  DCB0hw;               // Declare the DC-OK BIC 0 hardware bit as extern
extern int  tempB1hw;             // Declare the T-ALARM BIC 1 hardware bit as extern
extern int  faultB1hw;            // Declare the FAULT BIC 1 hardware bit as extern
extern int  DCB1hw;               // Declare the DC-OK BIC 1 hardware bit as extern

extern int  remoteCTRL;           // Declare the remote control hardware bit
// extern int  CD;                   // Declare the analog charge, discharge bit 

// Define parameters here (voltage, current, temperature etc..)
float safeVoltage = 40.0;
float safeCurrent = 0.0;
float setChargingVoltage = 50;
float setChargingCurrent = 0.0;
float setDischargingCurrent = 0.0;
float setDischargingVoltage = 0.0;

// Thresholds
float dataTempThreshold = 40.0;
float dataOverVoltageThreshold = 54.8;
float dataCurrentThreshold = 10;
float dataUnderVoltageThreshold = 38.0;

// Error flags
extern bool dataTempError;
extern bool dataVoltageError;
extern bool dataCurrentError;
extern bool dataUnderVoltageError;
extern bool batteryCurrentError;
extern bool batteryOvervoltageError;
extern bool batteryUndervoltageError;

// Struct to store each bit of the fault status
struct FaultStatus {
    bool fanFail;
    bool OTP;
    bool OVP;
    bool OVC;
    bool SCP;
    bool ACFail;
    bool DCStatus;
    bool highTemperature;
};

extern FaultStatus faultStatus; // Declare the Fault Status struct as extern

// GPIO Variables
const int SPI_CS_PIN = 38;    // CS Pin for MCP2515 on SCU
const int SPI_SCK_PIN = 1;    // SCK Pin for MCP2515 on SCU
const int SPI_MISO_PIN = 6;   // MISO Pin for MCP2515 on SCU
const int SPI_MOSI_PIN = 5;;  // MOSI Pin for MCP2515 on SCU
const int swEN = 17;          // Pin for swEN
const int BAT_OC_PIN = 10;    // Pin for BAT OC
const int BAT_OV_PIN = 11;    // Pin for BAT OV
const int BAT_UV_PIN = 12;    // Pin for BAT UV
const int BAT_OT_PIN = 13;    // Pin for BAT OT
const int nRelay = 21;        // Pin for N-pole relay
const int pRelay = 47;        // Pin for P-pole relay

// Declaration of the MCP CAN Object
MCP_CAN CAN(SPI_CS_PIN);   // Declare the CAN object

// Set the I2C address of your PCF8574 (A0, A1, A2 = GND)
#define PCF8574_ADDRESS 0x20
// Create an instance of the PCF8574 class
PCF8574 pcf8574(PCF8574_ADDRESS);

// MCP2515 Command Bytes
#define MCP2515_WRITE 0x02
#define MCP2515_READ 0x03
#define MCP2515_BIT_MODIFY 0x05

// MCP2515 Register Addresses
#define CANCTRL 0x0F // CAN Control Register
#define CANSTAT 0x0E // CAN Status Register

// BIC Address definitions
#define BIC0_ADDRESS 0x00

// BMS Address definition
#define BMS_ADDRESS // Fill in once known

#endif
