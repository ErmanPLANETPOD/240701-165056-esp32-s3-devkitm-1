// This file contains all the functions to control the PCF
// Created by: Erman Ergül
// Created on: 7-6-2024
// Last modified: 7-6-2024

#ifndef errorProcessing_h
#define errorProcessing_h

#include "globals.h"            // Library to include all the global variables
#include "pcfComm.h"            // Library to include writing remoteCTRL

// Function to compare the data errors against thresholds
void checkDataErrors() {
  dataTempError = ambTemperatureRead > dataTempThreshold;
  Serial.print("Data Temperature Error: ");
  Serial.println(dataTempError);
  dataVoltageError = DCVoltageRead > dataOverVoltageThreshold;
  Serial.print("Data Overvoltage Error: ");
  Serial.println(dataVoltageError);
  dataCurrentError = abs(DCCurrentRead) > dataCurrentThreshold;
  Serial.print("Data Current Error: ");
  Serial.println(dataCurrentError);
  dataUnderVoltageError = DCVoltageRead < dataUnderVoltageThreshold;
  Serial.print("Data Undervoltage Error: ");
  Serial.println(dataUnderVoltageError);
  batteryCurrentError = abs(batteryCurrent) > dataCurrentThreshold;
  Serial.print("Battery Current Error: ");
  Serial.println(batteryCurrentError);
  if (batteryCurrentError) {
    digitalWrite(BAT_OC_PIN, HIGH);
  } else {
    digitalWrite(BAT_OC_PIN, LOW);
  }
  batteryOvervoltageError = batteryVoltage > dataOverVoltageThreshold;
  Serial.print("Battery Overvoltage Error: ");
  Serial.println(batteryOvervoltageError);
  if (batteryOvervoltageError) {
    digitalWrite(BAT_OV_PIN, HIGH);
  } else {
    digitalWrite(BAT_OV_PIN, LOW);
  }
  batteryUndervoltageError = batteryVoltage < dataUnderVoltageThreshold;
  Serial.print("Battery Undervoltage Error: ");
  Serial.println(batteryUndervoltageError);
  if (batteryUndervoltageError) {
    digitalWrite(BAT_UV_PIN, HIGH);
  } else {
    digitalWrite(BAT_UV_PIN, LOW);
  }
}


// Function to shut down the BIC-2200-48CAN if any error is detected
void handleFaultOutputsInit() {
    bool shutdown = false;

    // Check hardware error indicators
    if (tempB0hw) { //  || faultB0hw
        shutdown = true;
    } else {
      Serial.println("Hardware errors OK");
    }

    // Check data error flags
    checkDataErrors();
    if (dataTempError || dataVoltageError || batteryCurrentError || batteryOvervoltageError) {
        shutdown = true;
    } else {
      Serial.println("Data errors OK");
    }

    // Check fault status flags
    if (faultStatus.OTP || faultStatus.OVP || faultStatus.OVC || faultStatus.ACFail || faultStatus.fanFail || faultStatus.highTemperature || faultStatus.SCP) {
        shutdown = true;
    } else {
      Serial.println("BIC Status OK");
    }

    if (batteryFailureLevel == 0x01 || batteryFailureLevel == 0x03) {
      shutdown = true;
    } else {
      Serial.println("Battery Failure Level not critical");
    }

    // Send shutdown command if any error is detected
    if (shutdown) {
        remoteCTRL = 0;
        writePCF(pcf8574, remoteCTRL);
        Serial.println("Start-up error, system shutting down..");
        while(1);
    } else {
      remoteCTRL = 1;
    }
}

void handleFaultOutputs() {
    bool shutdownOp = false;

    // Check hardware error indicators
    if (tempB0hw || faultB0hw || DCB0hw) {
        shutdownOp = true;
    } else {
      Serial.println("Hardware errors OK");
    }

    // Check data error flags
    checkDataErrors();
    if (dataTempError || dataVoltageError || dataCurrentError || batteryCurrentError || dataUnderVoltageError || batteryOvervoltageError || batteryUndervoltageError) {
        shutdownOp = true;
    } else {
      Serial.println("Data errors OK");
    }

    // Check fault status flags
    if (faultStatus.OTP || faultStatus.OVP || faultStatus.OVC || faultStatus.DCStatus || faultStatus.ACFail || faultStatus.fanFail || faultStatus.highTemperature || faultStatus.SCP) {
        shutdownOp = true;
    } else {
      Serial.println("BIC Status OK");
    }

    if (batteryFailureLevel == 0x01 || batteryFailureLevel == 0x03) {
      shutdownOp = true;
    } else {
      Serial.println("Battery Failure Level not critical");
    }

    // Send shutdown command if any error is detected
    if (shutdownOp) {
        remoteCTRL = 0;
        writePCF(pcf8574, remoteCTRL);
        Serial.println("System shutting down due to error..");
        while(1);
    } else {
      remoteCTRL = 1;
    }
}

// void PUS2Gate(Vdc, Vbat, Idc) {
//   bool shutdownPUS2 = false;

//   if (Vdc - Vbat > 1 || Vbat - Vdc > 1) {
//     shutdownPUS2 = true;
//     Serial.println("Voltage difference too great..");
//   } else {
//     Serial.println("Voltages equal");
//   }
//   if (Idc > 0.5) {
//     shutdownPUS2 = true;
//     Serial.println("Unnecessary current flow..");
//   } else {
//     Serial.println("Current flow as anticipated");
//   }

//   if (shutdownPUS2) {
//     remoteCTRL = 0;
//     writePCF(pcf8574, remoteCTRL);
//     pinMode(nRelay, OUTPUT);
//     digitalWrite(nRelay, LOW);
//     delay(500);
//     pinMode(pRelay, OUTPUT);
//     digitalWrite(pRelay, LOW);
//     Serial.println("System shutting down due to implausibility at PUS2..")
//     while(1);
//   } else {
//       remoteCTRL = 1;
//       writePCF(pcf8574, remoteCTRL);
//       pinMode(nRelay, OUTPUT);
//       digitalWrite(nRelay, HIGH);
//       delay(500);
//       pinMode(pRelay, OUTPUT);
//       digitalWrite(pRelay, HIGH);
//   }

// }

#endif
