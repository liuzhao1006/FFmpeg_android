#pragma once

#include "AlicePrefix.h"
class FrameBufferObject{
public:
    GLint mFrameBufferObjectBeforeSwitch;
    GLuint mFrameBufferObject;
    GLuint mColorBuffer;
public:
    FrameBufferObject();
    void Init(int width,int height);
    void BeginDraw();
    void EndDraw();
};