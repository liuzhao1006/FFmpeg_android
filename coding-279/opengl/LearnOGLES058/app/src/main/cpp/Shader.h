#pragma once

#include "AlicePrefix.h"
class Camera;
class Shader{
public:
    GLuint mProgram;
    GLint mPositionLocation,mTexCoordLocation,mTexCoord1Location,mNormalLocation;
    GLint mModelMatrixLocation,mViewMatrixLocation,mProjectionMatrixLocation;
    void InitStandardShader(const char * vertex_shader_path,const char * fragment_shader_path);
    void BeginDraw(Camera*camera,glm::mat4&m);
    void EndDraw();
};