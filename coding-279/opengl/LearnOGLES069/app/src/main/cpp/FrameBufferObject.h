#pragma once

#include "AlicePrefix.h"
class FrameBufferObject{
public:
    GLint mFrameBufferObjectBeforeSwitch;
    GLuint mFrameBufferObject;
    GLuint mColorBuffer0,mColorBuffer1,mColorBuffer2,mColorBuffer3;
    int mResolutionWidth,mResolutionHeight;
public:
    FrameBufferObject();
    void Init(int width,int height);
    void BeginDraw();
    void EndDraw();
};