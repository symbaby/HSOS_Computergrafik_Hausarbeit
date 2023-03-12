//
//  Application.cpp
//  ogl4
//
//  Created by Philipp Lensing on 16.09.16.
//  Copyright © 2016 Philipp Lensing. All rights reserved.
//

#include "../include/Application.h"

#ifdef WIN32

#include <GL/glew.h>
#include <glfw/glfw3.h>

#define _USE_MATH_DEFINES

#include <math.h>

#else
#define GLFW_INCLUDE_GLCOREARB
#define GLFW_INCLUDE_GLEXT
#include <glfw/glfw3.h>
#endif

#include "utils/Models/LinePlaneModel.h"
#include "utils/Models/TrianglePlaneModel.h"
#include "utils/Models/TriangleSphereModel.h"
#include "utils/Models/LineBoxModel.h"
#include "utils/Models/Model.h"
#include "utils/Lights/ShaderLightMapper.h"
#include "utils/Shader/PhongShader.h"
#include "Game/Spieler.h"
#include "Background/TerrainShader.h"
#include "utils/Models/TriangleBoxModel.h"
#include <random>


#ifdef WIN32
#define ASSET_DIRECTORY "../assets/"
#else
#define ASSET_DIRECTORY "../assets/"
#endif

bool Application::amLeben = true;

Application::Application(GLFWwindow *pWin) : pWindow(pWin), Cam(pWin), pModel(NULL), ShadowGenerator(2048, 2048) {

    //createScene();
    //createNormalTestScene();
    //createShadowTestScene();

    int windowWidth, windowHeight;
    glfwGetWindowSize(pWindow, &windowWidth, &windowHeight);
    createGameScene();
    this->postProcessor = new PostProcessor(windowWidth, windowHeight);

}

/**
 * Spielszene erstellen
 */
