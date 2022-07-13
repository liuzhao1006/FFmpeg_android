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
