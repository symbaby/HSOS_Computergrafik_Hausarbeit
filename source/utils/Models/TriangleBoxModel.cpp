//
//  TriangleBoxModel.cpp
//  CGXcode
//
//  Created by Philipp Lensing on 10.10.16.
//  Copyright © 2016 Philipp Lensing. All rights reserved.
//

#include "utils/Models/TriangleBoxModel.h"

TriangleBoxModel::TriangleBoxModel(float Width, float Height, float Depth) {

    // TODO: Add your code (Exercise 2)
    /// TODO Hardcode shit aufraeumen + normale fixen
    VB.begin();

    //Die Werte im Koordinatensystem
    float CoordWidth = Width / 2;
    float CoordHeight = Height / 2;
    float CoordDepth = Depth / 2;

    //Die Punkte
    Vector vorne_unten_links(-CoordWidth, -CoordHeight, -CoordDepth); // 0
    Vector vorne_oben_links(-CoordWidth, CoordHeight, -CoordDepth); // 1
    Vector vorne_unten_rechts(-CoordWidth, -CoordHeight, CoordDepth); // 2
    Vector vorne_oben_rechts(-CoordWidth, CoordHeight, CoordDepth); // 3
    Vector hinten_unten_links(CoordWidth, -CoordHeight, -CoordDepth); // 4
    Vector hinten_unten_rechts(CoordWidth, -CoordHeight, CoordDepth); // 5
    Vector hinten_oben_links(CoordWidth, CoordHeight, -CoordDepth); // 6
    Vector hinten_oben_rechts(CoordWidth, CoordHeight, CoordDepth); // 7


    //Die Normalen fuer Beleuchtung berechnen
    Vector boden_normal = -hinten_unten_rechts.normal(hinten_unten_rechts, hinten_unten_links,
                                                      vorne_unten_links); /*BODEN*/

    Vector rechts_normal = hinten_unten_rechts.normal(hinten_unten_rechts, hinten_unten_links,
                                                      hinten_oben_rechts); /*RECHTS*/

    Vector hinten_normal = hinten_unten_rechts.normal(hinten_unten_links, vorne_unten_links,
                                                      hinten_oben_links); /*HINTEN*/

    Vector links_normal = -hinten_unten_rechts.normal(vorne_unten_rechts, vorne_unten_links,
                                                      vorne_oben_links); /*LINKS*/

    Vector oben_normal = -hinten_unten_rechts.normal(hinten_oben_links, hinten_oben_rechts,
                                                     vorne_oben_rechts); /*OBEN*/

    Vector vorne_normal = -hinten_unten_rechts.normal(hinten_unten_rechts, vorne_unten_rechts,
                                                      vorne_oben_rechts); /*VORNE*/

    //Unten
    // 0
    VB.addNormal(boden_normal);
    VB.addTexcoord0(1, 0);
    VB.addVertex(hinten_unten_rechts);
    // 1
    VB.addNormal(boden_normal);
    VB.addTexcoord0(1, 1);
    VB.addVertex(hinten_unten_links);
    // 2
    VB.addNormal(boden_normal);
    VB.addTexcoord0(0, 1);
    VB.addVertex(vorne_unten_links);
    // 3
    VB.addNormal(boden_normal);
    VB.addTexcoord0(0, 0);
    VB.addVertex(vorne_unten_rechts);
    //Seite rechts_normal
    // 4
    VB.addNormal(rechts_normal);
    VB.addTexcoord0(0, 0);
    VB.addVertex(hinten_oben_rechts);
    // 5
    VB.addNormal(rechts_normal);
    VB.addTexcoord0(1, 0);
    VB.addVertex(hinten_oben_links);
    // 6
    VB.addNormal(rechts_normal);
    VB.addTexcoord0(1, 1);
    VB.addVertex(hinten_unten_links);
    // 7
    VB.addNormal(rechts_normal);
    VB.addTexcoord0(0, 1);
    VB.addVertex(hinten_unten_rechts);

    //Seite hinten_normal
    // 8
    VB.addNormal(hinten_normal);
    VB.addTexcoord0(0, 0);
    VB.addVertex(hinten_oben_links);
    // 9
    VB.addNormal(hinten_normal);
    VB.addTexcoord0(1, 0);
    VB.addVertex(vorne_oben_links);
    // 10
    VB.addNormal(hinten_normal);
    VB.addTexcoord0(1, 1);
    VB.addVertex(vorne_unten_links);
    // 11
    VB.addNormal(hinten_normal);
    VB.addTexcoord0(0, 1);
    VB.addVertex(hinten_unten_links);

    // Seite n
    //12
    VB.addNormal(links_normal);
    VB.addTexcoord0(0, 0);
    VB.addVertex(vorne_oben_links);
    //13
    VB.addNormal(links_normal);
    VB.addTexcoord0(1, 0);
    VB.addVertex(vorne_oben_rechts);
    //14
    VB.addNormal(links_normal);
    VB.addTexcoord0(0, 1);
    VB.addVertex(vorne_unten_links);
    //15
    VB.addNormal(links_normal);
    VB.addTexcoord0(1, 1);
    VB.addVertex(vorne_unten_rechts);

    //Oben o
    //16
    VB.addNormal(oben_normal);
    VB.addTexcoord0(1, 1);
    VB.addVertex(hinten_oben_rechts);
    //17
    VB.addNormal(oben_normal);
    VB.addTexcoord0(1, 0);
    VB.addVertex(hinten_oben_links);
    //18
    VB.addNormal(oben_normal);
    VB.addTexcoord0(0, 0);
    VB.addVertex(vorne_oben_links);
    //19
    VB.addNormal(oben_normal);
    VB.addTexcoord0(0, 1);
    VB.addVertex(vorne_oben_rechts);

    //Vorne p
    // 20
    VB.addNormal(vorne_normal);
    VB.addTexcoord0(1, 1);
    VB.addVertex(hinten_unten_rechts);
    // 21
    VB.addNormal(vorne_normal);
    VB.addTexcoord0(0, 1);
    VB.addVertex(vorne_unten_rechts);
    // 22
    VB.addNormal(vorne_normal);
    VB.addTexcoord0(0, 0);
    VB.addVertex(vorne_oben_rechts);
    // 23
    VB.addNormal(vorne_normal);
    VB.addTexcoord0(1, 0);
    VB.addVertex(hinten_oben_rechts);

    VB.end();

    IB.begin();
    //Unten boden_normal
    IB.addIndex(0);
    IB.addIndex(3);
    IB.addIndex(2);

    IB.addIndex(2);
    IB.addIndex(1);
    IB.addIndex(0);

    //Seite rechts_normal
    IB.addIndex(6);
    IB.addIndex(5);
    IB.addIndex(4);

    IB.addIndex(4);
    IB.addIndex(7);
    IB.addIndex(6);

    //Seite hinten_normal
    IB.addIndex(10);
    IB.addIndex(9);
    IB.addIndex(8);

    IB.addIndex(8);
    IB.addIndex(11);
    IB.addIndex(10);

    //Seite links_normal
    IB.addIndex(14);
    IB.addIndex(15);
    IB.addIndex(13);

    IB.addIndex(13);
    IB.addIndex(12);
    IB.addIndex(14);


    //Seite oben_normal
    IB.addIndex(18);
    IB.addIndex(19);
    IB.addIndex(16);

    IB.addIndex(16);
    IB.addIndex(17);
    IB.addIndex(18);

    //Seite vorne_normal
    IB.addIndex(23);
    IB.addIndex(22);
    IB.addIndex(21);

    IB.addIndex(21);
    IB.addIndex(20);
    IB.addIndex(23);

    IB.end();

}

void TriangleBoxModel::draw(const BaseCamera &Cam) {
    //BaseModel::draw(Cam);

    // TODO: Add your code (Exercise 2)
    BaseModel::draw(Cam);

    VB.activate();
    IB.activate();

    glDrawElements(GL_TRIANGLES, IB.indexCount(), IB.indexFormat(), 0);

    IB.deactivate();
    VB.deactivate();
}