void Application::createGameScene() {
    Matrix spawn, rm;

    // Phongshader fuer den Mond erstellen und eine Mond Textur laden
    pPhongShader = new PhongShader();
    pPhongShader->diffuseTexture(Texture::LoadShared(ASSET_DIRECTORY"moon.png"));
    pPhongShader->ambientColor(Color(1, 1, 1));
    pPhongShader->diffuseColor(Color(1, 1, 1));
    pPhongShader->specularColor(Color(1, 1, 1));


    // directional lights fuer die Beleuchtung der Szene
    auto *dl = new DirectionalLight();
    dl->direction(Vector(0, -5, 1));
    dl->color(Color(0.1, 0.1, 0.1));
    dl->castShadows(true);
    ShaderLightMapper::instance().addLight(dl);

    // directional lights fuer die Beleuchtung der Szene
    dl->direction(Vector(0, -5, -5));
    dl->color(Color(0.05, 0.05, 0.05));
    dl->castShadows(true);
    ShaderLightMapper::instance().addLight(dl);

    // Fuer die Godrays werden Punkt Licht erstellt die an  der Mond Position strahlen sollen
    Color c = Color(1.0f, 1.0f, 1.0f);
    Vector a = Vector(1, 0, 0.1f);
    for (int i = 0; i < 100; i++) {
        // point lights
        auto *pl = new PointLight();
        pl->position(Vector(15, 10, -35));
        pl->color(c);
        pl->attenuation(a);
        ShaderLightMapper::instance().addLight(pl);
    }


    // Fuer den Hintergrund der Szenerie das Terrain aus den Praktikum mit leicht abgedunkelten Texturen
    pTerrain = new Terrain();
    auto *pTerrainShader = new TerrainShader(ASSET_DIRECTORY);
    pTerrain->shader(pTerrainShader, true);
    pTerrain->load(ASSET_DIRECTORY"heightmap.bmp", ASSET_DIRECTORY"darkGrass.jpg", ASSET_DIRECTORY"darkRock.jpg");
    pTerrain->width(100);
    pTerrain->depth(60);
    pTerrain->height(5);
    spawn.translation(0, -1, -30);
    rm.rotationY(AI_RAD_TO_DEG(45));
    this->pTerrain->transform(spawn);
    Models.push_back(pTerrain);


    // Mond Modell
    this->pModel = new TriangleSphereModel(5, 32, 64);
    this->pModel->shader(pPhongShader, true);
    this->pModel->shadowCaster(false);
    Matrix moonMat, moonRM;
    Vector moon(15, 10, -35);
    moonMat.translation(moon);
    moonRM.rotationY(AI_RAD_TO_DEG(180));
    this->pModel->transform(moonMat * moonRM);
    this->Models.push_back(pModel);


    // Skybox
    this->pModel = new Model(ASSET_DIRECTORY "skybox.obj", false);
    this->pModel->shader(new PhongShader(), true);
    this->pModel->shadowCaster(false);
    this->Models.push_back(pModel);


    // Spieler
    this->pSpieler = new Spieler();
    this->pSpieler->shader(new PhongShader(), true);
    this->pSpieler->loadModels(ASSET_DIRECTORY "mc-grass-block.obj");
    this->Models.push_back(pSpieler);



    // Verfolger
    this->pVerfolger = new Verfolger();
    this->pVerfolger->shader(new PhongShader(), true);
    this->pVerfolger->loadModels(ASSET_DIRECTORY "verfolger_text.obj");
    this->Models.push_back(pVerfolger);
    this->pVerfolger->getVerfolgerModel()->setGefahr(true);
    this->spielWelt.push_back(pVerfolger->getVerfolgerModel());


    // Boden Spielfeld 1/3
    this->pModel = new Model(ASSET_DIRECTORY "floor.obj", false);
    this->pModel->shader(new PhongShader(), true);
    this->pModel->shadowCaster(false);
    spawn.translation(-100, -1, 0);
    this->pModel->transform(spawn);
    this->Models.push_back(pModel);
    this->spielWelt.push_back(pModel);

    // Boden Spielfeld 2/3
    this->pModel = new Model(ASSET_DIRECTORY "floor.obj", false);
    this->pModel->shader(new PhongShader(), true);
    this->pModel->shadowCaster(false);
    spawn.translation(0, -1, 0);
    this->pModel->transform(spawn);
    this->Models.push_back(pModel);
    this->spielWelt.push_back(pModel);

    // Boden Spielfeld 3/3
    this->pModel = new Model(ASSET_DIRECTORY "floor.obj", false);
    this->pModel->shader(new PhongShader(), true);
    this->pModel->shadowCaster(false);
    spawn.translation(100, -1, 0);
    this->pModel->transform(spawn);
    this->Models.push_back(pModel);
    this->spielWelt.push_back(pModel);


    // Hindernisse und Plattformen
    this->createLevel();


    // Triangle Box Model mit Deathscreen Textur
    Matrix deathMat;
    PhongShader *deathScreen = new PhongShader;
    deathScreen->diffuseTexture(Texture::LoadShared(ASSET_DIRECTORY"deathscreen.png"));
    deathMat.translation(200, 0, 0);
    this->pModel = new TriangleBoxModel(8, 5, 1);
    this->pModel->shader(deathScreen, true);
    this->pModel->transform(deathMat);
    this->Models.push_back(pModel);


    // Triangle Box Model mit Victory Textur
    Matrix victoryMat;
    auto *victoryScreen = new PhongShader;
    victoryScreen->diffuseTexture(Texture::LoadShared(ASSET_DIRECTORY"win.png"));
    victoryMat.translation(250, 0, 0);
    this->pModel = new TriangleBoxModel(8, 5, 1);
    this->pModel->shader(victoryScreen, true);
    this->pModel->transform(victoryMat);
    this->Models.push_back(pModel);



    // Const Shader fuer die Visualisierung der Bounding Boxen
    ConstantShader *pConst;
    pConst = new ConstantShader();
    pConst->color(Color(0, 1, 0));

    // Bounding Box des Verfolgers visualisieren
    hitboxEnemy = new LineBoxModel(pVerfolger->getVerfolgerModel()->boundingBox().Max,pVerfolger->getVerfolgerModel()->boundingBox().Min);
    hitboxEnemy->shader(pConst, true);
    Models.push_back(hitboxEnemy);

    // Bounding Box des Spielers visualisieren
    hitboxPlayer = new LineBoxModel(pSpieler->getSpielerModel()->boundingBox().Max,pSpieler->getSpielerModel()->boundingBox().Min);
    hitboxPlayer->shader(pConst, true);
    Models.push_back(hitboxPlayer);

}


