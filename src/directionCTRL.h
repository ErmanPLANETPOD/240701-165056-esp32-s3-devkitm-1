// This file contains all the functions required to set the direction of conversion
// Created by: Erman Ergül
// Created on: 6-6-2024
// Last modified: 6-6-2024

#ifndef directionCTRL_h
#define directionCTRL_h

#include "globals.h"            // Library to include all the global variables

void setDirection(uint8_t address, bool charge) {
    uint32_t id = 0x0C0300 | address;  // Controller to BIC-2200 message ID with address (extended frame)
    unsigned char data[3];

    if (charge) {
        // CD = 1; // Set to charging mode
        data[0] = 0x00; // DIRECTION_CTRL command low byte
        data[1] = 0x01; // DIRECTION_CTRL command high byte
        data[2] = 0x00; // Parameter low byte for charging (A/D)
    } else {
        // CD = 0; // Set to discharging mode
        data[0] = 0x00; // DIRECTION_CTRL command low byte
        data[1] = 0x01; // DIRECTION_CTRL command high byte
        data[2] = 0x01; // Parameter low byte for discharging (D/A)
    }

    byte sndStat = CAN.sendMsgBuf(id, 1, 3, data); // Send message with extended ID
    if (sndStat == CAN_OK) {
        Serial.print("Direction Control Command Sent for ");
        Serial.println(charge ? "Charging" : "Discharging");
    } else {
        Serial.print("Error Sending Direction Control Command for ");
        Serial.print(charge ? "Charging: " : "Discharging: ");
        Serial.println(sndStat, HEX);
    }
}
#endif