//
// Created by boery on 19.08.2022.
//

#include <list>
#include "Spieler.h"
#include "../../include/Application.h"
#include <chrono>


/**
 * Konstruktor des Spielers
 * Instanzvariablen setzen
 */
Spieler::Spieler() {
    this->spielerMatrix = this->transform();
    this->forwardBackward = 0;
    this->jumping = false;
    this->falling = false;
    this->movingRight = false;
    this->movingLeft = false;
}

/**
 * Destruktor des Spielers
 * Pointer loeschen
 */
Spieler::~Spieler() {
    delete this->pSpielerModel;
}

/**
 * Modell fuer den Spieler laden, und in der Spielwelt an einer bestimmten Koordinate spawnen
 *
 * @param model das im Asset Directory liegt
 * @return true wenn erfolgreich, false wenn Fehler auftritt
 */
bool Spieler::loadModels(const char *model) {
    if (model != nullptr) {
        // Instanziierung
        this->pSpielerModel = new Model(model, false);
        this->pSpielerModel->shader(this->pShader);

        // Spieler verschiebung
        Matrix spawnPunkt;

        /* DEBUG Spawns */
        // spawnPunkt.translation(10, 2, 0);
        // spawnPunkt.translation(-1, 3, 0);
        // spawnPunkt.translation(-12 - 50, 1, 0);

         spawnPunkt.translation(-40, 2, 0);

        this->pSpielerModel->transform(spawnPunkt);

        return true;
    } else {
        std::cout << "Spieler::loadModels() Fehler beim laden des Models" << std::endl;
        return false;
    }
}

/**
 *
 * @param forwardBackward geschwindigkeit die der Spieler laeuft
 */
void Spieler::move(float forwardBackward) {
    this->spielerBewegung.X = forwardBackward;
}

/**
 * In der Update Logik befindet sich die Spring, Fall und Collisionslogik
 *
 * @param dtime fuer die Matrix transformationen
 * @param spielWelt fuer Collision Detection - for each mit jedem Element aus der Liste
 * @param letzteSpielerPosition speichert die letzt Spielerposition fuer den naechsten update Aufruf
 * @param cam vom Hauptprogramm
 *
 * Quelle / Ansatz der Implementation: <a href="https://www.youtube.com/watch?v=isDMUZg0EaQ"> Ansatz zur Jump Logik</a>
 *
 */