/**
 * Hier platzieren wir unsere Bloecke, die unser Level definieren. Wir unterscheiden dabei zwischen Bloecken auf denen wir uns bewegen und beruehren koennen und Bloecken,
 * die unseren Charakter zerstoeren.
 * Ebenfalls fuellen wir das Level mit paar Baeumen und einem Wurfel, der das Ende des Levels darstellt.
 */
void Application::createLevel() {
    Matrix spawn;
    this->erstelleSpielobjekt(3, -39, 0, 0, ASSET_DIRECTORY"mc-grass-block.obj", false);

    this->erstelleSpielobjekt(2, -36, 0, 0, ASSET_DIRECTORY"mc-grass-block.obj", true);

    this->erstelleSpielobjekt(3, -34, 0, 0, ASSET_DIRECTORY"mc-grass-block.obj", false);

    this->erstelleSpielobjekt(1, -31, 0, 0, ASSET_DIRECTORY"mc-grass-block.obj", true);

    this->erstelleSpielobjekt(2, -30, 1, 0, ASSET_DIRECTORY"mc-grass-block.obj", false);

    this->erstelleSpielobjekt(2, -28, 0, 0, ASSET_DIRECTORY"mc-grass-block.obj", true);

    this->erstelleSpielobjekt(2, -26, 0, 0, ASSET_DIRECTORY"mc-grass-block.obj", false);

    this->erstelleSpielobjekt(6, -24, 0, 0, ASSET_DIRECTORY"mc-grass-block.obj", true);

    this->erstelleSpielobjekt(3, -23, 1, 0, ASSET_DIRECTORY"mc-grass-block.obj", false);

    this->erstelleSpielobjekt(2, -18, 0, 0, ASSET_DIRECTORY"mc-grass-block.obj", false);

    this->erstelleSpielobjekt(38, -16, 0, 0, ASSET_DIRECTORY"mc-grass-block.obj", true);

    this->erstelleSpielobjekt(2, -15, 1, 0, ASSET_DIRECTORY"mc-grass-block.obj", false);

    this->erstelleSpielobjekt(1, -12, 2, 0, ASSET_DIRECTORY"mc-grass-block.obj", false);

    this->erstelleSpielobjekt(1, -8, 2, 0, ASSET_DIRECTORY"mc-grass-block.obj", false);

    this->erstelleSpielobjekt(1, -4, 2, 0, ASSET_DIRECTORY"mc-grass-block.obj", false);

    this->erstelleSpielobjekt(1, -1, 2, 0, ASSET_DIRECTORY"mc-grass-block.obj", false);

    this->erstelleSpielobjekt(1, 6, 2, 0, ASSET_DIRECTORY"mc-grass-block.obj", false);

    this->erstelleSpielobjekt(1, 13, 1, 0, ASSET_DIRECTORY"mc-grass-block.obj", false);

    this->erstelleSpielobjekt(1, 18, 1, 0, ASSET_DIRECTORY"mc-grass-block.obj", false);

    this->erstelleSpielobjekt(1, 18, 1, 0, ASSET_DIRECTORY"mc-grass-block.obj", false);

    this->erstelleSpielobjekt(2, 22, 0, 0, ASSET_DIRECTORY"mc-grass-block.obj", false);

    this->erstelleSpielobjekt(10, 24, 0, 0, ASSET_DIRECTORY"mc-grass-block.obj", true);

    this->erstelleSpielobjekt(1, 26, 1, 0, ASSET_DIRECTORY"mc-grass-block.obj", false);

    this->erstelleSpielobjekt(1, 29, 2, 0, ASSET_DIRECTORY"mc-grass-block.obj", false);


    // Ziel Box - das Ende des Levels
    auto *zielBoxShader = new ConstantShader();
    zielBoxShader->color(Color(0, 0, 1));
    this->pZiel = new Model(ASSET_DIRECTORY"mc-grass-block.obj", false);
    this->pZiel->shader(zielBoxShader, true);
    this->pZiel->shadowCaster(false);
    spawn.translation(31, 1, 0);
    this->pZiel->transform(spawn);
    this->pZiel->setZiel(true);
    this->Models.push_back(pZiel);
    this->spielWelt.push_back(pZiel);


    // Baeume
    // this->erstelleSpielobjekt(3, -60, -1, -4, ASSET_DIRECTORY"Pine_Tree.obj", false);
    this->erstelleBaeume(3, -50, -1, -13, 3.0, ASSET_DIRECTORY"Pine_Tree.obj");
    this->erstelleBaeume(4, -47, -2, -12, 2.0, ASSET_DIRECTORY"Pine_Tree.obj");
    this->erstelleBaeume(6, -50, -3, -10, 3.0, ASSET_DIRECTORY"Pine_Tree.obj");

    this->erstelleBaeume(3, -30, -1, -11, 3.0, ASSET_DIRECTORY"Pine_Tree.obj");
    this->erstelleBaeume(4, -27, -2, -9, 2.0, ASSET_DIRECTORY"Pine_Tree.obj");
    this->erstelleBaeume(2, -30, -3, -7, 3.0, ASSET_DIRECTORY"Pine_Tree.obj");


}


