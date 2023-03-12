//
// Created by boery on 27.08.2022.
//

#include "FogShader.h"
// Aufbau der Klasse nach gegebener PhongShader Klasse.

/**
 * Konstruktor des Fog Shaders
 * Bei aufruf werden die zugehorigen Parameter fuer den Vertex und Fragment Shader vorbereitet
 */

FogShader::FogShader() :
        PostProcessingShader("vspostprocessing.glsl", "fsfog.glsl"),
        FogUpdateState(0xffffffff) {

    EyePosLoc = getParameterID("EyePos");
}

// Bei aufruf werden die zugehorigen Parameter fuer den Vertex und Fragment Shader vorbereitet
void FogShader::activate(const BaseCamera &Cam) const {
    PostProcessingShader::activate(Cam);

    Vector EyePos = Cam.position();
    FogUpdateState |= FOGUPDATESTATES::WEIGHTS_CHANGED;

    if (FogUpdateState & WEIGHTS_CHANGED) {
        glUniform3f(EyePosLoc, EyePos.X, EyePos.Y, EyePos.Z);
    }

    FogUpdateState = 0x0;
}


