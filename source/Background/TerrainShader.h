//
// Created by boery on 22.08.2022.
//

#ifndef CGPRAKT6_CLION_TERRAINSHADER_H
#define CGPRAKT6_CLION_TERRAINSHADER_H


#include "utils/Shader/PhongShader.h"

class TerrainShader : public PhongShader
{
public:
    enum {
        DETAILTEX0=0,
        DETAILTEX1,
        DETAILTEX_COUNT
    };

    TerrainShader(const std::string& AssetDirectory);
    virtual ~TerrainShader() {}
    virtual void activate(const BaseCamera& Cam) const;
    virtual void deactivate() const;

    const Texture* detailTex(unsigned int idx) const { assert(idx<DETAILTEX_COUNT); return DetailTex[idx]; }
    const Texture* mixTex() const { return MixTex; }

    void detailTex(unsigned int idx, const Texture* pTex) { assert(idx<DETAILTEX_COUNT); DetailTex[idx] = pTex; }
    void mixTex(const Texture* pTex) { MixTex = pTex; }

    void scaling(const Vector& s) { Scaling = s; }
    const Vector& scaling() const { return Scaling; }

    void time(float d);

private:
    void activateTex(const Texture* pTex, GLint Loc, int slot) const;

    float Time;

    const Texture* MixTex;
    const Texture* DetailTex[DETAILTEX_COUNT];
    Vector Scaling;
    // shader locations
    GLint MixTexLoc;
    GLint DetailTexLoc[DETAILTEX_COUNT];
    GLint ScalingLoc;

    // NEU
    GLint kLoc;
};

#endif //CGPRAKT6_CLION_TERRAINSHADER_H
