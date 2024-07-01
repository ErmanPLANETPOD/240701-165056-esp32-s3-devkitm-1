// This file contains all the functions required to set the current and voltage settings of the charger
// Created by: Erman Ergül
// Created on: 6-6-2024
// Last modified: 6-6-2024

#ifndef setPower_h
#define setPower_h

#include "globals.h"            // Library to include all the global variables

// Function to set the output DC voltage of the BIC.
void setVout(uint8_t address, float voltage) {
    uint32_t id = 0x0C0300 | address;  // Controller to BIC-2200 message ID with address (extended frame)
    unsigned char data[4];

    // Convert the voltage to the scaled value
    uint16_t scaledVoltage = static_cast<uint16_t>(voltage * 100);  // Apply scaling factor of 0.01

    // Split the scaled voltage into low and high bytes
    data[0] = 0x20; // VOUT_SET command low byte
    data[1] = 0x00; // VOUT_SET command high byte
    data[2] = scaledVoltage & 0xFF; // Parameter low byte
    data[3] = (scaledVoltage >> 8) & 0xFF; // Parameter high byte

    byte sndStat = CAN.sendMsgBuf(id, 1, 4, data);  // Send message with extended ID
    if (sndStat == CAN_OK) {
        // Serial.println("VOUT_SET Command Sent Successfully!");
    } else {
        // Serial.print("Error Sending VOUT_SET Command: ");
        // Serial.println(sndStat, HEX);
    }
}

// Function to set the reversed output voltage of the BIC.
void setReverseVout(uint8_t address, float disvoltage) {
    uint32_t id = 0x0C0300 | address;  // Controller to BIC-2200 message ID with address (extended frame)
    unsigned char data[4];

    // Convert the voltage to the scaled value
    uint16_t scaleddisVoltage = static_cast<uint16_t>(disvoltage * 100);  // Apply scaling factor of 0.01

    // Split the scaled voltage into low and high bytes
    data[0] = 0x20; // REVERSE_VOUT_SET command low byte
    data[1] = 0x01; // REVERSE_VOUT_SET command high byte
    data[2] = scaleddisVoltage & 0xFF; // Parameter low byte
    data[3] = (scaleddisVoltage >> 8) & 0xFF; // Parameter high byte

    byte sndStat = CAN.sendMsgBuf(id, 1, 4, data);  // Send message with extended ID
    if (sndStat == CAN_OK) {
        // Serial.println("REVERSE_VOUT_SET Command Sent Successfully!");
    } else {
        // Serial.print("Error Sending REVERSE_VOUT_SET Command: ");
        // Serial.println(sndStat, HEX);
    }
}


// Function to set the charging current of the BIC
void setIout(uint8_t address, float current) {
    uint32_t id = 0x0C0300 | address;  // Controller to BIC-2200 message ID with address (extended frame)
    unsigned char data[4];

    // Convert the current to the scaled value
    uint16_t scaledCurrent = static_cast<uint16_t>(current * 100);  // Apply scaling factor of 0.01

    // Split the scaled current into low and high bytes
    data[0] = 0x30; // IOUT_SET command low byte
    data[1] = 0x00; // IOUT_SET command high byte
    data[2] = scaledCurrent & 0xFF; // Parameter low byte (hexadecimal representation of the lower 8 bits)
    data[3] = (scaledCurrent >> 8) & 0xFF; // Parameter high byte (hexadecimal representation of the upper 8 bits)

    byte sndStat = CAN.sendMsgBuf(id, 1, 4, data);  // Send message with extended ID
    if (sndStat == CAN_OK) {
        // Serial.println("IOUT_SET Command Sent Successfully!");
    } else {
        // Serial.print("Error Sending IOUT_SET Command: ");
        // Serial.println(sndStat, HEX);
    }
}


// Function to set the discharging current of the BIC
void setReverseIout(uint8_t address, float dcurrent) {
    uint32_t id = 0x0C0300 | address;  // Controller to BIC-2200 message ID with address (extended frame)
    unsigned char data[4];

    // Convert the current to the scaled value
    uint16_t scaled_dCurrent = static_cast<uint16_t>(dcurrent * 100);  // Apply scaling factor of 0.01

    // Split the scaled current into low and high bytes
    data[0] = 0x30; // REVERSE_IOUT_SET command low byte
    data[1] = 0x01; // REVERSE_IOUT_SET command high byte
    data[2] = scaled_dCurrent & 0xFF; // Parameter low byte (hexadecimal representation of the lower 8 bits)
    data[3] = (scaled_dCurrent >> 8) & 0xFF; // Parameter high byte (hexadecimal representation of the upper 8 bits)

    byte sndStat = CAN.sendMsgBuf(id, 1, 4, data);  // Send message with extended ID
    if (sndStat == CAN_OK) {
        // Serial.println("REVERSE_IOUT_SET Command Sent Successfully!");
    } else {
        // Serial.print("Error Sending REVERSE_IOUT_SET Command: ");
        // Serial.println(sndStat, HEX);
    }
}

void setChargingParams(uint8_t address, float voltage, float current) {
    // Set charging voltage
    setVout(address, voltage);
    delay(500); // Allow some time for the BIC to process the command

    // Set charging current
    setIout(address, current);
    delay(500); // Allow some time for the BIC to process the command
}

void setDischargingParams(uint8_t address, float voltage, float current) {
    // Set discharging voltage
    setReverseVout(address, voltage);
    delay(500); // Allow some time for the BIC to process the command

    // Set discharging current
    setReverseIout(address, current);
    delay(500); // Allow some time for the BIC to process the command
}


#endif