void Spieler::update(float dtime, const std::vector<BaseModel *> &spielWelt, Matrix &letzteSpielerPosition, Camera &cam) {

    // Aktuelle Position des Spielers als Vektor
    Vector spielerPosition(this->pSpielerModel->transform().translation());
    float sprungkraft = 1.0;
    static float jump_timer = 0.f;

    //======================= PRESS LOGIC / JUMP LOGIC =============================

    if (this->isJumping() && !this->isFalling()) {
        // Wenn Spieler springt und nicht am fallen ist, jumptimer hochzaehlen
        // Jumptimer = Space gedrueckt halten = hoeherer Jump
        jump_timer += dtime;

        //Springen solange der Jumptimer noch nicht 1.f erreicht
        if (jump_timer < 1.f) {
            this->spielerMatrix.translation(0,  sprungkraft * dtime , 0);
            this->spielerMatrix = pSpielerModel->transform() * spielerMatrix;
            this->pSpielerModel->transform(spielerMatrix);
        } else {
            this->setFalling(true);
            jump_timer = 0.f;
        }
    }

    //======================= RELEASE LOGIC =======================================

    // Hilfsvariablen fuer den Fall
    static float fall_timer = 0.f;
    static float last_fall_velocity = 0.f;

    // Wird ausgefuehrt sobald FALLING auf true gesetzt wird.
    if (this->isFalling()) {
        static float gravity = 4.1f;
        fall_timer += dtime;
        // Je laenger man faellt, desto schneller faellt man runter
        spielerPosition.Y -= gravity * fall_timer;
        spielerMatrix.translation(0, spielerPosition.Y * dtime, 0);
        spielerMatrix = pSpielerModel->transform() * spielerMatrix;
        pSpielerModel->transform(spielerMatrix);
        last_fall_velocity = gravity * fall_timer;
    } else {
        fall_timer = 0.f;
    }

    //======================= BEWEGUNG LOGIC =======================================

    if (this->isMovingLeft() || this->isMovingRight() || this->isJumping() || this->isFalling()) {
        // Hilfsvariable falls wir eine Kollsion haben
        bool aufBoden = false;

        // AUF BODEN LAUFEN LOGIC
        if (!this->isFalling() && !this->isJumping() && (this->isMovingRight() || this->isMovingLeft())) {

            // Kollisionscheck mit Bounding Box des Spielers und Objekt was beruehrt wurde
            for (const auto &o: spielWelt) {
                if (Model::checkCollision(this->bbVomSpieler, o->boundingBox().transform(o->transform()))) {
                    aufBoden = true;
                }
            }

            // Wenn der Spieler nach den Kollisionscheck nicht auf den Boden ist und nicht springt, muss er fallen
            if (!aufBoden && !this->isJumping()) {
                this->setFalling(true);
            }
        }

        // Erneut Kollision checken mit Bounding Box vom Spieler und Objekte der Spielwelt
        for (const auto &o: spielWelt) {
            if (Model::checkCollision(this->bbVomSpieler, o->boundingBox().transform(o->transform()))) {

                // Wenn Kollision mit etwas gefaehrlichem dann stirbt der Spieler
                if(o->isGefahr()){
                    Application::amLeben = false;
                    this->deathScreen(cam);
                }

                // Wenn Ziel beruehrt dann springe zum victoryScreen
                if(o->isZiel()){
                    Application::amLeben = false;
                    this->victoryScreen(cam);
                }

                // Wenn der Spieler eine Kollision hat mit den "Fuessen" dann verschiebe den Spieler um die Schnittflaeche der Bounding Boxen
                // des Spielers und des Objekts nach oben
                if (this->isFalling()) {
                    std::cout << "Y KOLLISION" << std::endl;

                    this->doCollisionCorrectionY(o);
                    this->pSpielerModel->transform(spielerMatrix);
                    this->setFalling(false);
                    jump_timer = 0;

                }


                // Wenn der Spieler eine Kollision hat mit der "Nase/Hinterkopf" dann verschiebe den Spieler um die Schnittflaeche der Bounding Boxen
                // des Spielers und des Objekts nach links/rechts
                if (this->isMovingLeft() || this->isMovingRight()) {


                    // Auf Min Y von Spieler
                    float errorMargin = 0.1f;

                    // gucken welche Achse ueberlappung
                    if((this->getSpielerModel()->boundingBox().transform(this->getSpielerModel()->transform()).Min.Y + errorMargin) <= (o->boundingBox().transform(o->transform()).Min.Y) && ((o->boundingBox().transform(o->transform()).Min.Y <= this->getSpielerModel()->boundingBox().transform(this->getSpielerModel()->transform()).Max.Y))){
                        std::cout << "Bedingung 1"<< std::endl;
                        this->doCollisionCorrectionX(o);
                    }

                    // gucken welche Achse ueberlappung
                    else if((o->boundingBox().transform(o->transform()).Min.Y <= (this->getSpielerModel()->boundingBox().transform(this->getSpielerModel()->transform()).Min.Y + errorMargin)) && (this->getSpielerModel()->boundingBox().transform(this->getSpielerModel()->transform()).Max.Y <= (o->boundingBox().transform(o->transform()).Max.Y))){
                        std::cout << "Bedingung 2"<< std::endl;
                        this->doCollisionCorrectionX(o);
                    }

                    // gucken welche Achse ueberlappung
                    else if(this->getSpielerModel()->boundingBox().transform(this->getSpielerModel()->transform()).Min.Y + errorMargin <= (o->boundingBox().transform(o->transform()).Max.Y) && o->boundingBox().transform(o->transform()).Max.Y <= (this->getSpielerModel()->boundingBox().transform(this->getSpielerModel()->transform()).Max.Y)){
                        std::cout << "Bedingung 3"<< std::endl;
                        this->doCollisionCorrectionX( o);
                    }

                    // gucken welche Achse ueberlappung
                    else if((this->getSpielerModel()->boundingBox().transform(this->getSpielerModel()->transform()).Min.Y + errorMargin <= (this->getSpielerModel()->boundingBox().transform(this->getSpielerModel()->transform()).Min.Y)) && (this->getSpielerModel()->boundingBox().transform(this->getSpielerModel()->transform()).Max.Y <= (this->getSpielerModel()->boundingBox().transform(this->getSpielerModel()->transform()).Max.Y))){
                        std::cout << "Bedingung 4"<< std::endl;
                        this->doCollisionCorrectionX( o);
                    }


                    // Die Spielerposition um den Korrekturwert verschieben
                    this->pSpielerModel->transform(spielerMatrix);
                }
            }
        }


        // Spieler Bewegung durch Spielermatrix und Veraenderungsmatrix berechnen und Spielermodell uebergeben
        this->spielerMatrix.translation(this->spielerBewegung.X * dtime, 0, 0);
        this->spielerMatrix = this->pSpielerModel->transform() * this->spielerMatrix;
        this->pSpielerModel->transform(this->spielerMatrix);

        // Boundig Box des Spielers Updaten
        this->bbVomSpieler = this->getSpielerModel()->boundingBox().transform(this->getSpielerModel()->transform());

    }
}



