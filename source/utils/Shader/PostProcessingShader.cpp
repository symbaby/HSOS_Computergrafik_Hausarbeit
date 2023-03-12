//
// Created by boery on 27.08.2022.
//

#include "PostProcessingShader.h"

// Quelle: https://learnopengl.com/Advanced-Lighting/Bloom

/**
 * Konstruktor fuer die Post Proccesing Effekte
 * Fuer jeden PostFX Effekt wird ein neuer Frame generiert der dann am ende zusammengeblendet wird
 */
PostProcessingShader::PostProcessingShader() :
        UpdateState(0xFFFFFFFF) {
    std::string vs = ASSET_DIRECTORY + std::string("vspostprocessing.glsl");
    std::string fs = ASSET_DIRECTORY + std::string("fspostprocessing.glsl");

    if (!load(vs.c_str(), fs.c_str())) {
        throw std::exception();
    }
    ColorTexLoc = getParameterID("MainFrame");
    BloomTexLoc = getParameterID("BloomFrame");
    FogTexLoc = getParameterID("FogFrame");
    GodrayTexLoc = getParameterID("GodrayFrame");
}

/**
 * Konstruktor fuer die Post Proccesing Effekte
 * Fuer jeden PostFX Effekt wird ein neuer Frame generiert der dann am ende zusammengeblendet wird
 */
PostProcessingShader::PostProcessingShader(std::string vertexShader, std::string fragmentShader)
        : UpdateState(0xFFFFFFFF) {
    std::string vs = ASSET_DIRECTORY + vertexShader;
    std::string fs = ASSET_DIRECTORY + fragmentShader;

    if (!load(vs.c_str(), fs.c_str())) {
        throw std::exception();
    }
    ColorTexLoc = getParameterID("MainFrame");
    BloomTexLoc = getParameterID("BloomFrame");
    FogTexLoc = getParameterID("FogFrame");
    GodrayTexLoc = getParameterID("GodrayFrame");
}

// Bei aufruf werden die zugehorigen Parameter fuer den Vertex und Fragment Shader vorbereitet
void PostProcessingShader::activate(const BaseCamera &Cam) const {
    BaseShader::activate(Cam);

    if (UpdateState & FIRST_CREATION) {
        glUniform1i(ColorTexLoc, 0);
        glUniform1i(BloomTexLoc, 1);
        glUniform1i(FogTexLoc, 2);
        glUniform1i(GodrayTexLoc, 3);
    }

    UpdateState = 0x0;
}

void PostProcessingShader::deactivate() const {
    BaseShader::deactivate();
}
