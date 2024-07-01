#ifndef monitorBMS_h
#define monitorBMS_h

#include "globals.h" // Library to include all the global variables

bool parseBMSData(unsigned long id, unsigned char *buf, unsigned char len, float &batteryCurrent, float &batteryVoltage, int &batteryFailureLevel) {
  if (id == 0x18FF28F4 && len == 8) {
    // Byte 1
    bool chargeWireConnected = bitRead(buf[0], 0);
    bool batteryCharging = bitRead(buf[0], 1);
    bool batteryLowSOC = bitRead(buf[0], 2);
    bool batteryReady = bitRead(buf[0], 3);
    bool dischargeMOSStatus = bitRead(buf[0], 4);
    bool chargeMOSStatus = bitRead(buf[0], 5);

    Serial.print("chargeWireConnected:"); Serial.print(chargeWireConnected);
    Serial.print(",batteryCharging:"); Serial.print(batteryCharging);
    Serial.print(",batteryLowSOC:"); Serial.print(batteryLowSOC);
    Serial.print(",batteryReady:"); Serial.print(batteryReady);
    Serial.print(",dischargeMOSStatus:"); Serial.print(dischargeMOSStatus);
    Serial.print(",chargeMOSStatus:"); Serial.print(chargeMOSStatus);

    // Byte 2
    float SOC = buf[1];
    Serial.print(",SOC:"); Serial.print(SOC);
    Serial.print(",rawSOC:"); Serial.print(buf[1], HEX);

    // Byte 3-4
    batteryCurrent = (buf[2] | (buf[3] << 8)) - 5000; // Offset adjustment
    batteryCurrent *= 0.1; // 0.1A/bit
    Serial.print(",batteryCurrent:"); Serial.print(batteryCurrent, 3); // 0.1A/bit

    // Byte 5-6
    batteryVoltage = (buf[4] | (buf[5] << 8)) * 0.1; // 0.1V/bit
    Serial.print(",batteryVoltage:"); Serial.print(batteryVoltage, 3); // 0.1V/bit

    // Byte 7
    batteryFailureLevel = buf[6];
    Serial.print(",batteryFailureLevel:");
    switch (batteryFailureLevel) {
      case 0x00: Serial.print("0"); break;
      case 0x01: Serial.print("1"); break;
      case 0x02: Serial.print("2"); break;
      case 0x03: Serial.print("3"); break;
      default: Serial.print("Unknown"); break;
    }
    Serial.println();
    return true;
  } else {
    // Print unexpected CAN message
    Serial.print("Unexpected CAN ID: 0x");
    Serial.print(id, HEX);
    Serial.print(", Data Length: ");
    Serial.println(len);
    for (int i = 0; i < len; i++) {
      Serial.print(buf[i], HEX);
      Serial.print(" ");
    }
    Serial.println();
    return false;
  }
}


bool readBMSData(float &batteryCurrent, float &batteryVoltage, int &batteryFailureLevel) {
  unsigned long id;
  unsigned char len = 0;
  unsigned char buf[8];
  
  unsigned long timeout = millis() + 1000; // Timeout after 1 second
  while (millis() < timeout) {
    if (CAN_MSGAVAIL == CAN.checkReceive()) {
      CAN.readMsgBuf(&id, &len, buf);

      // Check if the frame is an extended frame
      if ((id & 0x80000000) == 0x80000000) {
        Serial.print("Extended frame message received with ID: 0x");
        id &= 0x1FFFFFFF; // Mask to get the actual ID
      } else {
        Serial.print("Standard frame message received with ID: 0x");
      }
      Serial.println(id, HEX);

      // Print message data
      Serial.print("Data: ");
      for (int i = 0; i < len; i++) {
        Serial.print(buf[i], HEX);
        Serial.print(" ");
      }
      Serial.println();

      // Parse the data based on the provided BMS protocol
      if (parseBMSData(id, buf, len, batteryCurrent, batteryVoltage, batteryFailureLevel)) {
        return true; // Data parsed correctly
      } else {
        Serial.println("Error parsing CAN data, possible noise corruption.");
        return false; // Data parsing error
      }
    }
  }
  Serial.println("Timeout waiting for BMS data");
  return false; // Indicate timeout error
}


#endif
