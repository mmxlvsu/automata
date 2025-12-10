#include "DiagramBuilder.h"

void DiagramBuilder::buildExampleDiagram(QGraphicsScene* scene)
{
    // Create states
    auto s0 = DrawHelper::createState(scene, -400, 1000);
    // auto s3 = DrawHelper::createState(scene, 700, 250);

    //Numbers
    auto f1 = DrawHelper::createFinalState(scene, -250, 850);
    auto s1 = DrawHelper::createState(scene, -150, 850);
    auto f2 = DrawHelper::createFinalState(scene, 10, 850);

    //Identifier
    auto f3 = DrawHelper::createFinalState(scene, -250, 970);

    //def
    auto s2 = DrawHelper::createState(scene, -150, 1000);
    auto s3 = DrawHelper::createState(scene, 10, 1000);

    //elif
    auto s4 = DrawHelper::createState(scene, -250, 1100);
    auto s5 = DrawHelper::createState(scene, -150, 1100);

    //for
    auto s6 = DrawHelper::createState(scene, 10, 1150);
    auto s7 = DrawHelper::createState(scene, 100, 1150);

    //while
    auto s8 = DrawHelper::createState(scene, -350, 1200);
    auto s9 = DrawHelper::createState(scene, -250, 1200);
    auto s10 = DrawHelper::createState(scene, -150, 1200);
    auto s11 = DrawHelper::createState(scene, 10, 1200);

    //return
    auto s12 = DrawHelper::createState(scene, -350, 1250);
    auto s13 = DrawHelper::createState(scene, -250, 1250);
    auto s14 = DrawHelper::createState(scene, -150, 1250);
    auto s15 = DrawHelper::createState(scene, 10, 1250);
    auto s16 = DrawHelper::createState(scene, 100, 1250);

    //Main final state/last final state
    auto f4 = DrawHelper::createFinalState(scene, 300, 900);

    // Straight lines
    //Numbers connection----
    DrawHelper::createArrow(scene, s0, f1, "0-9");
    DrawHelper::createArrow(scene, f1, s1, ".");
    DrawHelper::createArrow(scene, s1, f2, "0-9");

    //Identifier connection----
    DrawHelper::createArrow(scene, s0, f3, "a-z, A-Z");

    //def connection----
    DrawHelper::createArrow(scene, s0, s2, "d");
    DrawHelper::createArrow(scene, s2, s3, "e");

    //elif connection----
    DrawHelper::createArrow(scene, s0, s4, "e");
    DrawHelper::createArrow(scene, s4, s5, "l");
    DrawHelper::createArrow(scene, s5, s3, "i");

    //for connection----
    DrawHelper::createArrow(scene, s0, s6, "f");
    DrawHelper::createArrow(scene, s6, s7, "o");

    //while connection----
    DrawHelper::createArrow(scene, s0, s8, "w");
    DrawHelper::createArrow(scene, s8, s9, "h");
    DrawHelper::createArrow(scene, s9, s10, "i");
    DrawHelper::createArrow(scene, s10, s11, "l");

    //return connection----
    DrawHelper::createArrow(scene, s0, s12, "r");
    DrawHelper::createArrow(scene, s12, s13, "e");
    DrawHelper::createArrow(scene, s13, s14, "t");
    DrawHelper::createArrow(scene, s14, s15, "u");
    DrawHelper::createArrow(scene, s15, s16, "r");


    //final connections----
    DrawHelper::createArrow(scene, s3, f4, "f");
    DrawHelper::createArrow(scene, s7, f4, "r");
    DrawHelper::createArrow(scene, s11, f4, "e");
    DrawHelper::createArrow(scene, s16, f4, "n");





    // Curved arrow
    DrawHelper::createCurvedArrow(
    scene,
    s0,
    f4,
    "+, -, *, /, %, **, //=, +=, -=, *=, /===, !=, <=, >=, <, >, '"
    );

    

    // Self-loop
    //self-loop numbers
    DrawHelper::createSelfLoop(scene, f1, "0-9");
    DrawHelper::createSelfLoop(scene, f2, "0-9");

    //self-loop identifier
    DrawHelper::createSelfLoop(scene, f3, "a-z, A-Z, 0-9");

}
