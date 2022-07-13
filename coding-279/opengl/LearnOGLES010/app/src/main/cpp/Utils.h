#pragma once

#include "AlicePrefix.h"
struct Vertice{
    float mPosition[4];//x,y,z,w
};
unsigned char * LoadFileContent(const char *path,int&filesize);
float GetFrameTime();