/**
 * Kollisionsberechnung von AABB mit Objekt und Spieler
 *
 * Falls eine Kollision auf der "rechten Seite" dann verschiebe den Spieler nach links
 * Falls eine Kollision auf der "linken Seite" dann verschiebe den Spieler nach rechts
 *
 * Dabei wird nicht die letzte Spielerpostion beruecktsichtigt sondern die Schnittflaeche der AABB´s (Laenge der X Achse)
 * Danke Prof. Lensing :)
 *
 * @param o Objekt in der Spielwelt
 */
void Spieler::doCollisionCorrectionX(const BaseModel *o){
    Vector tmp = this->spielerMatrix.translation();
    float diffX = 0;
    if(this->isMovingLeft()){
        diffX = o->boundingBox().transform(o->transform()).Max.X -  this->getSpielerModel()->boundingBox().transform(this->getSpielerModel()->transform()).Min.X;
        this->spielerMatrix.translation(tmp.X + diffX + 0.035, tmp.Y, tmp.Z);
    } else {
        diffX = this->getSpielerModel()->boundingBox().transform(this->getSpielerModel()->transform()).Max.X - o->boundingBox().transform(o->transform()).Min.X;
        this->spielerMatrix.translation(tmp.X - diffX - 0.035, tmp.Y, tmp.Z);
    }
}

/**
 * Kollisionsberechnung von AABB mit Objekt und Spieler
 *
 * Falls eine Kollision auf den "Fuessen" dann verschiebe den Spieler nach oben
 *
 * Dabei wird nicht die letzte Spielerpostion beruecktsichtigt sondern die Schnittflaeche der AABB´s (Laenge der Y Achse)
 * Danke Prof. Lensing :)
 *
 * @param o Objekt in der Spielwelt
 */
void Spieler::doCollisionCorrectionY(const BaseModel *o){
    Vector tmp = this->spielerMatrix.translation();
    float diffY = 0;

    diffY = o->boundingBox().transform(o->transform()).Max.Y -  this->getSpielerModel()->boundingBox().transform(this->getSpielerModel()->transform()).Min.Y;
    this->spielerMatrix.translation(tmp.X, tmp.Y + diffY , tmp.Z);
}


/**
 * Verschiebe die Kamera in den "Todesscreen"
 *
 * @param cam die verschoben wird.
 */
void Spieler::deathScreen(Camera &cam) {
    Vector tmp(200,0,0);

    cam.setTarget(tmp);
    tmp.Z += 4.5f; // Kamera offset
    cam.setPosition(tmp);
}


/**
 * Verschiebe die Kamera in den "Victory"
 *
 * @param cam die verschoben wird.
 */
void Spieler::victoryScreen(Camera &cam) {
    Vector tmp(250,0,0);

    cam.setTarget(tmp);
    tmp.Z += 4.5f; // Kamera offset
    cam.setPosition(tmp);
}

// zeichne Modell
void Spieler::draw(const BaseCamera &cam) {
    this->pSpielerModel->draw(cam);
}

// Setter Getter
bool Spieler::isJumping() const {
    return jumping;
}

void Spieler::setJumping(bool jumping) {
    Spieler::jumping = jumping;
}

bool Spieler::isFalling() const {
    return falling;
}

void Spieler::setFalling(bool falling) {
    Spieler::falling = falling;
}

bool Spieler::isMovingRight() const {
    return movingRight;
}

void Spieler::setMovingRight(bool movingRight) {
    Spieler::movingRight = movingRight;
}

bool Spieler::isMovingLeft() const {
    return movingLeft;
}

void Spieler::setMovingLeft(bool movingLeft) {
    Spieler::movingLeft = movingLeft;
}

Model *Spieler::getSpielerModel() const {
    return pSpielerModel;
}

const Matrix &Spieler::getSpielerMatrix() const {
    return spielerMatrix;
}





