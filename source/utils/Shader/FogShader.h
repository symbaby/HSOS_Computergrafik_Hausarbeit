//
// Created by boery on 27.08.2022.
//

#ifndef CGPRAKT6_CLION_FOGSHADER_H
#define CGPRAKT6_CLION_FOGSHADER_H


#include "PostProcessingShader.h"

class FogShader : public PostProcessingShader {
public:
    FogShader();

    virtual ~FogShader() {};

    virtual void activate(const BaseCamera &Cam) const;


protected:
    GLint EyePosLoc;


    mutable unsigned int FogUpdateState;
    enum FOGUPDATESTATES {
        WEIGHTS_CHANGED = 1 << 0
    };

};


#endif //CGPRAKT6_CLION_FOGSHADER_H
