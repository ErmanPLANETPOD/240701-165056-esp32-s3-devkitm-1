// This file contains all the functions to control the PCF
// Created by: Erman Ergül
// Created on: 7-6-2024
// Last modified: 7-6-2024

#ifndef serialCommands_h
#define serialCommands_h

void waitForSerialInput() {
  Serial.println("Waiting for input...");
  while(!readBMSData(batteryCurrent, batteryVoltage, batteryFailureLevel));
  readBICData(BIC0_ADDRESS, pcf8574);
  handleFaultOutputsInit();
  while (true) {
    if (Serial.available() > 0) {
      String input = Serial.readStringUntil('\n'); // Read input until newline character
      input.trim(); // Remove any leading or trailing whitespace
      Serial.print("Received input: ");
      Serial.println(input);
      // Process the input if necessary
      // For now, simply break the loop and continue execution
      break;
    }
  }
}


void checkSerialCommands() {
    if (Serial.available() > 0) {
        String command = Serial.readStringUntil('\n'); // Read the incoming command
        command.trim(); // Remove any whitespace

        if (command.equalsIgnoreCase("OFF")) {
            remoteCTRL = 0;
            writePCF(pcf8574, remoteCTRL);
            Serial.println("The BIC has been turned OFF.");
            while (true) {
                if (Serial.available() > 0) {
                    String onCommand = Serial.readStringUntil('\n');
                    onCommand.trim();
                    if (onCommand.equalsIgnoreCase("ON")) {
                        remoteCTRL = 1;
                        writePCF(pcf8574, remoteCTRL);
                        Serial.println("The BIC has been turned ON.");
                        break; // Exit the while loop and resume normal operation
                    }
                }
                delay(100); // Small delay to avoid busy-waiting
            }
        }
    }
}


void serialSendConfig() {
  Serial.print("Continue with sending configuration command to BIC? Please enter: ");
  Serial.print("'YES'");
  Serial.println(" to continue..");

  while(true) {
  if (Serial.available() > 0) {
        String command = Serial.readStringUntil('\n'); // Read the incoming command
        command.trim(); // Remove any whitespace

        if (command.equalsIgnoreCase("YES")) {
            sendSystemConfig(BIC0_ADDRESS);
            break;
        }
    }
  }
}

void serialSetCANCTRLBit() {
  Serial.print("Continue with sending configuration command to BIC? Please enter: ");
  Serial.print("'YES'");
  Serial.println(" to continue..");

  while(true) {
  if (Serial.available() > 0) {
        String command = Serial.readStringUntil('\n'); // Read the incoming command
        command.trim(); // Remove any whitespace

        if (command.equalsIgnoreCase("YES")) {
            setCANCTRLBit(BIC0_ADDRESS);
            break;
        }
    }
  }
}

void serialBiDirectionalConfig() {
  Serial.print("Continue with sending bidirectional mode command to BIC? Please enter: ");
  Serial.print("'YES'");
  Serial.println(" to continue..");

  while(true) {
  if (Serial.available() > 0) {
        String command = Serial.readStringUntil('\n'); // Read the incoming command
        command.trim(); // Remove any whitespace

        if (command.equalsIgnoreCase("YES")) {
            setBidirectionalConfig(BIC0_ADDRESS);
            break;
        }
    }
  }
}

void serialEnableOutput() {
  Serial.print("Continue with enabling the output of the BIC, please double check data first? Please enter: ");
  Serial.print("'YES'");
  Serial.println(" to continue..");

  while(true) {
  if (Serial.available() > 0) {
        String command = Serial.readStringUntil('\n'); // Read the incoming command
        command.trim(); // Remove any whitespace

        if (command.equalsIgnoreCase("YES")) {
            writePCF(pcf8574, remoteCTRL);
            break;
        }
    }
  }
}

