//
// Created by boery on 27.08.2022.
//

#ifndef CGPRAKT6_CLION_POSTPROCESSOR_H
#define CGPRAKT6_CLION_POSTPROCESSOR_H


#include "utils/YourClasses/vector.h"
#include "../../../include/Camera.h"
#include "BloomShader.h"
#include "FogShader.h"
#include "Godray.h"

class PostProcessor {
public:
    PostProcessor(int width, int heigh);

    virtual ~PostProcessor();

    void InitVB();

    void drawFX(Vector& moonPos);

    void beforeDraw();

    void afterDraw();

protected:
    int width, height;
    unsigned int colorBuffers[4];
    unsigned int pingpongFBO[4];
    unsigned int pingpongBuffer[4];


    SimpleCamera Cam;
    GLuint VAO;
    GLuint FBO;
    GLuint RBO;
    BloomShader *pBloom;
    FogShader *pFog;
    Godray *pGodray;
    PostProcessingShader *pPostProcessingShader;

private:


};


#endif //CGPRAKT6_CLION_POSTPROCESSOR_H
