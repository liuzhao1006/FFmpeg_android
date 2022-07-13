#pragma once

#include "AlicePrefix.h"
struct Vertice{
    float mPosition[4];//x,y,z,w
    float mTexcoord[4];//u,v
    float mTexcoord1[4];//Uu,Uv,Vu,Vv
    float mNormal[4];
};
unsigned char * LoadFileContent(const char *path,int&filesize);
GLuint CompileShader(GLenum shader_type,const char * source_code);
GLuint CreateProgram(GLuint vertex_shader,GLuint fragment_shader);
GLuint CreateStandardProgram(const char *vertex_shader_path,const char * fragment_shader_path);
GLuint CreateBufferObject(GLenum type,void * data,int size,GLenum usage);
GLuint CreateTexture2D(void*pixel,int width,int height,GLenum gpu_format,GLenum cpu_format);
void SubmitTexture2D(GLuint texture,void*pixel,int x,int y,int width,int height,GLenum cpu_format,GLenum dataType);
unsigned char * DecodeBMP(unsigned char *bmp_file_content,int&width,int&height);
GLuint CreateTextureFromFile(const char *path);
void UpdateBufferObject(GLuint object,GLenum type,void * data,int size,int offset=0);
float GetFrameTime();