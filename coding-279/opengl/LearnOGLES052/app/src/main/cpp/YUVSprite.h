#pragma once

#include "AlicePrefix.h"
class Geometry;
class YUVSprite : public Geometry{
public:
    void Init();
    static GLuint InitYUVTexture();
};