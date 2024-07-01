// This file contains all the functions required to set the current and voltage settings of the charger
// Created by: Erman Ergül
// Created on: 6-6-2024
// Last modified: 6-6-2024

#ifndef monitorBIC_h
#define monitorBIC_h

#include "globals.h"            // Library to include all the global variables
#include "pcfComm.h"            // Library to include the functions to read hardware data

// Reading the DC Voltage of the BIC
float readVout(uint8_t address) {
    uint32_t id = 0x0C0300 | address;  // Controller to BIC-2200 message ID with address (extended frame)
    unsigned char data[2] = {0x60, 0x00};  // READ_VOUT command (0x0060 for DC voltage)

    byte sndStat = CAN.sendMsgBuf(id, 1, 2, data);  // Send message with extended ID
    if (sndStat == CAN_OK) {
        // Serial.println("Read VOUT Command Sent Successfully!");
    } else {
        // Serial.print("Error Sending Read VOUT Command: ");
        // Serial.println(sndStat, HEX);
        return -1; // Indicate an error
    }

    unsigned long timeout = millis() + 1000; // Timeout after 1 second
    while (millis() < timeout) {
        if (CAN_MSGAVAIL == CAN.checkReceive()) {
            long unsigned int rxId;
            byte len;
            unsigned char rxBuf[8];
            CAN.readMsgBuf(&rxId, &len, rxBuf);
            if (rxId == (0x800C0200 | address)) {  // Expect response from BIC-2200 with address
                // Correctly combine the bytes for voltage
                uint16_t voltageRaw = (rxBuf[3] << 8) | rxBuf[2];  // Combine bytes 3 and 2
                DCVoltageRead = voltageRaw * 0.01; // Assuming a scaling factor of 0.01
                Serial.print("VOUT: ");
                Serial.print(DCVoltageRead);
                Serial.println(" V");
                return DCVoltageRead;
            } else {
                // Serial.print("Received message with unexpected ID: 0x");
                // Serial.println(rxId, HEX);
            }
        }
    }
    Serial.println("Timeout waiting for VOUT response");
    return -1; // Indicate timeout error
}


// Reading the AC Voltage of the BIC
float readVin(uint8_t address) {
    uint32_t id = 0x0C0300 | address;  // Controller to BIC-2200 message ID with address (extended frame)
    unsigned char data[2] = {0x50, 0x00};  // READ_VIN command (0x0050 for AC voltage)

    byte sndStat = CAN.sendMsgBuf(id, 1, 2, data);  // Send message with extended ID
    if (sndStat == CAN_OK) {
        // Serial.println("Read VIN Command Sent Successfully!");
    } else {
        // Serial.print("Error Sending Read VIN Command: ");
        // Serial.println(sndStat, HEX);
        return -1; // Indicate an error
    }

    unsigned long timeout = millis() + 1000; // Timeout after 1 second
    while (millis() < timeout) {
        if (CAN_MSGAVAIL == CAN.checkReceive()) {
            long unsigned int rxId;
            byte len;
            unsigned char rxBuf[8];
            CAN.readMsgBuf(&rxId, &len, rxBuf);
            if (rxId == (0x800C0200 | address)) {  // Expect response from BIC-2200 with address
                // Correctly combine the bytes for voltage
                uint16_t voltageRaw = (rxBuf[3] << 8) | rxBuf[2];  // Combine bytes 3 and 2
                ACVoltageRead = voltageRaw * 0.1; // Assuming a scaling factor of 0.1
                Serial.print("VIN: ");
                Serial.print(ACVoltageRead);
                Serial.println(" V");
                return ACVoltageRead;
            } else {
                // Serial.print("Received message with unexpected ID: 0x");
                // Serial.println(rxId, HEX);
            }
        }
    }
    Serial.println("Timeout waiting for VIN response");
    return -1; // Indicate timeout error
}



