// Licensed under a Creative Commons Attribution 4.0 International License.

module importPedal(transparency=1){
    // Transparency
    color(alpha=transparency){
        // Origin alignment
        translate([0.5,11.5,0]){
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

module buildCase(transparency=1){
    color(alpha=transparency){
        buildBasePanel();
        buildLeftPanel();
        buildRightPanel();
        buildBackPanel(); 
        buildFrontPanel();
        buildTopPanel();
    }
}

module buildTopPanel(){
    difference(){
        // Panel
        translate([0,0,$sideHeight-$topThickness/2]){
            cube([$caseWidth,$caseLength,$topThickness],center=true);
        }
        // Footswitch hole
        translate(concat($footSwitchHolePosition,$sideHeight-$topThickness/2)){ 
            cylinder($topThickness+$fs,d1=$footSwitchHoleDiameter,d2=$footSwitchHoleDiameter,center=true);
        }
        // Push button left
        translate(concat($pushButtonHolePosition[0],$sideHeight-$topThickness/2)){ 
            cylinder($topThickness+$fs,d1=$pushButtonHoleDiameter,d2=$pushButtonHoleDiameter,center=true);
        } 
        // Push button right
        translate(concat($pushButtonHolePosition[1],$sideHeight-$topThickness/2)){ 
            cylinder($topThickness+$fs,d1=$pushButtonHoleDiameter,d2=$pushButtonHoleDiameter,center=true);
        }
        // Toggle switch
        translate(concat($toggleSwitchHolePosition,$sideHeight-$topThickness/2)){ 
            cylinder($topThickness+$fs,d1=$toggleSwitchHoleDiameter,d2=$toggleSwitchHoleDiameter,center=true);
        }
        // LCD window
        if ($lcdWindowEnable){
            translate(concat($lcdWindowPosition,$sideHeight-$topThickness/2)){
                cube([$lcdWindowWidth,$lcdWindowLength,$topThickness+$fs],center=true);
            }
        }
    }
}

module buildBasePanel(){
    difference(){
        union(){
            difference(){ // Cavity to avoid conflict with welds
                translate([0,0,-$baseThickness/2]) cube([$caseWidth,$caseLength,$baseThickness],center=true); // Base
                translate([0.5,11.5,-$cavityHeight/2]) cube([$cavityWidth,$cavityLength,$cavityHeight+$fs],center=true); // Cavity
            }   
            // Mounting bases
            for(i=[0:len($mountPosition)-1]){
                translate(concat($mountPosition[i],-$mountDepth/2)) cylinder($cavityHeight,d1=$mountOuterDiameter,d2=$mountOuterDiameter,center=true);
            }
        }
        // Mounting holes
        for (i=[0:len($mountPosition)-1]){
            translate(concat($mountPosition[i],-$mountDepth/2)) cylinder($mountDepth+$fs,d1=$mountInnerDiameter,d2=$mountInnerDiameter,center=true);
        }
    }
}

module buildLeftPanel(){
    difference(){
        // Panel
        translate([-($caseWidth+$sideThickness)/2,0,($sideHeight-$baseThickness)/2]){
            cube([$sideThickness,$caseLength+2*$sideThickness,$sideHeight+$baseThickness],center=true);
        }
        // Jack connector hole
        translate(concat(-($caseWidth+$sideThickness)/2,$audioJackHolePosition[0][0],$audioJackHolePosition[0][1])){ 
            rotate([0,90,0]){ 
                cylinder($sideThickness+$fs,d1=$audioJackHoleDiameter,d2=$audioJackHoleDiameter,center=true);
            }
        }
    }    
}

module buildRightPanel(){
    difference(){
        // Panel
        translate([($caseWidth+$sideThickness)/2,0,($sideHeight-$baseThickness)/2]){
            cube([$sideThickness,$caseLength+2*$sideThickness,$sideHeight+$baseThickness],center=true);
        }
        // Jack connector hole
        translate(concat(($caseWidth+$sideThickness)/2,$audioJackHolePosition[1][0],$audioJackHolePosition[1][1])){ 
            rotate([0,90,0]){ 
                cylinder($sideThickness+$fs,d1=$audioJackHoleDiameter,d2=$audioJackHoleDiameter,center=true);
            }
        }
        // Potentiometer hole
        translate(concat(($caseWidth+$sideThickness)/2,$potHolePosition[0],$potHolePosition[1])){ 
            rotate([0,90,0]){ 
                cylinder($sideThickness+$fs,d1=$potHoleDiameter,d2=$potHoleDiameter,center=true);
            }
        }
    }
}

module buildBackPanel(){
    // Panel
    translate([0,-($caseLength+$sideThickness)/2,($sideHeight-$baseThickness)/2]){
        cube([$caseWidth+2*$sideThickness,$sideThickness,$sideHeight+$baseThickness],center=true);
    }
}

module buildFrontPanel(){
    difference(){
        // Panel
        translate([0,($caseLength+$sideThickness)/2,($sideHeight-$baseThickness)/2]){
            cube([$caseWidth+2*$sideThickness,$sideThickness,$sideHeight+$baseThickness],center=true);
        }
        // USB Hole
        translate([$usbHolePosition[0],($caseLength+$sideThickness)/2,$usbHolePosition[1]]){
            cube([$usbHoleWidth,$sideThickness+$fs,$usbHoleHeight],center=true);
        }
        // Power supply connector
        translate([$dcJackHolePosition[0],($caseLength+$sideThickness)/2,$dcJackHolePosition[1]]){
            cube([$dcJackHoleWidth,$sideThickness+$fs,$dcJackHoleHeight],center=true);
        }
    }
}

module import_pin_header(rotation=[0,0,0],translation=[0,0,0]){
    rotate(rotation){ 
        translate(translation){
            import("./model3D/pinHeader.stl");
        }
    }
}

module place_pin_header(axis="z",direction="y",orientation="up"){
    // STL file offset
    translation = [-1.6,-5.57,-12.6];
    
    // X-axis
    if (axis=="x"){
        if (direction=="y"){
            if (orientation=="up"){
                import_pin_header([0,0,90],translation);
            }
            if (orientation=="down") {
                import_pin_header([0,0,-90],translation);
            }
            if ((orientation!="up")&&(orientation!="down")){
                assert(false,"wrong orientation (up/down).");
            }
        }
        if (direction=="z"){
            if (orientation=="up"){
                import_pin_header([0,90,90],translation);
            }
            if (orientation=="down") {
                import_pin_header([0,90,-90],translation);
            }
            if ((orientation!="up")&&(orientation!="down")){
                assert(false,"wrong orientation (up/down).");
            }
        }
        if ((direction!="y")&&(direction!="z")){
            assert(false, "wrong direction (x,y,z). Direction can't be identical to axis");
        }
    }
    
    // Y-axis
    if (axis=="y"){
        if (direction=="x"){
            if (orientation=="up"){
                import_pin_header([0,0,180],translation);
            }
            if (orientation=="down") {
                import_pin_header([0,0,0],translation);
            }
            if ((orientation!="up")&&(orientation!="down")){
                assert(false,"wrong orientation (up/down).");
            }
        }
        if (direction=="z"){
            if (orientation=="up"){
                import_pin_header([0,90,180],translation);
            }
            if (orientation=="down") {
                import_pin_header([0,90,0],translation);
            }
            if ((orientation!="up")&&(orientation!="down")){
                assert(false,"wrong orientation (up/down).");
            }
        }
        if ((direction!="x")&&(direction!="z")){
            assert(false, "wrong direction (x,y,z). Direction can't be identical to axis");
        }
    }
    
    // Z-axis
    if (axis=="z"){
        if (direction=="x"){
            if (orientation=="up"){
                import_pin_header([-90,0,0],translation);
            }
            if (orientation=="down") {
                import_pin_header([90,0,0],translation);
            }
            if ((orientation!="up")&&(orientation!="down")){
                assert(false,"wrong orientation (up/down).");
            }
        }
        if (direction=="y"){
            if (orientation=="up"){
                import_pin_header([-90,0,90],translation);
            }
            if (orientation=="down") {
                import_pin_header([90,0,90],translation);
            }
            if ((orientation!="up")&&(orientation!="down")){
                assert(false,"wrong orientation (up/down).");
            }
        }
        if ((direction!="x")&&(direction!="y")){
            assert(false, "wrong direction (x,y,z). Direction can't be identical to axis");
        }
    }
 
    // Wrong axis
    if ((axis!="x")&&(axis!="y")&&(axis!="z")){
        assert(false, "wrong axis (x,y,z).");
    }
}

module generate_pin_header(pinNumber=1,origin=[0,0,0],axis="z",direction="y",orientation="up"){
    // Pin header size in mm
    pinSize = 2.54;
    
    // Generate pin header row
    for(i=[0:pinNumber-1]){
        if (direction=="x"){
            translate(origin + [i*pinSize,0,0]){
                place_pin_header(axis,direction,orientation);
            }     
        }
        if (direction=="y"){
            translate(origin + [0,i*pinSize,0]){
                place_pin_header(axis,direction,orientation);
            }
        }
        if (direction=="z"){
            translate(origin + [0,0,i*pinSize]){
                place_pin_header(axis,direction,orientation);
            }
        }
    }
}