//
// Created by Heck on 2020/11/10.
//

#include "ParticleSystem.h"
#include "Utils.h"
void ParticleSystem::Init() {
    SetVertexCount(1000);
    for (int i = 0; i < mVertexCount; ++i) {
        SetPosition(i,srandf(),srandf(),-2.0f+srandf());
        SetTexcoord(i,randf(),randf(),randf());
    }
    Submit();
    mPrimitiveType=GL_POINTS;
}
void ParticleSystem::Update(float delta) {
    for (int i = 0; i < mVertexCount; ++i) {
        float x=mVertices[i].mPosition[0];
        float y=mVertices[i].mPosition[1];
        float z=mVertices[i].mPosition[2];
        SetPosition(i,x+srandf()*delta,y+srandf()*delta,z+srandf()*delta);
    }
    Submit();
}