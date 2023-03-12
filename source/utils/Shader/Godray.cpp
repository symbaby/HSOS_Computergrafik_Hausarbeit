//
// Created by boery on 29.08.2022.
//

#include "Godray.h"

/**
 * Konstruktor des Godray Shaders
 * Bei aufruf werden die zugehorigen Parameter fuer den Vertex und Fragment Shader vorbereitet
 */
Godray::Godray() :
        PostProcessingShader("vspostprocessing.glsl", "fsgodray.glsl"),
        LPos(0, 0, 0),
        GodRaysUpdateState(0xffffffff) {


    LPosLoc = getParameterID("LPos");
}

// Bei aufruf werden die zugehorigen Parameter fuer den Vertex und Fragment Shader vorbereitet
void Godray::activate(const BaseCamera &Cam) const {
    PostProcessingShader::activate(Cam);
    if (GodRaysUpdateState & LIGHTPOS_CHANGED) {
        glUniform2f(LPosLoc, LPos.X, LPos.Y);
    }
    GodRaysUpdateState = 0x0;
}

// Setter fuer die Mond Position
void Godray::lpos(Vector LPos) {
    this->LPos = LPos;
    GodRaysUpdateState |= LIGHTUPDATESTATE::LIGHTPOS_CHANGED;

}