//
//  ConstantShader.hpp
//  ogl4
//
//  Created by Philipp Lensing on 16.09.16.
//  Copyright © 2016 Philipp Lensing. All rights reserved.
//

#ifndef ConstantShader_hpp
#define ConstantShader_hpp

#include <stdio.h>
#ifdef WIN32
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#else
#define GLFW_INCLUDE_GLCOREARB
#define GLFW_INCLUDE_GLEXT
#include <glfw/glfw3.h>
#endif
#include <iostream>
#include <assert.h>
#include "utils/YourClasses/color.h"
#include "utils/YourClasses/vector.h"
#include "../../../include/Matrix.h"
#include "../../../include/Camera.h"
#include "BaseShader.h"


class ConstantShader : public BaseShader
{
public:
    ConstantShader();
    void color( const Color& c);
    const Color& color() const { return Col; }
    virtual void activate(const BaseCamera& Cam) const;
private:
    Color Col;
    GLuint ShaderProgram;
    GLint ColorLoc;
    GLint ModelViewProjLoc;
    
};

#endif /* ConstantShader_hpp */