// Reading the Temperature of the BIC
float readTemperature(uint8_t address) {
    uint32_t id = 0x0C0300 | address;  // Controller to BIC-2200 message ID with address (extended frame)
    unsigned char data[2] = {0x62, 0x00};  // READ_TEMP command (0x0070 for temperature)

    byte sndStat = CAN.sendMsgBuf(id, 1, 2, data);  // Send message with extended ID
    if (sndStat == CAN_OK) {
        // Serial.println("Read Temperature Command Sent Successfully!");
    } else {
        // Serial.print("Error Sending Read Temperature Command: ");
        // Serial.println(sndStat, HEX);
        return -1; // Indicate an error
    }

    unsigned long timeout = millis() + 1000; // Timeout after 1 second
    while (millis() < timeout) {
        if (CAN_MSGAVAIL == CAN.checkReceive()) {
            long unsigned int rxId;
            byte len;
            unsigned char rxBuf[8];
            CAN.readMsgBuf(&rxId, &len, rxBuf);
            if (rxId == (0x800C0200 | address)) {  // Expect response from BIC-2200 with address
                // Correctly combine the bytes for temperature
                uint16_t tempRaw = (rxBuf[3] << 8) | rxBuf[2];  // Combine bytes 3 and 2
                ambTemperatureRead = tempRaw * 0.1; // Assuming a scaling factor of 0.01
                Serial.print("Temperature: ");
                Serial.print(ambTemperatureRead);
                Serial.println(" °C");
                return ambTemperatureRead;
            } else {
                // Serial.print("Received message with unexpected ID: 0x");
                // Serial.println(rxId, HEX);
            }
        }
    }
    Serial.println("Timeout waiting for Temperature response");
    return -1; // Indicate timeout error
}



// Function to read the DC Current
// Function to read the DC Current
float readIout(uint8_t address) {
    uint32_t id = 0x0C0300 | address;  // Controller to BIC-2200 message ID with address (extended frame)
    unsigned char data[2] = {0x61, 0x00};  // READ_IOUT command (0x0061 for DC current)
    DCCurrentRead = 0.0; // Initialize the current read variable

    byte sndStat = CAN.sendMsgBuf(id, 1, 2, data);  // Send message with extended ID
    if (sndStat != CAN_OK) {
        Serial.print("Error Sending Read IOUT Command: ");
        Serial.println(sndStat, HEX);
        return -1; // Indicate an error
    }

    unsigned long timeout = millis() + 1000; // Timeout after 1 second
    while (millis() < timeout) {
        if (CAN_MSGAVAIL == CAN.checkReceive()) {
            long unsigned int rxId;
            byte len;
            unsigned char rxBuf[8];
            CAN.readMsgBuf(&rxId, &len, rxBuf);
            if (rxId == (0x800C0200 | address)) {  // Expect response from BIC-2200 with address
                // Debugging print statements to understand the data received
                Serial.print("Raw data: ");
                for (int i = 0; i < len; i++) {
                    Serial.print(rxBuf[i], HEX);
                    Serial.print(" ");
                }
                Serial.println();

                // Combine bytes 2 and 3 for current
                int16_t currentRaw = (rxBuf[3] << 8) | rxBuf[2]; // Interpret as signed integer
                DCCurrentRead = currentRaw * 0.01; // Assuming a scaling factor of 0.01

                Serial.print("Raw Current Data: ");
                Serial.print(currentRaw);
                Serial.print(" | IOUT: ");
                Serial.print(DCCurrentRead);
                Serial.println(" A");

                // Additional check to see if current reading makes sense
                if (abs(DCCurrentRead) > 1000) { // Adjust threshold as necessary
                    Serial.println("Warning: Unusually high current reading!");
                }

                return DCCurrentRead;
            } else {
                Serial.print("Received message with unexpected ID: 0x");
                Serial.println(rxId, HEX);
            }
        }
    }
    Serial.println("Timeout waiting for IOUT response");
    return -1; // Indicate timeout error
}






