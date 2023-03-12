//
// Created by boery on 19.08.2022.
//

#ifndef CGPRAKT6_CLION_SPIELER_H
#define CGPRAKT6_CLION_SPIELER_H


#include "utils/Models/Model.h"

class Spieler : public Model {
public:
    Spieler();

    virtual ~Spieler();

    bool loadModels(const char *model);

    void move(float forwardBackward);

    void update(float dtime, Camera &cam);

    void update(float dtime, const std::vector<BaseModel*>& spielWelt, Matrix &letzteSpielerPosition, Camera &cam);

    virtual void draw(const BaseCamera &cam);

    bool isJumping() const;

    void setJumping(bool jumping);

    bool isFalling() const;

    void setFalling(bool falling);

    bool isMovingRight() const;

    void setMovingRight(bool movingRight);

    bool isMovingLeft() const;

    void setMovingLeft(bool movingLeft);

    const Matrix &getSpielerMatrix() const;




protected:
    float forwardBackward;
    Model *pSpielerModel;
    Vector spielerBewegung;
    Matrix spielerMatrix;
    Matrix letzteSpielerPosition;

public:
    Model *getSpielerModel() const;

    AABB bbVomSpieler;




private:
    bool onGround;
    bool jumping;
    bool falling;
    bool movingRight;
    bool movingLeft;

    void doCollisionCorrectionX(const BaseModel *o);
    void doCollisionCorrectionY(const BaseModel *o);

    void deathScreen(Camera &cam);

    void victoryScreen(Camera &cam);



};


#endif //CGPRAKT6_CLION_SPIELER_H
