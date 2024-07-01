#include "globals.h"
#include "initProcesses.h"
#include "pcfComm.h"
#include "directionCTRL.h"
#include "setPower.h"
#include "monitorBIC.h"
#include "errorProcessing.h"
#include "monitorBMS.h"
#include "serialCommands.h"

int leon;

// Re-declaration of variables
float DCVoltageRead;       
float ACVoltageRead;       
float ambTemperatureRead;  
float DCCurrentRead;     
float reverseSetCurrent;
float setCurrent;
float reverseSetVoltage;
float setVoltage;

float batteryCurrent;    
float batteryVoltage;       
int batteryFailureLevel;

int  tempB0hw;             
int  faultB0hw;            
int  DCB0hw;               
int  tempB1hw;             
int  faultB1hw;            
int  DCB1hw;        

bool dataTempError;
bool dataVoltageError;
bool dataCurrentError;
bool dataUnderVoltageError;
bool batteryCurrentError;
bool batteryOvervoltageError;
bool batteryUndervoltageError;

FaultStatus faultStatus;

int remoteCTRL;
// int CD;

void setup() {

  initGPIO();

  Serial.begin(115200);
  	  while(!Serial);

      // Initializing SPI and I2C and CAN
      Wire.begin();

      SPI.begin(SPI_SCK_PIN, SPI_MISO_PIN, SPI_MOSI_PIN, SPI_CS_PIN);

      initCAN();

      forceNormalMode();


      // Initializing PCF8574
      initPCF();
      writePCF(pcf8574, 0);
      // Bypassing the hardware error checking circuit
      enBypass();
      // Reading all data such that we store initial values
      readBICData(BIC0_ADDRESS, pcf8574);
      handleFaultOutputsInit();
      // serialSendConfig();
      // setDirectionToDischarging(BIC0_ADDRESS);
      // setReverseVout(BIC0_ADDRESS, 45);
      // setReverseIout(BIC0_ADDRESS, 5);
      // serialEnableOutput();
      // setDirectionToDischarging(BIC0_ADDRESS);
      // setReverseVout(BIC0_ADDRESS, 45);
      // setReverseIout(BIC0_ADDRESS, 5);
      // writePCF(pcf8574, remoteCTRL);
      // handleBMSActivationPulse();
      readBICData(BIC0_ADDRESS, pcf8574);
      while(!readBMSData(batteryCurrent, batteryVoltage, batteryFailureLevel));
      handleFaultOutputsInit();
      configureChargeDischarge();
      readBICData(BIC0_ADDRESS, pcf8574);
      while(!readBMSData(batteryCurrent, batteryVoltage, batteryFailureLevel));
      handleFaultOutputsInit();
      serialEnableOutput();
      delay(400);
}

void loop() {
    // Serial.println("Setting direction to charging...");
    // setDirection(BIC0_ADDRESS, true); // Set to charging
    // delay(2000); // Wait for 2 seconds

    // // Set charging parameters
    // setChargingParams(BIC0_ADDRESS, 54.0, 2.0); // 54V, 2A
    // delay(2000); // Wait for 2 seconds

    // // Verify that the BIC is in charging mode (optional)
    // // int direction = readBICDirection(BIC0_ADDRESS);
    // // if (direction == 0x00) {
    // //     Serial.println("BIC is in charging mode.");
    // // } else {
    // //     Serial.println("Failed to confirm BIC is in charging mode.");
    // // }

    // delay(5000); // Wait for 5 seconds

    // Serial.println("Setting direction to discharging...");
    // setDirection(BIC0_ADDRESS, false); // Set to discharging
    // delay(2000); // Wait for 2 seconds

    // // Set discharging parameters
    // setDischargingParams(BIC0_ADDRESS, 45.0, 4.0); // 45V, 4A
    // delay(2000); // Wait for 2 seconds

    // // Verify that the BIC is in discharging mode (optional)
    // // int direction = readBICDirection(BIC0_ADDRESS);
    // // if (direction == 0x01) {
    // //     Serial.println("BIC is in discharging mode.");
    // // } else {
    // //     Serial.println("Failed to confirm BIC is in discharging mode.");
    // // }

if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    command.trim();

    if (command.equalsIgnoreCase("UPDATE")) {
      handleUpdateProcess();
      serialEnableOutput();
    }
  }
  readBICData(BIC0_ADDRESS, pcf8574);
  while(!readBMSData(batteryCurrent, batteryVoltage, batteryFailureLevel));
  handleFaultOutputsInit();
}