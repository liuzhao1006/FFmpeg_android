//
// Created by Heck on 2020/11/10.
//

#include "ParticleSystem.h"
void ParticleSystem::Init() {
    SetVertexCount(1);
    SetPosition(0,0.0f,0.0f,-2.0f);
    Submit();
    mPrimitiveType=GL_POINTS;
}
