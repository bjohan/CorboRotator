include <publicDomainGearV1.1.scad>
//use <ElectronicModuleAssembly/modules/rotorFixture.scad>

use <ElectronicModuleAssembly/modules/shelf.scad>
use <ElectronicModuleAssembly/modules/cnc-control-nano.scad>
use <ElectronicModuleAssembly/modules/jy-mcy-bt-board-v105.scad>
use <ElectronicModuleAssembly/modules/dsn-dvm-368.scad>
use <ElectronicModuleAssembly/modules/gy-89.scad>
use <ElectronicModuleAssembly/modules/mos-module-140c07.scad>
use <ElectronicModuleAssembly/modules/lm2596-dc-dc.scad>
dkNanoPos=[90, 0.5, 5];
dcdcPos=[95, 7, 5+2];
btPos=[65, 15, 5];
dispPos = [15,40,5];
sensPos = [40,25,5];
mosPos = [20, 0, 5];


module electronics1FixtureBody(){
    shelfModuleEven(85, 60, 30, 2, 3/2);
    translate(dkNanoPos)
        rotate([0, 0, 90])
            cncControlNanoFixture(3/2, 5);
    //translate(btPos)
    //    jyMcuBtBoardV105Fixture(3/2, 5);
    /*translate(dispPos)
        dsnDvm368Fixture(3/2, 5);
    translate(sensPos)
        gy89Fixture(3/2, 5);*/
}

module electronics1FixtureModules(){
    translate(dkNanoPos)
        rotate([0, 0, 90])
            cncControlNanoModule(3/2, 5);
    //translate(btPos)
    //    jyMcuBtBoardV105Module(3/2, 5);
    /*translate(dispPos)
        dsnDvm368Module(3/2, 5);
    translate(sensPos)
        gy89Module(3/2, 5);*/
}

module electronics1FixtureHolePattern(h){
    translate(dkNanoPos)
        rotate([0, 0, 90])
            cncControlNanoHolePattern(3/2, h);
    //translate(btPos)
    //    jyMcuBtBoardV105HolePattern(3/2, h);
    /*translate(dispPos)
        dsnDvm368HolePattern(3/2, h);
    translate(sensPos)
        gy89HolePattern(3/2, h);*/
}

module electronics1FixtureModule(){
    difference(){
        electronics1FixtureBody();
        translate([0,0,-20])
            electronics1FixtureHolePattern(60);
    }
}

module electronics15FixtureBody(){
    shelfModuleEven(85, 60, 45, 2, 3/2);
    translate(dcdcPos)
        rotate([0, 0, 90])
            lm2596DcDcFixture(3/2, 5);
}

module electronics15FixtureModules(){
    translate(dcdcPos)
        rotate([0, 0, 90])
            lm2596DcDcModule(3/2, 5);
}

module electronics15FixtureHolePattern(h){
    translate(dcdcPos)
        rotate([0, 0, 90])
            lm2596DcDcHolePattern(3/2, h);
}

module electronics15FixtureModule(){
    difference(){
        electronics15FixtureBody();
        translate([0,0,-20]){
            electronics15FixtureHolePattern(60);
            translate([13,10,0])
                cylinder(60,3,3);
            translate([13,50,0])
                cylinder(60,3,3);
            translate([13+56,50,0])
                cylinder(60,3,3);
            translate([13+56,10,0])
                cylinder(60,3,3);
        }
    }
}



module electronics2FixtureBody(){
    shelfModuleOdd(85, 60, 80, 2, 3/2);
    translate(dispPos)
        dsnDvm368Fixture(3/2, 5);
    //translate(sensPos)
    //    gy89Fixture(3/2, 5);
    translate(btPos)
        jyMcuBtBoardV105Fixture(3/2, 5);
    translate(mosPos)
        mosModule140c07Fixture(3/2, 5);
}

module electronics2FixtureModules(){
    translate(dispPos)
        dsnDvm368Module(3/2, 5);
    //translate(sensPos)
    //    gy89Module(3/2, 5);
    translate(btPos)
        jyMcuBtBoardV105Module(3/2, 5);
    translate(mosPos)
        mosModule140c07Module(3/2, 5);
}

module electronics2FixtureHolePattern(h){
    translate(dispPos)
        dsnDvm368HolePattern(3/2, h);
    //translate(sensPos)
    //    gy89HolePattern(3/2, h);
    translate(btPos)
        jyMcuBtBoardV105HolePattern(3/2, 5);
    translate(mosPos)
        mosModule140c07HolePattern(3/2, h);
}