/**
 * Per Baukasten Prinzip parametrierbar Bloecke der Szene hinzufuegen
 *
 * @param count wie viele Bloecke
 * @param abPositionX ab welcher X-Koordinate sollen die Bloecke spawnen
 * @param positionY auf welcher X-Koordinate sollen die Bloecke spawnen
 * @param positionZ auf welcher X-Koordinate sollen die Bloecke spawnen
 * @param file welches Model was im Asset Directory liegt
 * @param gefahr true wenn der Spieler bei Kollsion sterben soll, ansonsten false
 */
void Application::erstelleSpielobjekt(int count, int abPositionX, int positionY, int positionZ, const char *file,
                                      bool gefahr) {
    for (int i = 0; i < count; i++) {
        Matrix spawn;

        // Wenn Gefahren Objekt dann nimm "Spike" Modell und setzte Gefahr des Objekts auf true
        if (gefahr) {
            this->pModel = new Model(ASSET_DIRECTORY"cone.obj", false);
            this->pModel->shader(new PhongShader(), true);
            this->pModel->shadowCaster(false);
            spawn.translation((float) abPositionX, (float) positionY - 0.80, positionZ);
            this->pModel->transform(spawn);
            this->pModel->setGefahr(true);
            this->Models.push_back(pModel);
            this->spielWelt.push_back(pModel);

        } else {
            // Ansonsten nimm normales Model was uebergeben wird
            this->pModel = new Model(file, false);
            this->pModel->shader(new PhongShader(), true);
            this->pModel->shadowCaster(false);
            spawn.translation((float) abPositionX, (float) positionY, positionZ);
            this->pModel->transform(spawn);
            this->Models.push_back(pModel);
            this->spielWelt.push_back(pModel);
        }
        abPositionX++;
    }
}

/**
 * Per Baukasten Prinzip parametrierbar Baeume der Szene hinzufuegen
 *
 * @param count wie viele Baeueme wir haben wolllen
 * @param abPositionX ab welcher X-Koordinate sollen die Baeume spawnen
 * @param positionY auf welcher Y-Koordinate sollen die Baeume spawnen
 * @param positionZ auf welche Z-Koordinate sollen die Baeume spawnen
 * @param margin abstand zwischen den Baeumen
 * @param file welches Model was im Asset Directory liegt
 */
