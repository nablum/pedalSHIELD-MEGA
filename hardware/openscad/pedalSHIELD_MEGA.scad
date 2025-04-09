// Library
include <lib.scad>

// Arduino Mega
rotate([90,0,-90]) import("./model3D/ArduinoMega_2560.stl");

// PCB
translate([-0.5,-11.5,12]) import("./model3D/pedalSHIELD_MEGA.stl");

// LCD
translate([-0.5,29.6,24.8]) import("./model3D/displayOLED.stl");

// Pin headers
generate_pin_header(8,[24.3,-37.1,12],"z","y","up"); // Bottom right side
generate_pin_header(8,[24.3,-37.1+22.8,12],"z","y","up"); // Middle right side
generate_pin_header(10,[24.3,-37.1+44.7,12],"z","y","up"); // Top right side
generate_pin_header(8,[-24,-42.2,12],"z","y","up"); // Bottom left side
generate_pin_header(8,[-24,-19.3,12],"z","y","up"); // Middle left side
generate_pin_header(8,[-24,3.5,12],"z","y","up"); // Top left side