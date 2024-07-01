import processing.serial.*;

Serial myPort; // Create object from Serial class
String val;    // Data received from the serial port

// BIC Data
float Vdc, Vac, Temp, Idc;
boolean fanFail, OTP, OVP, OVC, SCP, ACFail, DCStatus, highTemperature;

// BMS Data
boolean chargeWireConnected, batteryCharging, batteryLowSOC, batteryReady, dischargeMOSStatus, chargeMOSStatus;
float SOC, batteryCurrent, batteryVoltage;
int batteryFailureLevel;

void setup() {
  size(800, 800); // Window size
  myPort = new Serial(this, "COM6", 115200); // Adjust COM port and baud rate
  myPort.bufferUntil('\n'); // Trigger serialEvent on new line
  println("Waiting for data...");
}

void draw() {
  background(0); // Set background color

  // Set text size and color
  textSize(24);
  fill(255);

  // Display BIC data
  textAlign(LEFT);
  
  text("Vdc (V):", 50, 50);
  text(nfc(Vdc, 3), 200, 50);
  
  text("Vac (V):", 50, 100);
  text(nfc(Vac, 3), 200, 100);
  
  text("Temp (°C):", 50, 150);
  text(nfc(Temp, 3), 200, 150);
  
  text("Idc (A):", 50, 200);
  text(nfc(Idc, 3), 200, 200);

  textSize(18);
  text("Fault Status Bits:", 50, 250);
  
  text("Bit 0 - FAN_FAIL:", 50, 280);
  text(fanFail ? "Fan locked" : "Fan working normally", 300, 280);
  
  text("Bit 1 - OTP:", 50, 310);
  text(OTP ? "Internal temperature abnormal" : "Internal temperature normal", 300, 310);
  
  text("Bit 2 - OVP:", 50, 340);
  text(OVP ? "DC over voltage protected" : "DC voltage normal", 300, 340);
  
  text("Bit 3 - OVC:", 50, 370);
  text(OVC ? "DC over current protected" : "DC current normal", 300, 370);
  
  text("Bit 4 - SCP:", 50, 400);
  text(SCP ? "Shorted circuit protected" : "Shorted circuit does not exist", 300, 400);
  
  text("Bit 5 - AC_FAIL:", 50, 430);
  text(ACFail ? "AC range abnormal" : "AC range normal", 300, 430);
  
  text("Bit 6 - DC Status:", 50, 460);
  text(DCStatus ? "DC turned off" : "DC turned on", 300, 460);
  
  text("Bit 7 - HI_TEMP:", 50, 490);
  text(highTemperature ? "Internal temperature abnormal" : "Internal temperature normal", 300, 490);

  // Display BMS data
  textSize(24);
  text("SOC (%):", 50, 540);
  text(nfc(SOC, 3), 200, 540);
  
  text("Battery Current (A):", 50, 590);
  text(nfc(batteryCurrent, 3), 300, 590);
  
  text("Battery Voltage (V):", 50, 640);
  text(nfc(batteryVoltage, 3), 300, 640);
  
  text("Battery Failure Level:", 50, 690);
  text(batteryFailureLevel == 0 ? "No Failure" : batteryFailureLevel == 1 ? "Level 1 (Stop Now)" : batteryFailureLevel == 2 ? "Level 2 (Power reduce to 50%)" : batteryFailureLevel == 3 ? "Level 3 (Alarm)" : "Unknown", 300, 690);
}

void serialEvent(Serial myPort) {
  val = myPort.readStringUntil('\n'); // Read data until newline
  println("Received data: " + val); // Debug print

  if (val != null) {
    // Parse the data
    String[] data = val.trim().split(",");

    for (String item : data) {
      String[] parts = item.split(":");
      if (parts.length == 2) {
        switch (parts[0]) {
          case "Vdc":
            Vdc = float(parts[1]);
            break;
          case "Vac":
            Vac = float(parts[1]);
            break;
          case "Temp":
            Temp = float(parts[1]);
            break;
          case "Idc":
            Idc = float(parts[1]);
            break;
          case "fanFail":
            fanFail = parts[1].equals("1");
            break;
          case "OTP":
            OTP = parts[1].equals("1");
            break;
          case "OVP":
            OVP = parts[1].equals("1");
            break;
          case "OVC":
            OVC = parts[1].equals("1");
            break;
          case "SCP":
            SCP = parts[1].equals("1");
            break;
          case "ACFail":
            ACFail = parts[1].equals("1");
            break;
          case "DCStatus":
            DCStatus = parts[1].equals("1");
            break;
          case "highTemperature":
            highTemperature = parts[1].equals("1");
            break;
          case "SOC":
            SOC = float(parts[1]);
            break;
          case "batteryCurrent":
            batteryCurrent = float(parts[1]);
            break;
          case "batteryVoltage":
            batteryVoltage = float(parts[1]);
            break;
          case "batteryFailureLevel":
            batteryFailureLevel = int(parts[1]);
            break;
        }
      }
    }
  }
}