bool configureChargeDischarge() {
  while (true) {
    Serial.println("Do you want to discharge or charge? (Enter 'C' for charge or 'D' for discharge)");

    // Wait for user input
    while (!Serial.available());
    String mode = Serial.readStringUntil('\n');
    mode.trim();

    if (mode.equalsIgnoreCase("C")) {
      setDirection(BIC0_ADDRESS, true);
      writePCF(pcf8574, 0);
      Serial.println("You have selected CHARGE, type YES to confirm or NO to restart");

      while (true) {
        // Wait for user input
        while (!Serial.available());
        String confirm = Serial.readStringUntil('\n');
        confirm.trim();

        if (confirm.equalsIgnoreCase("YES")) {
          // Set voltage
          Serial.println("Please write in xx.x format what the SET_VOLTAGE must be:");
          while (!Serial.available());
          float serialSetVoltage = Serial.readStringUntil('\n').toFloat();
          setVout(BIC0_ADDRESS, serialSetVoltage);

          // Confirm set voltage
          float setVoltage = readVOUT_SET(BIC0_ADDRESS);
          Serial.print("Set Voltage: ");
          Serial.println(setVoltage);
          Serial.println("Is the set voltage correct? (type YES to confirm or NO to restart)");

          while (!Serial.available());
          confirm = Serial.readStringUntil('\n');
          confirm.trim();

          if (confirm.equalsIgnoreCase("YES")) {
            // Set current
            Serial.println("Please write in xx.x format what the SET_CURRENT must be:");
            while (!Serial.available());
            float serialSetCurrent = Serial.readStringUntil('\n').toFloat();
            setIout(BIC0_ADDRESS, serialSetCurrent);

            // Confirm set current
            float setCurrent = readIOUT_SET(BIC0_ADDRESS);
            Serial.print("Set Current: ");
            Serial.println(setCurrent);
            Serial.println("Is the set current correct? (type YES to confirm or NO to restart)");

            while (!Serial.available());
            confirm = Serial.readStringUntil('\n');
            confirm.trim();

            if (confirm.equalsIgnoreCase("YES")) {
              Serial.println("Setting initial charging parameters complete, continuing execution..");
              return true; // Configuration confirmed
            }
          }
        }
        if (confirm.equalsIgnoreCase("NO")) {
          return false; // Restart the process
        }
      }
    } else if (mode.equalsIgnoreCase("D")) {
      setDirection(BIC0_ADDRESS, false);
      writePCF(pcf8574, 0);
      Serial.println("You have selected DISCHARGE, type YES to confirm or NO to restart");

      while (true) {
        // Wait for user input
        while (!Serial.available());
        String confirm = Serial.readStringUntil('\n');
        confirm.trim();

        if (confirm.equalsIgnoreCase("YES")) {
          // Set reverse voltage
          Serial.println("Please write in xx.x format what the SET_REVERSE_VOLTAGE must be:");
          while (!Serial.available());
          float serialSetReverseVoltage = Serial.readStringUntil('\n').toFloat();
          setReverseVout(BIC0_ADDRESS, serialSetReverseVoltage);

          // Confirm set reverse voltage
          float setReverseVoltage = readReverseVOUT_SET(BIC0_ADDRESS);
          Serial.print("Set Reverse Voltage: ");
          Serial.println(setReverseVoltage);
          Serial.println("Is the set reverse voltage correct? (type YES to confirm or NO to restart)");

          while (!Serial.available());
          confirm = Serial.readStringUntil('\n');
          confirm.trim();

          if (confirm.equalsIgnoreCase("YES")) {
            // Set reverse current
            Serial.println("Please write in xx.x format what the SET_REVERSE_CURRENT must be:");
            while (!Serial.available());
            float serialSetReverseCurrent = Serial.readStringUntil('\n').toFloat();
            setReverseIout(BIC0_ADDRESS, serialSetReverseCurrent);

            // Confirm set reverse current
            float setReverseCurrent = readReverseIOUT_SET(BIC0_ADDRESS);
            Serial.print("Set Reverse Current: ");
            Serial.println(setReverseCurrent);
            Serial.println("Is the set reverse current correct? (type YES to confirm or NO to restart)");

            while (!Serial.available());
            confirm = Serial.readStringUntil('\n');
            confirm.trim();

            if (confirm.equalsIgnoreCase("YES")) {
              Serial.println("Setting initial discharging parameters complete, continuing execution..");
              return true; // Configuration confirmed
            }
          }
        }
        if (confirm.equalsIgnoreCase("NO")) {
          return false; // Restart the process
        }
      }
    }
  }
}