module electronics2FixtureModule(){
    difference(){
        electronics2FixtureBody();
        translate([0,0,-20])
            electronics2FixtureHolePattern(60);
    }
}

module electronics3FixtureBody(){
    shelfModuleOdd(85, 60, 10, 2, 3/2);
    translate(sensPos)
        gy89Fixture(3/2, 5);
}

module electronics3FixtureModules(){
    translate(sensPos)
        gy89Module(3/2, 5);
}

module electronics3FixtureHolePattern(h){
    translate(sensPos)
        gy89HolePattern(3/2, h);
}

module electronics3FixtureModule(){
    difference(){
        electronics3FixtureBody();
        translate([0,0,-20])
            electronics3FixtureHolePattern(60);
    }
}

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

mastRad = 3.5;
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



module tubeStatorGear(){
    tubeRad = 32.35;
    //t = 5;
    //height = 100;
    difference(){
        union(){
            //tube(, mastRad+t, mastRad);
            tube(30, tubeRad+t*2, tubeRad);
            
            translate([0,0,30-6])
                gear(mmPerTooth,75,12,1);
                //pulley(124/2, 4, 10, 30, 2);
        }
        translate([0,0,9])
        radialHoles(150, 1.5);
        cylinder(30, tubeRad, tubeRad);
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
            translate([-45, -30, 0])
                cylinder(t, cr, cr);
            translate([-45, 50, 0])
                cylinder(t, cr, cr);
        }
        cylinder(t, mastRad+t*2, mastRad+t*2);
    }
}


module nema17Mount(){
    
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
                translate([65, 0, 0]){
                translate([-43/2, -43/2, 0])
                    hull()
                nema17WithGearHolder();
            }
                /*translate([65,0,6]){
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
                */
            }
            
            translate([65, 0, 0]){
                translate([-43/2, -43/2, 0])
                nema17WithGearHolder();
            }
            
            //translate([65,0,0])
            //    nema17WithGearCentered(false);
                //rotate([0,0,90])
                //    xqs5040DServoHoles(8.5,2.75);
            /*translate([33,-30,0])
                rotate([0,0,0])
                    cube([45,5, 30]);
            
            translate([33,55,0])
                rotate([0,0,0])
                    cube([45,5, 30]);
            translate([33,60,0])
                rotate([0,0,180+45])
                    cube([26,5, 30]);*/
        }
        //translate([65,0,0])
        //        rotate([0,0,90])
        //            xqs5040DServoHoles(10,1.6);
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

module nema17MountPlaced(){
    translate([0,0,51]){
        nema17MountWithElectronics();
        //electronics1Placed();
        //electronics2Placed();
    }
}



module servoMountPlaced(){
    translate([0,0,51]){
        servoMountWithElectronics();
        electronics1Placed();
        electronics2Placed();
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

module nema17Gear(){
    h = 16;
    difference(){
        translate([0,0,h/2])
            gear(mmPerTooth,16,h,1);
        translate([0,0,-1])
            dShaft(3.25, h+2, 1);
       translate( [0, 0, h-4])
       rotate([90,0,90]){
            cylinder(100, 3/2, 3/2);
            translate([0,0,7])
                cylinder(100,3,3);
       } 
        //pulley(8, 4, 10, 1, 2);
        //cylinder(5,8/2-0.25, 8/2-0.25);
        //translate([0,0,7])
        //    cylinder(15,3.25, 3.25);
    }
}


module nema17GearPlaced(){
    translate([0,0,51]){
        translate([65,0,10]){
            rotate([0,0,0]){
                rotate([180,0,0])
                    translate([0,0,11]){
                        nema17Gear();
                        //pulley(15, 2, 10, 2, 2);
                    }
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
                        servoPulley();
                        //pulley(15, 2, 10, 2, 2);
                    }
            }
        }
    }
}


module printServoMount(){
    servoMountWithElectronics();
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

module completeAssemblyServo(){
    
    statorPlaced();
    translate([0,0,19]){
        rotorPlaced();
        servoMountPlaced();
        servoPlaced();
        servoPulleyPlaced();
    }
    statorGearPlaced();
}

module completeAssemblyNema17(){
    
