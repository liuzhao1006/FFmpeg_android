#pragma once

#include "AlicePrefix.h"
struct Vertice{
    float mPosition[4];//x,y,z,w
    float mColor[4];//r,g,b,a
};
unsigned char * LoadFileContent(const char *path,int&filesize);
GLuint CompileShader(GLenum shader_type,const char * source_code);
GLuint CreateProgram(GLuint vertex_shader,GLuint fragment_shader);
GLuint CreateStandardProgram(const char *vertex_shader_path,const char * fragment_shader_path);
float GetFrameTime();