// Reading the Fault Status of the BIC
FaultStatus readFaultStatus(uint8_t address) {
    uint32_t id = 0x0C0300 | address;  // Controller to BIC-2200 message ID with address (extended frame)
    unsigned char data[2] = {0x40, 0x00};  // READ_FAULT_STATUS command (0x0040 for fault status)

    byte sndStat = CAN.sendMsgBuf(id, 1, 2, data);  // Send message with extended ID
    if (sndStat == CAN_OK) {
        // Serial.println("Read Fault Status Command Sent Successfully!");
    } else {
        // Serial.print("Error Sending Read Fault Status Command: ");
        // Serial.println(sndStat, HEX);
        return {};
    }

    unsigned long timeout = millis() + 1000; // Timeout after 1 second
    while (millis() < timeout) {
        if (CAN_MSGAVAIL == CAN.checkReceive()) {
            long unsigned int rxId;
            byte len;
            unsigned char rxBuf[8];
            CAN.readMsgBuf(&rxId, &len, rxBuf);
            if (rxId == (0x800C0200 | address)) {  // Expect response from BIC-2200 with address
                uint16_t faultStatusRaw = (rxBuf[3] << 8) | rxBuf[2];  // Combine bytes 3 and 2

                FaultStatus faultStatus;
                faultStatus.fanFail = faultStatusRaw & 0x01;
                faultStatus.OTP = faultStatusRaw & 0x02;
                faultStatus.OVP = faultStatusRaw & 0x04;
                faultStatus.OVC = faultStatusRaw & 0x08;
                faultStatus.SCP = faultStatusRaw & 0x10;
                faultStatus.ACFail = faultStatusRaw & 0x20;
                faultStatus.DCStatus = faultStatusRaw & 0x40;
                faultStatus.highTemperature = faultStatusRaw & 0x80;

                Serial.print("Fault Status: ");
                Serial.println(faultStatusRaw, HEX);  // Print fault status in hexadecimal format

                // Print each bit with its description
                Serial.print("Bit 0 - FAN_FAIL (Fan locked flag): ");
                Serial.println(faultStatus.fanFail);

                Serial.print("Bit 1 - OTP (Over temperature protection): ");
                Serial.println(faultStatus.OTP);

                Serial.print("Bit 2 - OVP (DC over voltage protection): ");
                Serial.println(faultStatus.OVP);

                Serial.print("Bit 3 - OLP (DC over current protection): ");
                Serial.println(faultStatus.OVC);

                Serial.print("Bit 4 - SHORT (Short circuit protection): ");
                Serial.println(faultStatus.SCP);

                Serial.print("Bit 5 - AC_FAIL (AC abnormal flag): ");
                Serial.println(faultStatus.ACFail);

                Serial.print("Bit 6 - OP_OFF (DC status): ");
                Serial.println(faultStatus.DCStatus);

                Serial.print("Bit 7 - HI_TEMP (Internal high temperature alarm): ");
                Serial.println(faultStatus.highTemperature);

                return faultStatus;
            } else {
                // Serial.print("Received message with unexpected ID: 0x");
                // Serial.println(rxId, HEX);
            }
        }
    }
    Serial.println("Timeout waiting for Fault Status response");
    return {};
}

void readBICData(uint8_t address, PCF8574 &pcf8574) {
    readVout(address);
    readVin(address);
    readIout(address);
    readTemperature(address);
    readFaultStatus(address);
    readPCF(pcf8574);
    
  Serial.print("Vdc:");
  Serial.print(DCVoltageRead);
  Serial.print(",Vac:");
  Serial.print(ACVoltageRead);
  Serial.print(",Temp:");
  Serial.print(ambTemperatureRead);
  Serial.print(",Idc:");
  Serial.print(DCCurrentRead);
  Serial.print(",fanFail:");
  Serial.print(faultStatus.fanFail);
  Serial.print(",OTP:");
  Serial.print(faultStatus.OTP);
  Serial.print(",OVP:");
  Serial.print(faultStatus.OVP);
  Serial.print(",OVC:");
  Serial.print(faultStatus.OVC);
  Serial.print(",SCP:");
  Serial.print(faultStatus.SCP);
  Serial.print(",ACFail:");
  Serial.print(faultStatus.ACFail);
  Serial.print(",DCStatus:");
  Serial.print(faultStatus.DCStatus);
  Serial.print(",highTemperature:");
  Serial.println(faultStatus.highTemperature);
}

