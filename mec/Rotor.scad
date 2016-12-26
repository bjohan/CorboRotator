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


//xqs5040DServo();

module mastPart(){
    mastRad = 30;
    t = 10;
    height = 100;
    cylinder(height, mastRad, mastRad); 
}
mastPart();