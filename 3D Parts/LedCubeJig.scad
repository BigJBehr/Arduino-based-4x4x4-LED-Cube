// 4x4x4 LED Cube LED Soldering Aides

Led2Led = 17;

X = 3 * Led2Led;
Y = X;
Z = 2;

LedD = 5.2;
LedRing = 6;
BossD = LedD + 4;   // leaves 2mm
BossZ = 12;
Leads = 1.27;
LeadW = .6;
Wire = 1;           // sized for 22awg bare wire
WireZ = BossZ - 1.5;
SupportW = 5;

$fn = 50;
BendingJig();
LayerSpacers(70);

module LayerSpacers(yoffset)
{
    for(y = [0: 12: 60])
    {
        translate([0, y + yoffset, 0]) LayerSpacer();
    
        // easy removal tab
        translate([0, y + yoffset, 0]) cylinder(d = 25, h = .4);
    }
}   //  LayerSpacers

module LayerSpacer()
{
    x = 85;
    y = 10;
    difference()
    {
        cube([x, y, 5]);
        for(i = [20: 20: x])
            translate([i, 0, 3]) cube([Wire, y, 2]);
    }
}   //  LayerSpacer

module BendingJig()
{
    translate([-11, -10, 0]) cube([84, 71, Z]);
    Bosses();
    Supports();
    translate([73, 9, Z])
        linear_extrude(height = 2, center = true, convexity = 10, twist = 0)
            rotate([0, 0, 90]) text("Front");

    // easy removal tab
    translate([-10, -10, 0]) cylinder(d = 25, h = .4);
}   //  BendingJig

module Bosses()
{
    for(y = [0: Led2Led: Y])
        for(x = [0: Led2Led: X])
            translate([x, y, Z]) Boss();
}   //  Bosses

module Boss()
{
    difference()
    {
        cylinder(d = BossD, h = BossZ);
        cylinder(d = LedD, h = BossZ);
        translate([0, 0, BossZ - 3]) cylinder(d = LedRing, h = BossZ);

        translate([-BossD / 2, -LeadW / 2, WireZ]) cube([BossD, LeadW, 3]);
        translate([-Leads, -BossD / 2, WireZ]) cube([2 * Leads, BossD, 3]);
    }
}   //  Boss

module Supports()
{
    i = (Led2Led - SupportW) / 2;

    // inner supports
    for(y = [i: Led2Led: Y])
        for(x = [i: Led2Led: X])
            translate([x, y, Z]) Support(0);

    // left side power supports
    y = -(2 * SupportW);
    for(x = [i: Led2Led: X])
        translate([x, y, Z]) Support(0);
    
    // right side power supports
    y1 = Y + SupportW;
    for(x = [i: Led2Led: X])
        translate([x, y1, Z]) Support(0);

    // back side power supports
    translate([-(SupportW + 1), i, Z]) rotate([0, 0, 90]) Support(0);
    translate([-(SupportW + 1), i + 2 * Led2Led, Z]) rotate([0, 0, 90]) Support(0);
/*
    for(y = [i: Led2Led: Y])
    {
        translate([-(SupportW + 1), y, Z]) rotate([0, 0, 90]) Support(0);
    }
*/    
    // front side ground support
    translate([X + SupportW + 1, Led2Led + (2 * SupportW) + 1, Z]) rotate([0, 0, 270]) Support(0);

    
    // bending posts
    translate([-5.5, -4.5, Z]) cylinder(d = SupportW, h = BossZ);
    translate([-5.5, X + 4.5, Z]) cylinder(d = SupportW, h = BossZ);

    translate([X + 5.5, Led2Led - 5.5, Z]) cylinder(d = SupportW, h = BossZ);
    translate([X + 5.5, (2 * Led2Led) + 5.5, Z]) cylinder(d = SupportW, h = BossZ);

    
    translate([X + SupportW, -10, Z]) Support(0);
    translate([X + SupportW, X + SupportW, Z]) Support(0);
    
    // paths
//    z1 = 3;
//    translate([-6.5, -(6 + ((SupportW - Wire) / 2)), 10]) cube([70, Wire, z1]);
}   //  Supports

module Support(type)
{
    notchz = 3;
    z = WireZ - 0.5;
    difference()
    {
        cube([SupportW, SupportW, BossZ]);
        translate([0, (SupportW - Wire) / 2, z]) cube([SupportW, Wire, notchz]);
        
        if (1 == type)
        {
            translate([(SupportW - Wire) / 2, 0, z]) cube([Wire, SupportW / 2, notchz]);
        }
        
        if (2 == type)
        {
            translate([(SupportW - Wire) / 2, 0, z]) cube([Wire, SupportW, notchz]);
        }
    }
}   //  Support


