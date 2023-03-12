//
//  Application.hpp
//  ogl4
//
//  Created by Philipp Lensing on 16.09.16.
//  Copyright © 2016 Philipp Lensing. All rights reserved.
//

#ifndef Application_hpp
#define Application_hpp

#include <stdio.h>
#include <list>
#include "camera.h"

#include "utils/Shader/ConstantShader.h"
#include "vertexbuffer.h"
#include "indexbuffer.h"
#include "utils/Models/BaseModel.h"
#include "utils/Lights/ShadowMapGenerator.h"
#include "Game/Spieler.h"
#include "Game/Verfolger.h"
#include "utils/Models/LineBoxModel.h"
#include "utils/Shader/PhongShader.h"
#include "Background/Terrain.h"
#include "utils/Shader/PostProcessor.h"



class Application {
public:
    typedef std::list<BaseModel *> ModelList;

    Application(GLFWwindow *pWin);

    void start();

    void update(float dtime);

    void draw();

    void end();

    static bool amLeben;


protected:
    void createScene();

    void createNormalTestScene();

    void createShadowTestScene();

    void createGameScene();

    Camera Cam;
    ModelList Models;
    GLFWwindow *pWindow;
    BaseModel *pModel;
    ShadowMapGenerator ShadowGenerator;

private:
    // ------------- AB HIER UNSER STUFF ------------------ //
    Spieler *pSpieler = nullptr;
    Verfolger *pVerfolger = nullptr;
    BaseModel *pZiel;


    PostProcessor *postProcessor;
    LineBoxModel *hitboxPlayer;
    LineBoxModel *hitboxEnemy;
    PhongShader *pPhongShader;
    Terrain *pTerrain;

    Matrix letzteSpielerPosition;
    float forwardBackward = 0;

    std::vector<BaseModel *> spielWelt;

    void calcMoonPos(Vector sunPosition, Vector& v);

    void createLevel();





private:

    void processKeyPress(float &forwardBackward);

    void erstelleSpielobjekt(int count, int abPositionX, int positionY, int positionZ, const char *file, bool gefahr);
    void erstelleBaeume(int count, int abPositionX, int positionY, int positionZ, float margin, const char *file);




};

#endif /* Application_hpp */
