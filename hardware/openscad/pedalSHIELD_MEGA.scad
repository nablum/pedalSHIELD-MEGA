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
arduinoWeldsHeight = 1.5;

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
$caseWidth = pedalWidth+2;
$caseLength = pedalLength+4;
$baseThickness = $cavityHeight+nut_thickness($nutType)+2;
$sideHeight = 34;
$sideThickness = 2;
$topThickness = 2;
$topSideOverlap = 0.5;

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
