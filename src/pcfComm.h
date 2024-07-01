// This file contains all the functions to control the PCF
// Created by: Erman Ergül
// Created on: 7-6-2024
// Last modified: 7-6-2024

#ifndef pcfComm_h
#define pcfComm_h

#include "globals.h"            // Library to include all the global variables

void readPCF(PCF8574 &pcf8574) {
  tempB0hw = pcf8574.read(0);  // Read pin 0
  Serial.print("BIC0 Temperature Error: ");
  Serial.println(tempB0hw);
  faultB0hw = pcf8574.read(1); // Read pin 1
  Serial.print("BIC0 Fault Error: ");
  Serial.println(faultB0hw);
  DCB0hw = pcf8574.read(2);    // Read pin 2
  DCB0hw = !DCB0hw;
  Serial.print("BIC0 DC Error: ");
  Serial.println(DCB0hw);
  tempB1hw = pcf8574.read(3);  // Read pin 3
  Serial.print("BIC1 Temperature Error: ");
  Serial.println(tempB1hw);
  faultB1hw = pcf8574.read(4); // Read pin 4
  Serial.print("BIC1 Fault Error: ");
  Serial.println(faultB1hw);
  DCB1hw = pcf8574.read(5);    // Read pin 5
  Serial.print("BIC1 DC Error: ");
  Serial.println(DCB1hw);
}

void writePCF(PCF8574 &pcf8574, int rctrl) {
  // if (cd == 1) {
  //   pcf8574.write(6, HIGH);
  // } else {
  //   pcf8574.write(6, LOW);
  if (rctrl == 1) {
    pcf8574.write(7, HIGH);
  } else {
    pcf8574.write(7, LOW);
  }
}

#endif
