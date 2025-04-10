// Licensed under a Creative Commons Attribution 4.0 International License.

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

module tube(length=10,outerDiameter=4,innerDiameter=2){
    difference(){
        cylinder(length,outerDiameter,outerDiameter);
        cylinder(length+0.1,innerDiameter,innerDiameter);
    }
}