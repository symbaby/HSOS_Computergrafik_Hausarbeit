//
// Created by boery on 27.08.2022.
//

#include <string>
#include "BloomShader.h"

// Quelle: https://learnopengl.com/Advanced-Lighting/Bloom
// Aufbau der Klasse nach gegebener PhongShader Klasse.

/**
 * Konstruktor des Bloom Shaders
 * Bei aufruf werden die zugehorigen Parameter fuer den Vertex und Fragment Shader vorbereitet
 */
BloomShader::BloomShader() :
PostProcessingShader("vspostprocessing.glsl", "fsbloom.glsl"),
        GaussUpdateState(0xffffffff) {
    for (unsigned int i = 0; i < 5; i++) {
        std::string locParameterName = "Weights[" + std::to_string(i) + "]";
        WeightsLoc[i] = getParameterID(locParameterName.c_str());
    }
}

/**
 * Bei aufruf werden die zugehorigen Parameter fuer den Vertex und Fragment Shader vorbereitet
 * @param Cam
 */
void BloomShader::activate(const BaseCamera &Cam) const {
    PostProcessingShader::activate(Cam);
    if (GaussUpdateState & WEIGHTS_CHANGED) {
        for (unsigned int i = 0; i < 5; i++) {
            setParameter(WeightsLoc[i], Weights[i]);
        }
    }

    UpdateState = 0x0;
}
