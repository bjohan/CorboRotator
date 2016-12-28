module xqs5040DServoHoles(h,r){
        translate([-15,0,0]){
            translate([-1.5-3, 11/2+3])
                cylinder(h, r, r);
            translate([-1.5-3, -11/2-3])
                cylinder(h, r, r);
            translate([64.5, 11/2+3])
                cylinder(h, r, r);
            translate([64.5, -11/2-3])
                cylinder(h, r, r);
        }
}

module xqs5040DServoBody(){
    translate([-15,-15,-50]){
        translate([-7.5,0,50])
            cube([75 ,30, 4]);
        cube([60,30,60]);
        
    }
    translate([0,0,10])
            cylinder(7,3.5,3.5);
}

module xqs5040DServo(){
    difference(){
        xqs5040DServoBody();
        translate([0,0,-1])
        xqs5040DServoHoles(6,3);
    }
}

module tube(l, ro, ri){
    difference(){
        cylinder(l, ro, ro);
        cylinder(l, ri, ri); 
    }
}

module tooth(h, r){
    //cylinder(h,r,r);
    translate([-r, -r, 0])
        cube([2*r, 2*r, h]);
}

module teeth(n, delta, h, duty){
    circ = n*delta;
    div = circ/n;
    rad = 0.5*circ/PI;
    for( i = [0:n-1]){
        rotate([0,0,i*360/n])
        translate([rad, 0, 0]){
            tooth(h, duty*div*0.5);
        }
    }
}

function computeTeethRadius(n, delta) = n*delta*0.5/PI;

module pulley(n, delta, h, ri, fh){
    r = computeTeethRadius(n, delta);
    tube(2, r+fh, ri);
    translate([0, 0, 2]){
        tube(h, r, ri);
        teeth(n, delta, h, 0.5);
    }
    
    translate([0, 0, 2+h]){
        tube(2, r+fh, ri);
    }
}

module belt(n, delta, h, t){
    r = computeTeethRadius(n, delta);
    tube(h, r+t, r);
    teeth(n, delta, h, 0.5);
}

module relievedSlit(l, h, w, r){
    translate([-l/2, -w/2, 0])
        cube([l,w,h]);
    translate([0,0,h])
        rotate([0,90,0])
            translate([0,0,-l/2])
                cylinder(l, r, r);
}

module radialHoles(l, r){
    rotate([0,90,0])
        translate([0,0,-l/2])
            cylinder(l,r,r);
    rotate([0,0,90])
        rotate([0,90,0])
            translate([0,0,-l/2])
                cylinder(l,r,r);
    rotate([0,0,45]){
        rotate([0,90,0])
            translate([0,0,-l/2])
                cylinder(l,r,r);
        rotate([0,0,90])
            rotate([0,90,0])
                translate([0,0,-l/2])
                    cylinder(l,r,r);
    }
}

//pulley(130, 2, 10, 30, 2);
//teeth(100, 2, 10, 0.5);

mastRad = 30;
t = 5;
height = 100;

module stator(){
    //mastRad = 30;
    //t = 5;
    //height = 100;
    difference(){
        union(){
            tube(height, mastRad+t, mastRad);
            tube(height/2, mastRad+t*2, mastRad);
            translate([0,0,height])
                cylinder(t, mastRad+t, mastRad+t);
            translate([0,0,height/2-14])
                pulley(125, 2, 10, 30, 2);
        }
        relievedSlit(150, 35, 2, 2);
        rotate([0,0,90])
            relievedSlit(150, 35, 2, 2);
        rotate([0,0,45]){
            relievedSlit(150, 35, 2, 2);
            rotate([0,0,90])
                relievedSlit(150, 35, 2, 2);
        }
    
    }
}

module servoPlate(t){
    difference(){
        hull(){
            cylinder(t, mastRad+t*3, mastRad+t*3);
            translate([70,-20,0])
                cylinder(t, 15, 15);
            translate([70,50,0])
                cylinder(t, 15, 15);
            translate([-50, -45, 0])
                cylinder(t, 15, 15);
            translate([-50, 45, 0])
                cylinder(t, 15, 15);
        }
        cylinder(t, mastRad+t*2, mastRad+t*2);
    }
}

module servoMount(){
    
    difference(){
        union(){
            tube(30, mastRad+t*3, mastRad+t*2);
            difference(){
                servoPlate(5);
                translate([65,0,-1]){
                    rotate([0,0,90]){
                        xqs5040DServoHoles(10,3);
                        //xqs5040DServoBody();
                    }
                }
            }
        }
        translate([65,0,6]){
            rotate([0,0,90]){
                //xqs5040DServoHoles(10,3);
                minkowski(){
                    xqs5040DServoBody();
                    sphere(1);
                }
            }
        }
        translate([0,0,5+25/2])
            radialHoles(200,4/2);
    }
}

module rotor(){
    difference(){
        union(){
            tube(height/2, mastRad+t*2, mastRad+0.25);
            translate([0,0,height/2])
                cylinder(t, mastRad+t*2, mastRad+t*2);
        }
        translate([0,0,5+25/2])
            radialHoles(200,3/2);
    }
    //pulley(125, 2, 10, 30, 2);
}

module rotorPlaced(){
    translate([0,0,51]){
        rotor();
    }
}

module servoMountPlaced(){
    translate([0,0,51]){
        servoMount();
    }
}

module statorPlaced(){
    stator();
}

module servoPlaced(){
    translate([0,0,51]){
        translate([65,0,10]){
            rotate([0,0,90]){
                rotate([180,0,0])
                    xqs5040DServo();                
            }
        }
    }
}

module servoPulleyPlaced(){
    translate([0,0,51]){
        translate([65,0,10]){
            rotate([0,0,90]){
                rotate([180,0,0])
                    translate([0,0,11]){
                        pulley(15, 2, 10, 2, 2);
                    }
            }
        }
    }
}


servoMount();
//rotorPlaced();
//statorPlaced();

//servoMountPlaced();
//servoPlaced();
//servoPulleyPlaced();
//belt(150, 2, 10, 1);