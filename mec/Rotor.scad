include <publicDomainGearV1.1.scad>
module xqs5040DServoHoles(h,r){
        translate([-15,0,0]){
            translate([-1.5-3, 11/2+3])
                cylinder(h, r, r);
            translate([-1.5-3, -11/2-3])
                cylinder(h, r, r);
            translate([64.5-0.5, 11/2+3])
                cylinder(h, r, r);
            translate([64.5-0.5, -11/2-3])
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
height = 130;
mmPerTooth = 4.5;
module stator(){
    //mastRad = 30;
    //t = 5;
    //height = 100;
    difference(){
        union(){
            tube(height, mastRad+t, mastRad);
            tube(height/2, mastRad+t, mastRad);
            translate([0,0,height])
                cylinder(t, mastRad+t, mastRad+t);
            //translate([0,0,height/2-14+8])
            //    gear(mmPerTooth,75,12,10);
                //pulley(124/2, 4, 10, 30, 2);
        }
        relievedSlit(150, 35, 2, 2);
        rotate([0,0,90])
            relievedSlit(150, 35, 2, 2);
        rotate([0,0,45]){
            relievedSlit(150, 35, 2, 2);
            rotate([0,0,90])
                relievedSlit(150, 35, 2, 2);
        }
        translate([0,0,48])
            radialHoles(150, 1.5);
    
    }
}

module statorGear(){
    //mastRad = 30;
    //t = 5;
    //height = 100;
    difference(){
        union(){
            //tube(, mastRad+t, mastRad);
            tube(30, mastRad+t*2, mastRad+t);
            
            translate([0,0,30-6])
                gear(mmPerTooth,75,12,1);
                //pulley(124/2, 4, 10, 30, 2);
        }
        translate([0,0,9])
        radialHoles(150, 2);
        cylinder(30, mastRad+t+0.25, mastRad+t+0.25);
        /*relievedSlit(150, 35, 2, 2);
        rotate([0,0,90])
            relievedSlit(150, 35, 2, 2);
        rotate([0,0,45]){
            relievedSlit(150, 35, 2, 2);
            rotate([0,0,90])
                relievedSlit(150, 35, 2, 2);
        }*/
    
    }
}

module servoPlate(t){
    cr = 20;
    difference(){
        hull(){
            cylinder(t, mastRad+t*3, mastRad+t*3);
            translate([70,-30,0])
                cylinder(t, cr, cr);
            translate([70,50,0])
                cylinder(t, cr, cr);
            translate([-80, -30, 0])
                cylinder(t, cr, cr);
            translate([-80, 50, 0])
                cylinder(t, cr, cr);
        }
        cylinder(t, mastRad+t*2, mastRad+t*2);
    }
}

module servoMount(){
    
    difference(){
        union(){
            difference(){
                union(){
                    tube(30, mastRad+t*3, mastRad+t*2);
                    difference(){
                        servoPlate(5);
                        /*translate([65,0,-1]){
                            rotate([0,0,90]){
                                xqs5040DServoHoles(10,3);
                                //xqs5040DServoBody();
                            }
                        }*/
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
            translate([65,0,0])
                rotate([0,0,90])
                    xqs5040DServoHoles(8.5,2.75);
            translate([33,-30,0])
                rotate([0,0,0])
                    cube([45,5, 30]);
            
            translate([33,55,0])
                rotate([0,0,0])
                    cube([45,5, 30]);
            translate([33,60,0])
                rotate([0,0,180+45])
                    cube([26,5, 30]);
        }
        translate([65,0,0])
                rotate([0,0,90])
                    xqs5040DServoHoles(10,1.6);
    }
}

module rotor(){
    t=5;
    difference(){
        union(){
            tube(height/2, mastRad+t*2-0.4, mastRad+5+0.5);
            translate([0,0,height/2])
                cylinder(t, mastRad+t*2-0.4, mastRad+t*2-0.4);
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

module servoPulley(){
    difference(){
        translate([0,0,6])
            gear(mmPerTooth,16,12,1);
        //pulley(8, 4, 10, 1, 2);
        cylinder(5,8/2-0.25, 8/2-0.25);
        translate([0,0,7])
            cylinder(15,3.25, 3.25);
    }
}

module servoPulleyPlaced(){
    translate([0,0,51]){
        translate([65,0,10]){
            rotate([0,0,90]){
                rotate([180,0,0])
                    translate([0,0,11]){
                        servoPulley();
                        //pulley(15, 2, 10, 2, 2);
                    }
            }
        }
    }
}


module printServoMount(){
    servoMount();
}

module printRotor(){
    rotate([180,0,0])
        rotor();
}

module printStator(){
    rotate([180,0,0])
        stator();
}


module printStatorGear(){
    rotate([180,0,0])
        statorGear();
}

module printServoPulley(){
    servoPulley();
}

module statorGearPlaced(){
    translate([0,0,39])
        statorGear();
}

module completeAssembly(){
    
    statorPlaced();
    translate([0,0,19]){
        rotorPlaced();
        servoMountPlaced();
        servoPlaced();
        servoPulleyPlaced();
    }
    statorGearPlaced();
}

module checkGeometry(){
    intersection(){
        rotorPlaced();
        statorPlaced();
        servoMountPlaced();
        servoPlaced();
        servoPulleyPlaced();
    }
}

//checkGeometry();

printServoMount();
//printRotor();
//printStator();
//printServoPulley();

//servoPulley();
//printRotor();
//servoMount();

//completeAssembly();
//printStatorGear();
//printServoPulley();

//rotorPlaced();
//statorPlaced();
//servoMountPlaced();
//servoPlaced();
//servoPulleyPlaced();
//belt(73, 4, 8, 1.5, $fn=100);
//gear(mm_per_tooth,n1,thickness,hole);
//gear(9,11,20,3);
