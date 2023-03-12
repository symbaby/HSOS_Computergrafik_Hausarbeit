//
// Created by boery on 22.08.2022.
//

#ifndef CGPRAKT6_CLION_TERRAIN_H
#define CGPRAKT6_CLION_TERRAIN_H


#include "utils/Models/BaseModel.h"
#include "../../include/VertexBuffer.h"
#include "../../include/IndexBuffer.h"
#include "../../include/Texture.h"
#include "utils/YourClasses/rgbimage.h"

#ifdef WIN32
#define ASSET_DIRECTORY "../assets/"
#else
#define ASSET_DIRECTORY "../assets/"
#endif


class Terrain : public BaseModel {
public:
    Terrain(const char *HeightMap = NULL, const char *DetailMap1 = NULL, const char *DetailMap2 = NULL);

    virtual ~Terrain();

    bool load(const char *HeightMap, const char *DetailMap1, const char *DetailMap2);

    virtual void shader(BaseShader *shader, bool deleteOnDestruction = false);

    virtual void draw(const BaseCamera &Cam);

    float width() const { return Size.X; }

    float height() const { return Size.Y; }

    float depth() const { return Size.Z; }

    void width(float v) { Size.X = v; }

    void height(float v) { Size.Y = v; }

    void depth(float v) { Size.Z = v; }

    const Vector &size() const { return Size; }

    void size(const Vector &s) { Size = s; }

protected:
    void applyShaderParameter();

    VertexBuffer VB;
    IndexBuffer IB;
    Texture DetailTex[2];
    Texture MixTex;
    Texture HeightTex;
    Vector Size;



};


#endif //CGPRAKT6_CLION_TERRAIN_H
