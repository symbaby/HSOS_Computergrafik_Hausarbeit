//
// Created by boery on 19.08.2022.
//

#include "Verfolger.h"

/**
 * Konstruktor des Verfolgers
 * Instanzvariablen setzten
 */
Verfolger::Verfolger() {
    this->forwardBackward = 0;
}

/**
 * Destruktor des Verfoglers
 * Pointer Loeschen
 */
Verfolger::~Verfolger() {
    delete this->pVerfolgerModel;
}

/**
 * Modell fuer den Verfolger laden, und in der Spielwelt an einer bestimmten Koordinate spawnen
 *
 * @param model das im Asset Directory liegt
 * @return true wenn erfolgreich, false wenn Fehler auftritt
 */
bool Verfolger::loadModels(const char *model) {

    if (model != nullptr) {
        // Instanziierung
        this->pVerfolgerModel = new Model(model, false);
        this->pVerfolgerModel->shader(pShader);

        // Verfolger verschiebung nach Links damit der Spieler Zeit hat von ihn zu fluechten
        Matrix spawnPunkt;
        // spawnPunkt.translation(4.5, -1, 0);
        spawnPunkt.translation(-70, -1, 0);
        this->pVerfolgerModel->transform(spawnPunkt);
        // this->bbVerfolger.transform(this->transform());

        return true;
    } else {
        std::cout << "Verfolger::loadModels() Fehler beim laden des Models" << std::endl;
        return false;
    }
}

/**
 *
 * @param forwardBackward input Parameter, der die Bewegung des Verfolgers definiert
 */
void Verfolger::move(float forwardBackward) {
    this->verfolgerBewegung.X = forwardBackward;
}

/**
 *
 * @param dtime abhaengig machen von der Zeit - nicht der Framerate
 * @param cam Kamera aus dem Hauptoprogramm wird hier benoetigt, um sie mit zu translaten
 */
void Verfolger::update(float dtime, Camera &cam) {
    // std::cout << "Updated" << std::endl;
    Matrix verfolgerPositionMat;
    Vector verfolgerPositionVec;

    verfolgerPositionMat = pVerfolgerModel->transform();
    verfolgerPositionVec = verfolgerPositionMat.translation();


    // Offset fuer die Kamera
    verfolgerPositionVec.X += 5.f;
    verfolgerPositionVec.Y += 1.f;

    // Kamera guckt auf Verfolger
    cam.setTarget(verfolgerPositionVec);
    verfolgerPositionVec.Z += 7.f; // Kamera offset
    cam.setPosition(verfolgerPositionVec);

    // Verfolger bewegt sich mit konstanter geschwindigkeit
    this->move(0.8f);

    this->verfolgerMatrix.translation(this->verfolgerBewegung.X * dtime, 0, 0);
    this->verfolgerMatrix = this->pVerfolgerModel->transform() * this->verfolgerMatrix;
    this->pVerfolgerModel->transform(this->verfolgerMatrix);


    // Boundig Box des Verfolgers aktualisieren
    this->bbVerfolger = this->getVerfolgerModel()->boundingBox().transform(this->getVerfolgerModel()->transform());
}

// zeichne Modell
void Verfolger::draw(const BaseCamera &cam) {
    this->pVerfolgerModel->draw(cam);
}

 // Getter
Model *Verfolger::getVerfolgerModel() const {
    return pVerfolgerModel;
}
