// Licensed under a Creative Commons Attribution 4.0 International License.

// Library
include <lib.scad>

// Minimum dimensions
$fa = 1;
$fs = 0.1;

// *** DESIGN *** //
buildCase(transparency=1);
importPedal(transparency=0.6);

// *** MODULE *** //
module buildCase(transparency=1){
    color(alpha=transparency){
        // Base
        buildBase();
    }
}

module buildBase(){
    // Dimensions    
        // Base
        baseWidth = 62;
        baseHeight = 122;
        baseThickness = 10;
        yOffset = -11.5;
        basePosition = [0,yOffset,-baseThickness/2];

        // Cavity
        cavityThickness = 5;
        cavityWidth = 50;
        cavityHeight = 96;
        cavityPosition = [0,0,-cavityThickness/2];

        // Mounts
        holeOuterDiameter = 3;
        holeInnerDiameter = 2;
        holeDepth = 5;
        holePositions = [[24.1,34.2,-holeDepth/2],
                         [24.1,-40.6,-holeDepth/2],
                         [-24.1,35.5,-holeDepth/2],
                         [-24.1,-46.9,-holeDepth/2]];
              
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