// Function to read the set current (IOUT_SET)
float readIOUT_SET(uint8_t address) {
    uint32_t id = 0x0C0300 | address;  // Controller to BIC-2200 message ID with address (extended frame)
    unsigned char data[2] = {0x30, 0x00};  // READ_IOUT_SET command (0x3000 for current level)
    float setCurrent = 0.0; // Initialize the current set variable

    byte sndStat = CAN.sendMsgBuf(id, 1, 2, data);  // Send message with extended ID
    if (sndStat != CAN_OK) {
        Serial.print("Error Sending Read IOUT_SET Command: ");
        Serial.println(sndStat, HEX);
        return -1; // Indicate an error
    }

    unsigned long timeout = millis() + 1000; // Timeout after 1 second
    while (millis() < timeout) {
        if (CAN_MSGAVAIL == CAN.checkReceive()) {
            long unsigned int rxId;
            byte len;
            unsigned char rxBuf[8];
            CAN.readMsgBuf(&rxId, &len, rxBuf);
            if (rxId == (0x800C0200 | address)) {  // Expect response from BIC-2200 with address
                // Debugging print statements to understand the data received
                Serial.print("Raw data: ");
                for (int i = 0; i < len; i++) {
                    Serial.print(rxBuf[i], HEX);
                    Serial.print(" ");
                }
                Serial.println();

                // Check if the command code is correct
                if (rxBuf[0] == 0x30 && rxBuf[1] == 0x00) {
                    // Combine bytes 2 and 3 for current set value
                    uint16_t currentRaw = (rxBuf[3] << 8) | rxBuf[2];
                    setCurrent = currentRaw * 0.01; // Assuming a scaling factor of 0.01

                    Serial.print("Raw Set Current Data: ");
                    Serial.print(currentRaw);
                    Serial.print(" | Set Current: ");
                    Serial.print(setCurrent);
                    Serial.println(" A");

                    return setCurrent;
                } else {
                    Serial.println("Received message with unexpected command code");
                }
            } else {
                Serial.print("Received message with unexpected ID: 0x");
                Serial.println(rxId, HEX);
            }
        }
    }
    Serial.println("Timeout waiting for IOUT_SET response");
    return -1; // Indicate timeout error
}

// Function to read the reverse set current (REVERSE_IOUT_SET)
float readReverseIOUT_SET(uint8_t address) {
    uint32_t id = 0x0C0300 | address;  // Controller to BIC-2200 message ID with address (extended frame)
    unsigned char data[2] = {0x30, 0x01};  // READ_REVERSE_IOUT_SET command (0x0130 for reverse current level)
    float reverseSetCurrent = 0.0; // Initialize the reverse current set variable

    byte sndStat = CAN.sendMsgBuf(id, 1, 2, data);  // Send message with extended ID
    if (sndStat != CAN_OK) {
        Serial.print("Error Sending Read REVERSE_IOUT_SET Command: ");
        Serial.println(sndStat, HEX);
        return -1; // Indicate an error
    }

    unsigned long timeout = millis() + 1000; // Timeout after 1 second
    while (millis() < timeout) {
        if (CAN_MSGAVAIL == CAN.checkReceive()) {
            long unsigned int rxId;
            byte len;
            unsigned char rxBuf[8];
            CAN.readMsgBuf(&rxId, &len, rxBuf);
            if (rxId == (0x800C0200 | address)) {  // Expect response from BIC-2200 with address
                // Debugging print statements to understand the data received
                Serial.print("Raw data: ");
                for (int i = 0; i < len; i++) {
                    Serial.print(rxBuf[i], HEX);
                    Serial.print(" ");
                }
                Serial.println();

                // Check if the command code is correct
                if (rxBuf[0] == 0x30 && rxBuf[1] == 0x01) {
                    // Combine bytes 2 and 3 for reverse current set value
                    int16_t currentRaw = (rxBuf[3] << 8) | rxBuf[2];
                    reverseSetCurrent = currentRaw * 0.01; // Assuming a scaling factor of 0.01

                    Serial.print("Raw Reverse Set Current Data: ");
                    Serial.print(currentRaw);
                    Serial.print(" | Reverse Set Current: ");
                    Serial.print(reverseSetCurrent);
                    Serial.println(" A");

                    return reverseSetCurrent;
                } else {
                    Serial.println("Received message with unexpected command code");
                }
            } else {
                Serial.print("Received message with unexpected ID: 0x");
                Serial.println(rxId, HEX);
            }
        }
    }
    Serial.println("Timeout waiting for REVERSE_IOUT_SET response");
    return -1; // Indicate timeout error
}