void Application::erstelleBaeume(int count, int abPositionX, int positionY, int positionZ, float margin,
                                 const char *file) {
    Matrix oldPos;
    bool first = true;

    for (int i = 0; i < count; i++) {
        Matrix spawn;

        this->pModel = new Model(file, false);
        this->pModel->shader(new PhongShader(), true);
        this->pModel->shadowCaster(false);
        if(first){
            spawn.translation((float) abPositionX + margin, (float) positionY, positionZ + margin);
        } else {
            spawn.translation((float) oldPos.translation().X + margin, (float) positionY, positionZ + margin);
        }

        oldPos = spawn;
        this->pModel->transform(spawn);
        this->Models.push_back(pModel);
        this->spielWelt.push_back(pModel);
        abPositionX++;
        first = false;
    }
}

/**
 *
 * @param forwardBackward der Input der Tastatur wird im Parameter gespeichert und steht im Hauptprogramm fuer das Veraendern der SpielerMatrix zur Verfuegung.
 */
void Application::processKeyPress(float &forwardBackward) {

    // Jeder Frame wird der zu 0 damit die Addition von
    // this->forwardBackward nicht unendlich hoch wird.
    this->forwardBackward = 0.f;


    if (glfwGetKey(pWindow, GLFW_KEY_D) == GLFW_PRESS) {
        this->pSpieler->setMovingRight(true);
        this->forwardBackward += 1.5;
    }


    if (glfwGetKey(pWindow, GLFW_KEY_D) == GLFW_RELEASE) {
        this->pSpieler->setMovingRight(false);
    }


    if (glfwGetKey(pWindow, GLFW_KEY_A) == GLFW_PRESS) {
        this->pSpieler->setMovingLeft(true);
        //this->pSpieler->setMovingRight(false);
        this->forwardBackward -= 1.5;
    }


    if (glfwGetKey(pWindow, GLFW_KEY_A) == GLFW_RELEASE) {
        this->pSpieler->setMovingLeft(false);
    }


    if (glfwGetKey(pWindow, GLFW_KEY_W) == GLFW_PRESS) {
        //std::cout << "W gedrueckt" << std::endl;
        if (!this->pSpieler->isJumping() && !this->pSpieler->isFalling()) {
            this->pSpieler->setJumping(true);
        }
    }

    if (glfwGetKey(pWindow, GLFW_KEY_W) == GLFW_RELEASE) {
        if (this->pSpieler->isJumping()) {
            this->pSpieler->setJumping(false);
            if (!this->pSpieler->isFalling()) {
                this->pSpieler->setFalling(true);
            }
        }
    }
}

