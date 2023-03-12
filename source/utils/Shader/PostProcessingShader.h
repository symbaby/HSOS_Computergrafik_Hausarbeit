//
// Created by boery on 27.08.2022.
//

#ifndef CGPRAKT6_CLION_POSTPROCESSINGSHADER_H
#define CGPRAKT6_CLION_POSTPROCESSINGSHADER_H


#include "BaseShader.h"

#ifdef WIN32
#define ASSET_DIRECTORY "../assets/"
#else
#define ASSET_DIRECTORY "../assets/"
#endif

// Architektur vom PhongShader geklaut
// Quelle: https://learnopengl.com/Advanced-Lighting/Bloom
class PostProcessingShader : public BaseShader {

public:
    PostProcessingShader();

    virtual ~PostProcessingShader() {}

    PostProcessingShader(std::string vertexShader, std::string fragmentShader);

    virtual void activate(const BaseCamera &Cam) const;

    virtual void deactivate() const;

protected:
    GLint ColorTexLoc;
    GLint BloomTexLoc;
    GLint FogTexLoc;
    GLint GodrayTexLoc;


    mutable unsigned int UpdateState;
    enum UPDATESTATES {
        FIRST_CREATION = 1 << 0
    };

};


#endif //CGPRAKT6_CLION_POSTPROCESSINGSHADER_H