    statorPlaced();
    translate([0,0,19]){
        rotorPlaced();
        nema17GearPlaced();
        nema17MountPlaced();
        nema17WithGearPlaced();
        //servoPulleyPlaced();
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

module servoMountWithElectronics(){
    servoMount();
    translate([-42, 63, 5]){
        rotate([90,0,-90]){
            shelfModuleOdd(85, 60, 0, 10, 3/2);
            /*translate([0, 0, 15])
                electronics1FixtureModule();
            translate([0, 0, 50]){
                electronics2FixtureModule();
            }*/
        }
    }
}

module nema17MountWithElectronics(){
    nema17Mount();
    translate([-42, 63, 5]){
        rotate([90,0,-90]){
            shelfModuleOdd(85, 60, 0, 10, 3/2);
        }
        translate([-5,0,0]){
            cylinder(60, 5,5);
            translate([0,-105,0])
                cylinder(60,5,5);
        }
    }
}

module electronics1Placed(){
    //servoMount();
    translate([-42, 63, 75]){
        rotate([90,0,-90]){
            //shelfModuleOdd(85, 60, 0, 10, 3/2);
            translate([0, 0, 10])
                electronics1FixtureModule();
        }
    }
}

module electronics2Placed(){
    //servoMount();
    translate([-42, 63, 75]){
        rotate([90,0,-90]){
            //shelfModuleOdd(85, 60, 0, 10, 3/2);
            translate([0, 0, 40]){
                electronics2FixtureModule();
            }
        }
    }
}

module electronics3Placed(){
    //servoMount();
    translate([-42-82, 63, 75]){
        rotate([90,0,-90]){
            //shelfModuleOdd(85, 60, 0, 10, 3/2);
            translate([0, 0, 40]){
                electronics3FixtureModule();
            }
        }
    }
}


module printElectronics1FixtureModule(){
    electronics1FixtureModule();
}

module printElectronics2FixtureModule(){
    electronics2FixtureModule();
}
module printElectronics3FixtureModule(){
    electronics3FixtureModule();
}

module gearHoles(l, r){
    rotate([0,0,45]){
    translate([-14, 0, 0])
        cylinder(l, r, r);
    translate([14, 0, 0])
        cylinder(l, r, r);
    translate([0, 14, 0])
        cylinder(l, r, r);
    translate([0, -14, 0])
        cylinder(l, r, r);
    }
}

module dShaft(r, l, cut){
       difference(){
                    cylinder(l, r, r);
                    translate([r-cut, -r, -1]){
                        cube([l+2, l+2, l+2]);
                    }
                }
}

module nema17WithGear(screw){
    cube([43, 43, 39]);
    translate([43/2, 43/2, 39]){
        difference(){
            cylinder(31,37/2,37/2);
            translate([0,0,22])
                if(!screw)
                    gearHoles(10, 3/2);
        }
        translate([0,0,31]){
            if(screw)
                gearHoles(10, 3/2);
            
            cylinder(3.2,11.5, 11.5);
            translate([0,0,3]){
                
                dShaft(3, 15, 1);
            }
        }
    }
}

module nema17WithGearPlaced(){
    translate([0,0,51]){
        translate([65,0,0]){
            //rotate([0,0,90]){
                nema17WithGearCentered(false);
        }
    }
    
}


module nema17WithGearCentered(screw){
    translate([-43/2, 43/2, 73])
    rotate([180,0,0]){
        nema17WithGear(screw);
    }
}
module nema17WithGearHolder(){
    w = 45;
    r = 10;
    difference(){
        hull(){
            translate([r-3, r-3, 0])
                cylinder(33, r, r);
            translate([r-3, w-r, 0])
                cylinder(33, r, r);
            translate([w-r, r-3, 0])
                cylinder(33, r, r);
            translate([w-r, w-r, 0])
                cylinder(33, r, r);
        }
        translate([43/2, 43/2, 0])
            nema17WithGearCentered(true);
    }
    //translate(43, )
}


//electronics1FixtureModule();
//electronics15FixtureModule();
//electronics2FixtureModule();
//electronics3FixtureModule();



tubeStatorGear();


//nema17Gear();
//nema17WithGearHolder();
/*completeAssemblyNema17();
electronics1Placed();
electronics2Placed();
electronics3Placed();*/
//nema17GearPlaced();
//nema17MountWithElectronics();


//printElectronics1FixtureModule();
//translate([0,70,0])
//$fn=50;
//printElectronics2FixtureModule();
//electronics2FixtureModules();
//servoMountWithElectronics();
//electronics1Placed();
//translate([0,0,75])
//electronics2Placed();
//checkGeometry();

//printServoMount();
//printRotor();
//printStator();
//printServoPulley();

//servoPulley();
//printRotor();
//servoMount();

//completeAssemblyServo();
//printStatorGear();
//printServoPulley();

//rotorPlaced();
//statorPlaced();
//servoMountPlaced();
//statorGearPlaced();
//servoPlaced();
//servoPulleyPlaced();
//belt(73, 4, 8, 1.5, $fn=100);
//gear(mm_per_tooth,n1,thickness,hole);
//gear(9,11,20,3);
