#pragma once

#include "AlicePrefix.h"
struct Vertice{
    float mPosition[4];//x,y,z,w
};
unsigned char * LoadFileContent(const char *path,int&filesize);
GLuint CompileShader(GLenum shader_type,const char * source_code);
float GetFrameTime();