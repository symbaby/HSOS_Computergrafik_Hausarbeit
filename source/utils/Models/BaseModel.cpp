//
//  BaseModel.cpp
//  ogl4
//
//  Created by Philipp Lensing on 19.09.16.
//  Copyright © 2016 Philipp Lensing. All rights reserved.
//

#include "utils/Models/BaseModel.h"

BaseModel::BaseModel() : pShader(NULL), DeleteShader(false), ShadowCaster(true)
{
    Transform.identity();
}

BaseModel::~BaseModel()
{
    if(DeleteShader && pShader)
        delete pShader;
    DeleteShader = false;
    pShader = NULL;
}

void BaseModel::shader( BaseShader* shader, bool deleteOnDestruction )
{
    pShader = shader;
    DeleteShader = deleteOnDestruction;
}

void BaseModel::draw(const BaseCamera& Cam)
{
    if(!pShader) {
        std::cout << "BaseModel::draw() no Shader found" << std::endl;
        return;
    }
    
    pShader->modelTransform(transform());
    pShader->activate(Cam);
    
}

bool BaseModel::isGefahr() const {
    return gefahr;
}

void BaseModel::setGefahr(bool gefahr) {
    BaseModel::gefahr = gefahr;
}

bool BaseModel::isZiel() const {
    return ziel;
}

void BaseModel::setZiel(bool ziel) {
    BaseModel::ziel = ziel;
}

