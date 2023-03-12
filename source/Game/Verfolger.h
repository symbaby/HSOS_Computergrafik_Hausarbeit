//
// Created by boery on 19.08.2022.
//

#ifndef CGPRAKT6_CLION_VERFOLGER_H
#define CGPRAKT6_CLION_VERFOLGER_H


#include "utils/Models/Model.h"

class Verfolger : public Model {
public:

    Verfolger();

    virtual ~Verfolger();

    bool loadModels(const char *model);

    void move(float forwardBackward);

    void update(float dtime, Camera &cam);


    virtual void draw(const BaseCamera &cam);

    Model *getVerfolgerModel() const;

    AABB bbVerfolger;


protected:
    float forwardBackward;
    Model *pVerfolgerModel = nullptr;
    Vector verfolgerBewegung;
    Matrix verfolgerMatrix;




};


#endif //CGPRAKT6_CLION_VERFOLGER_H