void handleUpdateProcess() {
  writePCF(pcf8574, 0); // Close the output of the BIC
  Serial.println("Detected incoming update request, confirm with YES or write EXIT to cancel the update process");

  while (true) {
    while (!Serial.available());
    String confirm = Serial.readStringUntil('\n');
    confirm.trim();

    if (confirm.equalsIgnoreCase("YES")) {
      while (true) {
        bool success = configureChargeDischarge();
        if (success) {
          return; // Exit and continue execution
        } else {
          Serial.println("Do you wish to exit the update process or restart? (Enter EXIT or RESTART)");
          while (!Serial.available());
          confirm = Serial.readStringUntil('\n');
          confirm.trim();

          if (confirm.equalsIgnoreCase("EXIT")) {
            return; // Exit and continue execution
          } else if (confirm.equalsIgnoreCase("RESTART")) {
            break; // Restart the configureChargeDischarge process
          }
        }
      }
    } else if (confirm.equalsIgnoreCase("EXIT")) {
      return; // Exit and continue
    }
  }
}

void handleBMSActivationPulse() {
  Serial.println("Send short BMS activation pulse, type YES to execute or SKIP to continue.");

  while (true) {
    while (!Serial.available());
    String command = Serial.readStringUntil('\n');
    command.trim();

    if (command.equalsIgnoreCase("YES")) {
      // Set direction to discharge
      setDirection(BIC0_ADDRESS, false);
      
      // Get reverse VOUT and IOUT
      Serial.println("Please write in xx.x format what the SET_REVERSE_VOLTAGE must be:");
      while (!Serial.available());
      float serialSetReverseVoltage = Serial.readStringUntil('\n').toFloat();
      setReverseVout(BIC0_ADDRESS, serialSetReverseVoltage);

      // Confirm set reverse voltage
      float setReverseVoltage = readReverseVOUT_SET(BIC0_ADDRESS);
      Serial.print("Set Reverse Voltage: ");
      Serial.println(setReverseVoltage);
      Serial.println("Is the set reverse voltage correct? (type YES to confirm or NO to restart)");

      while (!Serial.available());
      String confirm = Serial.readStringUntil('\n');
      confirm.trim();

      if (confirm.equalsIgnoreCase("YES")) {
        // Get reverse IOUT
        Serial.println("Please write in xx.x format what the SET_REVERSE_CURRENT must be:");
        while (!Serial.available());
        float serialSetReverseCurrent = Serial.readStringUntil('\n').toFloat();
        setReverseIout(BIC0_ADDRESS, serialSetReverseCurrent);

        // Confirm set reverse current
        float setReverseCurrent = readReverseIOUT_SET(BIC0_ADDRESS);
        Serial.print("Set Reverse Current: ");
        Serial.println(setReverseCurrent);
        Serial.println("Is the set reverse current correct? (type YES to confirm or NO to restart)");

        while (!Serial.available());
        confirm = Serial.readStringUntil('\n');
        confirm.trim();

        if (confirm.equalsIgnoreCase("YES")) {
          // Send pulse
          Serial.println("Sending activation pulse...");
          serialEnableOutput();
          delay(500); // Delay for 500ms to simulate the pulse duration

          // Close the output
          writePCF(pcf8574, 0);
          Serial.println("Activation pulse completed, output closed.");
          return;
        }
      }
    } else if (command.equalsIgnoreCase("SKIP")) {
      Serial.println("Skipping BMS activation pulse.");
      return;
    }
  }
}

#endif
