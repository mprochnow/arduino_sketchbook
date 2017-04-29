nozzle_width = 0.4;
wall_thickness = 4 * nozzle_width;
t = wall_thickness;

pcb_width = 117.2;
pcb_depth = 73.1;
pcb_height = 1.7;
pcb_clearance_led = 4;
pcb_led_height = 2;
pcb_clearance_below = 5;
pcb_clearance_top = 23;
pcb_holder_radius = 3.6;
pcb_holder_y_pos = 10;
overlap = 5;
gap = 3;
front_lid_holder_width = 10;

case_wall_thickness = 4 * t;
case_width = case_wall_thickness + pcb_width+case_wall_thickness;
case_depth = case_wall_thickness + pcb_clearance_led + pcb_depth + case_wall_thickness;
lower_half_height = t + pcb_clearance_below + pcb_height + pcb_led_height;
upper_half_height = t + pcb_clearance_top - gap;

//        x-pos, z-pos, width, height
cutouts = [[ 4.5, -0.4, 12.0,  5.8], // audio in
           [29.0,  4.0,  9.0, 10.9],     // power in
           [71.0, -1.3, 11.0, 14.0], // audio out left
           [83.5, -1.3, 11.0, 14.0], // audio out right
           [99.0,  1.0, 13.0,  6.5]]; // usb power out

module lower_half() {
    difference() {
        union() {
            cube([case_width, case_depth, lower_half_height]);

            for (x=[case_wall_thickness-t, case_wall_thickness+pcb_width]) {
                translate([x, case_wall_thickness-t, lower_half_height])
                    cube([t, t+pcb_clearance_led+pcb_depth+t, gap+overlap]);
            }
            
            translate([case_wall_thickness, case_wall_thickness+pcb_clearance_led+pcb_depth, lower_half_height])
                cube([pcb_width, t, gap+overlap]);
        }
    
        translate([case_wall_thickness, case_wall_thickness-t, t])
        difference() {
            cube([pcb_width, t+pcb_depth+pcb_clearance_led, lower_half_height+0.1]);
            
            for (x=[0, pcb_width]) {
                for (y=[pcb_holder_y_pos, pcb_depth-pcb_holder_y_pos]) {
                    translate([x, pcb_clearance_led+y, -0.1])
                        cylinder(r=pcb_holder_radius, h=pcb_clearance_below+0.1, $fn=16);
                }
            }
            
            for (x=[-1, 1]) {
                translate([pcb_width/2+x*pcb_width/4-front_lid_holder_width/2, t, 0])
                    cube([front_lid_holder_width, pcb_clearance_led, pcb_clearance_below+pcb_height]);
            }
        }

        for (cutout = cutouts) {
            xpos = cutout[0];
            zpos = cutout[1];
            width = cutout[2];
            
            translate([case_wall_thickness+xpos, case_wall_thickness+pcb_clearance_led+pcb_depth-0.1, 2*t+pcb_clearance_below+pcb_height+zpos])
                cube([width, case_wall_thickness+0.2, 30]);
        }
        
        translate([case_wall_thickness, case_depth-case_wall_thickness+t, t])
            cube([pcb_width, case_depth+0.1, 30]);
    }
}

module upper_half() {
    difference() {
        cube([case_width, case_depth, upper_half_height]);
        
        translate([case_wall_thickness, case_wall_thickness, -0.1])
            cube([case_width-2*case_wall_thickness, case_depth-2*case_wall_thickness, upper_half_height-t+0.1]);

        translate([case_wall_thickness-t, case_wall_thickness-t, -0.1])
            #cube([case_width-2*case_wall_thickness+2*t, case_depth-2*case_wall_thickness+2*t, overlap+0.1]);

        translate([case_wall_thickness, case_depth-case_wall_thickness+t, -0.1])
            cube([pcb_width, case_depth+0.1, upper_half_height-t+0.1]);

        translate([case_wall_thickness, case_depth-case_wall_thickness+t-0.1, -0.1])
            cube([pcb_width, case_depth+0.1, overlap]);

        for (cutout = cutouts) {
            xpos = cutout[0];
            zpos = cutout[1];
            width = cutout[2];
            height = cutout[3];
            
            translate([case_wall_thickness+xpos, case_wall_thickness+pcb_clearance_led+pcb_depth-0.1, -(pcb_clearance_below+pcb_height)+gap+zpos])
                #cube([width, case_wall_thickness+0.2, height]);
        }
    }
}

lower_half();

translate([0, 0, lower_half_height+gap+10])
//rotate([0, 180, 0])
upper_half();