void Application::start() {
    glEnable(GL_DEPTH_TEST); // enable depth-testing
    glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

/**
 * update Methode unseres Kernprogramms
 *
 * @param dtime Ablauf des Spielgeschehens abhaengig von der Zeit - nicht Rechenleistung
 */
void Application::update(float dtime) {

    // Keypress speichert nun je nach Input ForwardBackward -1 oder +1;
    this->letzteSpielerPosition = this->pSpieler->getSpielerMatrix();

    // A, W, D Press
    this->processKeyPress(this->forwardBackward);

    // Input veraendert die Spieler Matrix
    this->pSpieler->move(this->forwardBackward);

    // Hier wird mit der "neune" Spieler Matrix die Logik berechnet
    this->pSpieler->update(dtime, spielWelt, letzteSpielerPosition, Cam);

    if(amLeben == true){
        this->pVerfolger->update(dtime, Cam);
    }


    // Collision Check in update
    // this->doCollision();

    // Updaten der Bounding Boxen
    this->hitboxEnemy->transform(this->pVerfolger->getVerfolgerModel()->transform());
    this->hitboxPlayer->transform(this->pSpieler->getSpielerModel()->transform());


    // Spielerei
    Matrix RM;
    RM.rotationY(AI_DEG_TO_RAD(30) * dtime);
    pZiel->transform(pZiel->transform() * RM);

    Cam.update();
}


void Application::draw() {

    ShadowGenerator.generate(Models);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    ShaderLightMapper::instance().activate();


    // glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    this->postProcessor->beforeDraw();

    for (auto it = Models.begin(); it != Models.end(); ++it) {
        (*it)->draw(Cam);
    }

    // Dummy Vektor wo die Postion des Mondes im korrekten Transformationsraum gespeichert wird
    Vector moonPos;

    // errechne die Postion des Mondes
    this->calcMoonPos(Vector(15, 10, -35), moonPos);
    this->postProcessor->drawFX(moonPos); // ## Auskommentieren fuer kein Post Processing
    ShaderLightMapper::instance().deactivate();


    //  glBindFramebuffer(GL_FRAMEBUFFER, 0);
    this->postProcessor->afterDraw();


    // 3. check once per frame for opengl errors
    GLenum Error = glGetError();
    assert(Error == 0);


}

/**
 * Modelle werden geloescht - Speicher wird freigegeben
 */
void Application::end() {
    for (auto it = Models.begin(); it != Models.end(); ++it)
        delete *it;

    Models.clear();
}

/**
 * Transformationspipeline durchgehen und die Postion des Mondes ausrechnen
 *
 * @param moonPos die aktuelle Position des Mondel im "Weltraum"
 * @param v leerer Vektor der in der Methode beschrieben wird
 */
void Application::calcMoonPos(Vector moonPos, Vector &v) {

    // View Matrix und Projektionsmatrix
    Matrix modelViewM = Cam.getViewMatrix();
    Matrix modelProjectionM = Cam.getProjectionMatrix() * modelViewM;

    // Mond Position wird mit Model Projektions Matrix multipliziert
    v = modelProjectionM * moonPos;


    // Alle Objekte befinden sich in einem Raum, in dem die Kamera im Ursprung liegt
    // Transformationspipeline hochgehen wie in Praktikum 4
    Vector camPosition = Cam.position();
    Matrix projectionCam = Cam.getProjectionMatrix();
    projectionCam.invert();

    Vector tmp = moonPos;
    moonPos = projectionCam * tmp;

    Vector camToMoon = moonPos - camPosition; // Distanzvektor von Kamera zu Mond ausrechnen

    Matrix viewMatrix = Cam.getViewMatrix();
    Vector camAngle = viewMatrix.invert().transformVec3x3(tmp);


    if (camAngle.dot(camToMoon) < 0) {
        v.X = 1 - (1 + v.X) / 2;
        v.Y = 1 - (1 + v.Y) / 2;
    } else {
        v.X = (1 + v.X) / 2;
        v.Y = (1 + v.Y) / 2;
    }

}


void Application::createScene() {
    Matrix m, n;

    pModel = new Model(ASSET_DIRECTORY "skybox.obj", false);
    pModel->shader(new PhongShader(), true);
    pModel->shadowCaster(false);
    Models.push_back(pModel);


    pModel = new Model(ASSET_DIRECTORY "scene.dae", false);
    pModel->shader(new PhongShader(), true);
    m.translation(10, 0, -10);
    pModel->transform(m);
    Models.push_back(pModel);


    // directional lights
    auto *dl = new DirectionalLight();
    dl->direction(Vector(0.2f, -1, 1));
    dl->color(Color(0.25, 0.25, 0.5));
    dl->castShadows(true);
    ShaderLightMapper::instance().addLight(dl);

    Color c = Color(1.0f, 0.7f, 1.0f);
    Vector a = Vector(1, 0, 0.1f);
    float innerradius = 45;
    float outerradius = 60;

    // point lights
    PointLight *pl = new PointLight();
    pl->position(Vector(-1.5, 3, 10));
    pl->color(c);
    pl->attenuation(a);
    ShaderLightMapper::instance().addLight(pl);

    pl = new PointLight();
    pl->position(Vector(5.0f, 3, 10));
    pl->color(c);
    pl->attenuation(a);
    ShaderLightMapper::instance().addLight(pl);

    pl = new PointLight();
    pl->position(Vector(-1.5, 3, 28));
    pl->color(c);
    pl->attenuation(a);
    ShaderLightMapper::instance().addLight(pl);

    pl = new PointLight();
    pl->position(Vector(5.0f, 3, 28));
    pl->color(c);
    pl->attenuation(a);
    ShaderLightMapper::instance().addLight(pl);

    pl = new PointLight();
    pl->position(Vector(-1.5, 3, -8));
    pl->color(c);
    pl->attenuation(a);
    ShaderLightMapper::instance().addLight(pl);

    pl = new PointLight();
    pl->position(Vector(5.0f, 3, -8));
    pl->color(c);
    pl->attenuation(a);
    ShaderLightMapper::instance().addLight(pl);


    // spot lights
    SpotLight *sl = new SpotLight();
    sl->position(Vector(-1.5, 3, 10));
    sl->color(c);
    sl->direction(Vector(1, -4, 0));
    sl->innerRadius(innerradius);
    sl->outerRadius(outerradius);
    ShaderLightMapper::instance().addLight(sl);

    sl = new SpotLight();
    sl->position(Vector(5.0f, 3, 10));
    sl->color(c);
    sl->direction(Vector(-1, -4, 0));
    sl->innerRadius(innerradius);
    sl->outerRadius(outerradius);
    ShaderLightMapper::instance().addLight(sl);

    sl = new SpotLight();
    sl->position(Vector(-1.5, 3, 28));
    sl->color(c);
    sl->direction(Vector(1, -4, 0));
    sl->innerRadius(innerradius);
    sl->outerRadius(outerradius);
    ShaderLightMapper::instance().addLight(sl);

    sl = new SpotLight();
    sl->position(Vector(5.0f, 3, 28));
    sl->color(c);
    sl->direction(Vector(-1, -4, 0));
    sl->innerRadius(innerradius);
    sl->outerRadius(outerradius);
    ShaderLightMapper::instance().addLight(sl);

    sl = new SpotLight();
    sl->position(Vector(-1.5, 3, -8));
    sl->color(c);
    sl->direction(Vector(1, -4, 0));
    sl->innerRadius(innerradius);
    sl->outerRadius(outerradius);
    ShaderLightMapper::instance().addLight(sl);

    sl = new SpotLight();
    sl->position(Vector(5.0f, 3, -8));
    sl->color(c);
    sl->direction(Vector(-1, -4, 0));
    sl->innerRadius(innerradius);
    sl->outerRadius(outerradius);
    ShaderLightMapper::instance().addLight(sl);

}

void Application::createNormalTestScene() {
    pModel = new LinePlaneModel(10, 10, 10, 10);
    ConstantShader *pConstShader = new ConstantShader();
    pConstShader->color(Color(0, 0, 0));
    pModel->shader(pConstShader, true);
    // add to render list
    Models.push_back(pModel);


    pModel = new Model(ASSET_DIRECTORY "cube.obj", false);
    pModel->shader(new PhongShader(), true);
    Models.push_back(pModel);


}


void Application::createShadowTestScene() {
    pModel = new Model(ASSET_DIRECTORY "shadowcube.obj", false);
    pModel->shader(new PhongShader(), true);
    Models.push_back(pModel);


    pModel = new Model(ASSET_DIRECTORY "bunny.dae", false);
    pModel->shader(new PhongShader(), true);
    Models.push_back(pModel);


    // directional lights
    auto *dl = new DirectionalLight();
    dl->direction(Vector(0, -1, -1));
    dl->color(Color(0.5, 0.5, 0.5));
    dl->castShadows(true);
    ShaderLightMapper::instance().addLight(dl);

    auto *sl = new SpotLight();
    sl->position(Vector(2, 2, 0));
    sl->color(Color(0.5, 0.5, 0.5));
    sl->direction(Vector(-1, -1, 0));
    sl->innerRadius(10);
    sl->outerRadius(13);
    sl->castShadows(true);
    ShaderLightMapper::instance().addLight(sl);

}






