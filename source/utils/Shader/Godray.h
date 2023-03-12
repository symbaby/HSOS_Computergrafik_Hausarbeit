//
// Created by boery on 29.08.2022.
//

#ifndef CGPRAKT6_CLION_GODRAY_H
#define CGPRAKT6_CLION_GODRAY_H


#include "PostProcessingShader.h"

class Godray : public PostProcessingShader {
public:
    Godray();
    virtual void activate(const BaseCamera& Cam) const;
    void lpos(Vector LPos);

protected:
    GLint LPosLoc;
    Vector LPos;


    mutable unsigned int GodRaysUpdateState;
    enum LIGHTUPDATESTATE {
        LIGHTPOS_CHANGED = 1 << 0
    };

};


#endif //CGPRAKT6_CLION_GODRAY_H
