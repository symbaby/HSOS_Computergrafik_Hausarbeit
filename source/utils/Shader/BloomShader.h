//
// Created by boery on 27.08.2022.
//

#ifndef CGPRAKT6_CLION_BLOOMSHADER_H
#define CGPRAKT6_CLION_BLOOMSHADER_H


#include "PostProcessingShader.h"

// Quelle: https://learnopengl.com/Advanced-Lighting/Bloom
class BloomShader : public PostProcessingShader {

public:
    BloomShader();
    virtual ~BloomShader() {};
    virtual void activate(const BaseCamera& Cam) const;
protected:

    const float Weights[5] = { 0.2270270270f, 0.1945945946f, 0.1216216216f, 0.0540540541f, 0.0162162162f };
    GLint WeightsLoc[5];

    mutable unsigned int GaussUpdateState;
    enum BLOOMUPDATESTATES {
        WEIGHTS_CHANGED = 1 << 0
    };


};


#endif //CGPRAKT6_CLION_BLOOMSHADER_H