// Function to read the set voltage (VOUT_SET)
float readVOUT_SET(uint8_t address) {
    uint32_t id = 0x0C0300 | address;  // Controller to BIC-2200 message ID with address (extended frame)
    unsigned char data[2] = {0x20, 0x00};  // READ_VOUT_SET command (0x0020 for voltage level)
    float setVoltage = 0.0; // Initialize the voltage set variable

    byte sndStat = CAN.sendMsgBuf(id, 1, 2, data);  // Send message with extended ID
    if (sndStat != CAN_OK) {
        Serial.print("Error Sending Read VOUT_SET Command: ");
        Serial.println(sndStat, HEX);
        return -1; // Indicate an error
    }

    unsigned long timeout = millis() + 1000; // Timeout after 1 second
    while (millis() < timeout) {
        if (CAN_MSGAVAIL == CAN.checkReceive()) {
            long unsigned int rxId;
            byte len;
            unsigned char rxBuf[8];
            CAN.readMsgBuf(&rxId, &len, rxBuf);
            if (rxId == (0x800C0200 | address)) {  // Expect response from BIC-2200 with address
                // Debugging print statements to understand the data received
                Serial.print("Raw data: ");
                for (int i = 0; i < len; i++) {
                    Serial.print(rxBuf[i], HEX);
                    Serial.print(" ");
                }
                Serial.println();

                // Check if the command code is correct
                if (rxBuf[0] == 0x20 && rxBuf[1] == 0x00) {
                    // Combine bytes 2 and 3 for voltage set value
                    uint16_t voltageRaw = (rxBuf[3] << 8) | rxBuf[2];
                    setVoltage = voltageRaw * 0.01; // Assuming a scaling factor of 0.01

                    Serial.print("Raw Set Voltage Data: ");
                    Serial.print(voltageRaw);
                    Serial.print(" | Set Voltage: ");
                    Serial.print(setVoltage);
                    Serial.println(" V");

                    return setVoltage;
                } else {
                    Serial.println("Received message with unexpected command code");
                }
            } else {
                Serial.print("Received message with unexpected ID: 0x");
                Serial.println(rxId, HEX);
            }
        }
    }
    Serial.println("Timeout waiting for VOUT_SET response");
    return -1; // Indicate timeout error
}

// Function to read the reverse set voltage (reverseVOUT_SET)
float readReverseVOUT_SET(uint8_t address) {
    uint32_t id = 0x0C0300 | address;  // Controller to BIC-2200 message ID with address (extended frame)
    unsigned char data[2] = {0x20, 0x01};  // READ_REVERSE_VOUT_SET command (0x0120 for reverse voltage level)
    float reverseSetVoltage = 0.0; // Initialize the reverse voltage set variable

    byte sndStat = CAN.sendMsgBuf(id, 1, 2, data);  // Send message with extended ID
    if (sndStat != CAN_OK) {
        Serial.print("Error Sending Read REVERSE_VOUT_SET Command: ");
        Serial.println(sndStat, HEX);
        return -1; // Indicate an error
    }

    unsigned long timeout = millis() + 1000; // Timeout after 1 second
    while (millis() < timeout) {
        if (CAN_MSGAVAIL == CAN.checkReceive()) {
            long unsigned int rxId;
            byte len;
            unsigned char rxBuf[8];
            CAN.readMsgBuf(&rxId, &len, rxBuf);
            if (rxId == (0x800C0200 | address)) {  // Expect response from BIC-2200 with address
                // Debugging print statements to understand the data received
                Serial.print("Raw data: ");
                for (int i = 0; i < len; i++) {
                    Serial.print(rxBuf[i], HEX);
                    Serial.print(" ");
                }
                Serial.println();

                // Check if the command code is correct
                if (rxBuf[0] == 0x20 && rxBuf[1] == 0x01) {
                    // Combine bytes 2 and 3 for reverse voltage set value
                    uint16_t voltageRaw = (rxBuf[3] << 8) | rxBuf[2];
                    reverseSetVoltage = voltageRaw * 0.01; // Assuming a scaling factor of 0.01

                    Serial.print("Raw Reverse Set Voltage Data: ");
                    Serial.print(voltageRaw);
                    Serial.print(" | Reverse Set Voltage: ");
                    Serial.print(reverseSetVoltage);
                    Serial.println(" V");

                    return reverseSetVoltage;
                } else {
                    Serial.println("Received message with unexpected command code");
                }
            } else {
                Serial.print("Received message with unexpected ID: 0x");
                Serial.println(rxId, HEX);
            }
        }
    }
    Serial.println("Timeout waiting for REVERSE_VOUT_SET response");
    return -1; // Indicate timeout error
}



#endif