#pragma once

#include "AlicePrefix.h"

class Shader{
public:
    GLuint mProgram;
    GLint mPositionLocation,mTexCoordLocation,mTexCoord1Location,mNormalLocation;
    GLint mModelMatrixLocation,mViewMatrixLocation,mProjectionMatrixLocation;
    void InitStandardShader(const char * vertex_shader_path,const char * fragment_shader_path);
    void BeginDraw(glm::mat4&m,glm::mat4&v,glm::mat4&p);
    void EndDraw();
};