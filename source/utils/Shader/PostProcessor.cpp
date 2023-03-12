//
// Created by boery on 27.08.2022.
//

#include "PostProcessor.h"

// Hilfsklasse die alle Post Processing Effekte zusammen blenden soll mithilfe von Frame Buffern.
// Der Aufbau der Klasse ist sehr stark an der Quelle angeleht. Pro Post Processing Effekt machen wir einen Platz mehr frei beim colorBuffer, pingpongFBO und pingpongBuffer
// Wir nutzen 3 Shader (Fog, Bloom, Godray) und benoetigen ein extra Platz für das gesamte zusammenblenden. Deshalb ist unser colorBuffer, pingpongFBO, pingpongBuffer.. [4] gross...
// Quelle: https://learnopengl.com/code_viewer_gh.php?code=src/5.advanced_lighting/7.bloom/bloom.cpp

PostProcessor::PostProcessor(int width, int height) : width(width), height(height) {

    // Instanziieren
    pBloom = new BloomShader();
    pFog = new FogShader();
    pGodray = new Godray();
    pPostProcessingShader = new PostProcessingShader();

    //render and frame buffer objects
    glGenFramebuffers(1, &FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glGenRenderbuffers(1, &RBO);
    glBindRenderbuffer(GL_RENDERBUFFER, RBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, RBO);

    // Wir nutzen 3 Shader + 1 zum Blenden also 4
    glGenTextures(4, colorBuffers);

    for (unsigned int i = 0; i < 4; i++) {
        glBindTexture(GL_TEXTURE_2D, colorBuffers[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, this->width, this->height, 0, GL_RGBA, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        // attatch the tex to frame buffer
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, colorBuffers[i], 0);

    }

    // render multi color buffers
    // Wir nutzen 3 Shader + 1 zum Blenden also 4
    unsigned int attachments[4] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3};

    // Wir nutzen 3 Shader + 1 zum Blenden also 4
    glDrawBuffers(4, attachments);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Framebuffer not complete!" << std::endl;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    //2 buffers ( ping pong verfahren ) für die 2 color buffer texture

    // Wir nutzen 3 Shader + 1 zum Blenden also 4
    glGenFramebuffers(4, pingpongFBO);
    glGenTextures(4, pingpongBuffer);
    for (unsigned int i = 0; i < 4; i++) {
        glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[i]);
        glBindTexture(GL_TEXTURE_2D, pingpongBuffer[i]);
        glTexImage2D(
                GL_TEXTURE_2D, 0, GL_RGBA16F, this->width, this->height, 0, GL_RGBA, GL_FLOAT, NULL
        );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(
                GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingpongBuffer[i], 0
        );
    }

    //init the VB for the frame
    this->InitVB();
}

/**
 * Destruktor, loeschend er Pointer
 */
PostProcessor::~PostProcessor() {
    delete pBloom;
    delete pFog;
    delete pGodray;
}

void PostProcessor::beforeDraw() {
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void PostProcessor::afterDraw() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void PostProcessor::InitVB() {
    unsigned int VBO;
    float quadVertices[] = {
            -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
            1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
            1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) (3 * sizeof(float)));

    //render quad
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

void PostProcessor::drawFX(Vector &moonPos) {
    bool horizontal = false, first_iteration = true;

    //GAUSS Filter, amount = 10 sind die iterationen für blur vertical + horizontal

    // BLOOM SHADER //
    pBloom->activate(Cam);
    for (unsigned int i = 0; i < 10; i++) {
        glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[horizontal]);
        glClear(GL_COLOR_BUFFER_BIT);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, first_iteration ? colorBuffers[1] : pingpongBuffer[!horizontal]);
        //render a quad for the frame
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glBindVertexArray(0);
        horizontal = !horizontal;
        if (first_iteration){
            first_iteration = false;
        }
    }
    pBloom->deactivate();


    // FOG SHADER //
    pFog->activate(Cam);
    glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[2]);
    glClear(GL_COLOR_BUFFER_BIT);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, colorBuffers[2]);
    //render a quad for the frame
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
    pFog->deactivate();



    // GODRAYS //
    // Mond Position vom Hauptprogramm
    pGodray->lpos(moonPos);
    pGodray->activate(Cam);
    glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[2]);
    glClear(GL_COLOR_BUFFER_BIT);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, colorBuffers[2]);
    //render a quad for the frame
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
    pGodray->deactivate();


    //mainshader blendet die bilder zusammen.
    pPostProcessingShader->activate(Cam);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, colorBuffers[0]);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, pingpongBuffer[!horizontal]);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, pingpongBuffer[2]);

    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, pingpongBuffer[2]);



    //render a quad for the frame
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);

    pPostProcessingShader->deactivate();
}
