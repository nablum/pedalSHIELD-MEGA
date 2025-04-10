// Licensed under a Creative Commons Attribution 4.0 International License.

// Library
include <lib.scad>

// Minimum dimensions
$fa = 1;
$fs = 0.1;

// *** DESIGN *** //
// Note: order matters for transparency
buildCase(transparency=1);
importPedal(transparency=0.6);

// *** MODULE *** //
module buildCase(transparency=1){
    color(alpha=transparency){
        buildBase(); // Base
        buildLeftSide(); // Left side
    }
}

module buildLeftSide(){
    // Dimension    
        // Panel
        panelWidth = 122;
        panelHeight = 30;
        panelThickness = 2;
        panelPosition = [-32.5,0,panelHeight/2];
    
        // Hole
        holeDiameter = 5;
        holePosition = [panelPosition[0],-50,24-holeDiameter/2];
    
    // Design
    difference(){
        translate(panelPosition) cube([panelThickness,panelWidth,panelHeight],center=true); // Panel
        translate(holePosition){ rotate([0,90,0]){ cylinder(panelThickness+$fs,holeDiameter,holeDiameter,center=true);}} // Hole
    }
}

module buildBase(){
    // Dimensions    
        // Base
        baseWidth = 67;
        baseHeight = 122;
        baseThickness = 10;
        basePosition = [0,0,-baseThickness/2];

        // Cavity
        cavityThickness = 5;
        cavityWidth = 50;
        cavityHeight = 96;
        cavityPosition = [0,11.5,-cavityThickness/2];

        // Mounts
        holeOuterDiameter = 3;
        holeInnerDiameter = 2;
        holeDepth = 5;
        holePositions = [[24.1,45.7,-holeDepth/2],
                         [24.1,-29.1,-holeDepth/2],
                         [-24.1,47,-holeDepth/2],
                         [-24.1,-35.4,-holeDepth/2]];
              
    // Design
    difference(){
        union(){
            difference(){ // Cavity to avoid conflict with welds
                translate(basePosition) cube([baseWidth,baseHeight,baseThickness],center=true); // Base
                translate(cavityPosition) cube([cavityWidth,cavityHeight,cavityThickness+0.1],center=true); // Cavity
            }
            
            // Mounting bases
            for(i=[0:len(holePositions)-1]){
                translate(holePositions[i]) cylinder(cavityThickness,holeOuterDiameter,holeOuterDiameter,center=true);
            }
        }
        
        // Mounting holes
        for (i=[0:len(holePositions)-1]){
            translate(holePositions[i]) cylinder(holeDepth+$fs,holeInnerDiameter,holeInnerDiameter,center=true);
        }
    }
}


module importPedal(transparency=1){
    // Transparency
    color(alpha=transparency){
        // Origin alignment
        translate([0,11.5,0]){
            // Arduino Mega
            rotate([90,0,-90]) import("./model3D/ArduinoMega_2560.stl");

            // PCB
            translate([-0.5,-11.5,12]) import("./model3D/pedalSHIELD_MEGA.stl");

            // LCD
            translate([-0.5,29.6,24.8]) import("./model3D/displayOLED.stl");

            // Pin headers
            generate_pin_header(8,[24.3,-37.1,12],"z","y","up"); // Bottom right side
            generate_pin_header(8,[24.3,-14.3,12],"z","y","up"); // Middle right side
            generate_pin_header(10,[24.3,7.6,12],"z","y","up"); // Top right side
            generate_pin_header(8,[-24,-42.2,12],"z","y","up"); // Bottom left side
            generate_pin_header(8,[-24,-19.3,12],"z","y","up"); // Middle left side
            generate_pin_header(8,[-24,3.5,12],"z","y","up"); // Top left side
        }
    }
}