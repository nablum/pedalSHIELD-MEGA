// Licensed under a Creative Commons Attribution 4.0 International License.

// *** LIBRARY *** //
include <lib.scad>

// *** VISUALISATION *** //
view = "assembly"; // choose between "assembly", "printing", "cutting"
printing = true; // True for 3D print

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
arduinoWeldsHeight = 2.5;

// Cavity (to avoid conflict with arduino's welds)
$cavityWidth = arduinoWidth-2;
$cavityLength = arduinoLength-2;
$cavityHeight = arduinoWeldsHeight+0.5;

// Mounts
$nutType = M3_nut;
$screwType = M3_dome_screw;
$screwPosition = [[24.68,45.65], // X/Y
                  [24.68,-29.25],
                  [-23.55,46.9],
                  [-23.55,-35.6]];

// Case
$caseWidth = pedalWidth+0.5;
$caseLength = pedalLength+0.5;
$baseThickness = $cavityHeight+nut_thickness($nutType)+2;
$sideHeight = 34;
$sideThickness = 2;
$topThickness = 2;
$topSideOverlap = 0.5;

// Audio jack
$audioJackHoleDiameter = 12;
$audioJackHolePosition = [[-49.8,23.2], // Y/Z - Left/Right
                          [-49.8,23.2]]; 

// DC jack
$dcJackHolePosition = [-18.3,7.1]; // X/Z
$dcJackHoleWidth = 9.5;
$dcJackHoleHeight = 12;

// USB
$usbHolePosition = [12.1,7]; // X/Z
$usbHoleWidth = 13;
$usbHoleHeight = 12.5;

// Potentiometer
$potHoleDiameter = 4;
$potHolePosition = [54.5,22.6]; // Y/Z

// Foot switch
$footSwitchHoleDiameter = 12.6;
$footSwitchHolePosition = [0,-25.5]; // X/Y

// Push buttons
$pushButtonHoleDiameter = 7;
$pushButtonHolePosition = [[-15,16.5], // X/Y - Left/Right
                           [15,16.5]];

// Toggle switch
$toggleSwitchHoleDiameter = 7;
$toggleSwitchHolePosition = [0,16.5]; // X/Y

// LCD window
$lcdWindowEnable = false;
$lcdWindowWidth = 31;
$lcdWindowLength = 16;
$lcdWindowPosition = [0,44];

// *** DESIGN *** //
if(view=="assembly") buildCase();
if(view=="printing") printCase();
if(view=="cutting") cutCase();

// *** MODULES *** //
module buildCase(){
    importPedal();
    addMounts();
    buildBottomPanel();
    buildLeftPanel();
    buildRightPanel();
    buildBackPanel(); 
    buildFrontPanel();
    color(alpha=0.5) buildTopPanel();
}

module printCase(){
    translate([0,0,$baseThickness]){
        buildBottomPanel();
        buildLeftPanel();
        buildRightPanel();
        buildBackPanel(); 
        buildFrontPanel();
    }
    translate([$caseWidth+5,0,-$sideHeight+$topThickness]){
        buildTopPanel();
    }
}

module cutCase(){
    projection(cut = false){
        buildTopPanel();
    }    
}
