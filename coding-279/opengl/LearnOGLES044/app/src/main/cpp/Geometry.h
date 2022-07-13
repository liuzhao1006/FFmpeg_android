#pragma once
#include "AlicePrefix.h"

class Geometry{
public:
    GLuint mVBO;
    GLuint mIBO;
    int mVertexCount;
    int mIndexCount;
    bool mbDrawWidthIBO;
    GLenum mPrimitiveType;
    Geometry();
    virtual void BeginDraw();
    virtual void Draw();
    virtual void EndDraw();
};
