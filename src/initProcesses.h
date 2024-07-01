#ifndef initProcesses_h
#define initProcesses_h

#include "globals.h"            // Library to include all the global variables


// Functions to initialize the MCP2515
byte mcp2515_readRegister(byte address) {
    byte value;
    digitalWrite(SPI_CS_PIN, LOW); // Select MCP2515
    SPI.transfer(MCP2515_READ);    // Send read command
    SPI.transfer(address);         // Send address
    value = SPI.transfer(0x00);    // Read value
    digitalWrite(SPI_CS_PIN, HIGH); // Deselect MCP2515
    return value;
}

// Function to modify a register
void mcp2515_modifyRegister(byte address, byte mask, byte data) {
    digitalWrite(SPI_CS_PIN, LOW); // Select MCP2515
    SPI.transfer(MCP2515_BIT_MODIFY); // Send bit modify command
    SPI.transfer(address);             // Send address
    SPI.transfer(mask);                // Send mask
    SPI.transfer(data);                // Send data
    digitalWrite(SPI_CS_PIN, HIGH);    // Deselect MCP2515
}

void forceNormalMode() {
    byte mode = mcp2515_readRegister(CANSTAT) & 0xE0;
    if (mode != 0x00) {
        // Serial.println("Forcing MCP2515 to Normal Mode");
        mcp2515_modifyRegister(CANCTRL, 0xE0, 0x00);
        delay(10);
        mode = mcp2515_readRegister(CANSTAT) & 0xE0;
        if (mode == 0x00) {
            // Serial.println("MCP2515 in Normal Mode");
        } else {
            // Serial.print("Failed to enter Normal Mode, current mode: 0x");
            // Serial.println(mode, HEX);
        }
    }
}


// Configuring the system to turn on
void sendSystemConfig(uint8_t address) {
    uint32_t id = 0x0C0300 | address;  // Controller to BIC-2200 message ID with address (extended frame)
    unsigned char data[4];

    // Command code and parameters for SYSTEM_CONFIG
    data[0] = 0xC2; // SYSTEM_CONFIG command low byte
    data[1] = 0x00; // SYSTEM_CONFIG command high byte
    data[2] = 0x03; // Parameter low byte
    data[3] = 0x00; // Parameter high byte

    byte sndStat = CAN.sendMsgBuf(id, 1, 4, data);  // Send message with extended ID
    if (sndStat == CAN_OK) {
        // Serial.println("System Configuration Command Sent Successfully!");
    } else {
        // Serial.print("Error Sending System Configuration Command: ");
        // Serial.println(sndStat, HEX);
    }
}

void setCANCTRLBit(uint8_t address) {
    uint32_t id = 0x0C0300 | address;  // Controller to BIC-2200 message ID with address (extended frame)
    unsigned char data[4];

    // Command code and parameters for SYSTEM_CONFIG to set CAN_CTRL bit
    data[0] = 0xC2; // SYSTEM_CONFIG command low byte
    data[1] = 0x00; // SYSTEM_CONFIG command high byte
    data[2] = 0x00; // Parameter low byte to set CAN_CTRL bit to 0
    data[3] = 0x00; // Parameter high byte

    byte sndStat = CAN.sendMsgBuf(id, 1, 4, data);  // Send message with extended ID
    if (sndStat == CAN_OK) {
        Serial.println("CAN_CTRL Bit Set Successfully!");
    } else {
        Serial.print("Error Setting CAN_CTRL Bit: ");
        Serial.println(sndStat, HEX);
    }
}


// Setting the bidirectional battery mode
void setBidirectionalConfig(uint8_t address) {
    uint32_t id = 0x0C0300 | address;  // Controller to BIC-2200 message ID with address (extended frame)
    unsigned char data[4];

    // Command code and parameters for BIDIRECTIONAL_CONFIG
    data[0] = 0x40; // BIDIRECTIONAL_CONFIG command low byte
    data[1] = 0x01; // BIDIRECTIONAL_CONFIG command high byte
    data[2] = 0x01; // Parameter low byte for bi-direction battery mode
    data[3] = 0x00; // Parameter high byte

    byte sndStat = CAN.sendMsgBuf(id, 1, 4, data);  // Send message with extended ID
    if (sndStat == CAN_OK) {
        // Serial.println("Bidirectional Configuration Command Sent Successfully!");
    } else {
        // Serial.print("Error Sending Bidirectional Configuration Command: ");
        // Serial.println(sndStat, HEX);
    }
}

void turnDCOn(uint8_t address) {
    uint32_t id = 0x0C0300 | address;  // Controller to BIC-2200 message ID with address (extended frame)
    unsigned char data[4];

    // Command code and parameters for turning DC on
    data[0] = 0x00; // OP_CTRL command low byte
    data[1] = 0x00; // OP_CTRL command high byte
    data[2] = 0x01; // Parameter low byte to turn on DC
    data[3] = 0x00; // Parameter high byte

    byte sndStat = CAN.sendMsgBuf(id, 1, 4, data);  // Send message with extended ID
    if (sndStat == CAN_OK) {
        Serial.println("DC Turn On Command Sent Successfully!");
    } else {
        Serial.print("Error Sending DC Turn On Command: ");
        Serial.println(sndStat, HEX);
    }
}

void enBypass() {  
  digitalWrite(swEN, HIGH); // Start with swEN enabled
}

void initCAN() {
    // Initialize CAN with Normal mode using library's function
    if (CAN.begin(MCP_ANY, CAN_250KBPS, MCP_20MHZ) == CAN_OK) {
        // Serial.println("CAN BUS Shield init ok!");
    } else {
        // Serial.println("CAN BUS Shield init fail");
        while (1);
    }
}

void initPCF() {
      // Initialize the PCF8574
  if (!pcf8574.begin()) {
    Serial.println("Failed to initialize PCF8574");
    while (1);
  }
}

void initGPIO() {
  pinMode(BAT_OC_PIN, OUTPUT);
  pinMode(BAT_OV_PIN, OUTPUT);
  pinMode(BAT_UV_PIN, OUTPUT);
  pinMode(BAT_OT_PIN, OUTPUT);
  pinMode(nRelay, OUTPUT);
  pinMode(pRelay, OUTPUT);
  pinMode(swEN, OUTPUT);

  digitalWrite(BAT_OC_PIN, LOW);
  digitalWrite(BAT_OV_PIN, LOW);
  digitalWrite(BAT_UV_PIN, LOW);
  digitalWrite(BAT_OT_PIN, LOW);
  digitalWrite(nRelay, LOW);
  digitalWrite(pRelay, LOW);
  digitalWrite(swEN, LOW);
}






#endif