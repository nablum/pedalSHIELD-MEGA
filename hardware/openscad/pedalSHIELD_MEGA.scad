// Licensed under a Creative Commons Attribution 4.0 International License.

// *** LIBRARY *** //
include <lib.scad>

// *** DIMENSIONS *** //
// System
$fa = 1; // Minimum angle
$fs = 0.1; // Minimum size

// Pedal
pedalWidth = 62;
pedalLength = 122;
pedalHeight = 33;

// Arduino
arduinoWidth = 53;
arduinoLength = 99;
arduinoHeight = 15;

// Case
$caseWidth = pedalWidth+2;
$caseLength = pedalLength+4;
$baseThickness = 5;
$sideHeight = 34;
$sideThickness = 2;
$topThickness = 2;

// Cavity (to avoid conflict with arduino's welds)
$cavityWidth = arduinoWidth - 2;
$cavityLength = arduinoLength - 2;
$cavityHeight = $baseThickness/2;

// Board mounts
$mountInnerDiameter = 4;
$mountOuterDiameter = 6;
$mountDepth = $baseThickness/2;
$mountPosition = [[24.6,45.7], // X/Y
                  [24.6,-29.1],
                  [-23.6,47],
                  [-23.6,-35.5]];

// Audio jack
$audioJackHoleDiameter = 10;
$audioJackHolePosition = [[-49.8,21.7], // Y/Z - Left/Right
                          [-49.8,21.7]]; 

// DC jack
$dcJackHolePosition = [-18.3,7.1]; // X/Z
$dcJackHoleWidth = 9.5;
$dcJackHoleHeight = 11;

// USB
$usbHolePosition = [12.1,6.6]; // X/Z
$usbHoleWidth = 13;
$usbHoleHeight = 11.5;

// Potentiometer
$potHoleDiameter = 4;
$potHolePosition = [54.5,20.6]; // Y/Z

// Foot switch
$footSwitchHoleDiameter = 12.6;
$footSwitchHolePosition = [0,-25.5]; // X/Y

// Push buttons
$pushButtonHoleDiameter = 6.4;
$pushButtonHolePosition = [[-15,16.5], // X/Y - Left/Right
                           [15,16.5]];

// Toggle switch
$toggleSwitchHoleDiameter = 6;
$toggleSwitchHolePosition = [0,16.5]; // X/Y

// LCD window
$lcdWindowEnable = false;
$lcdWindowWidth = 35;
$lcdWindowLength = 24;
$lcdWindowPosition = [0,41];

// *** DESIGN *** //
// Note: order matters for transparency
importPedal(transparency=1);
buildCase(transparency=0.6);