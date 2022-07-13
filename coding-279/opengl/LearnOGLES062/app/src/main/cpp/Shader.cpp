//
// Created by Heck on 2020/11/6.
//

#include "Shader.h"
#include "Utils.h"
#include "Camera.h"
void Shader::InitStandardShader(const char *vertex_shader_path, const char *fragment_shader_path) {
    mProgram=CreateStandardProgram(vertex_shader_path,fragment_shader_path);
    mPositionLocation=glGetAttribLocation(mProgram,"position");
    mTexCoordLocation=glGetAttribLocation(mProgram,"texcoord");
    mTexCoord1Location=glGetAttribLocation(mProgram,"texcoord1");
    mNormalLocation=glGetAttribLocation(mProgram,"normal");
    mModelMatrixLocation=glGetUniformLocation(mProgram,"U_ModelMatrix");
    mViewMatrixLocation=glGetUniformLocation(mProgram,"U_ViewMatrix");
    mProjectionMatrixLocation=glGetUniformLocation(mProgram,"U_ProjectionMatrix");
}
void Shader::BeginDraw(Camera*camera,glm::mat4 &m) {
    glUseProgram(mProgram);
    glUniformMatrix4fv(mModelMatrixLocation,1,GL_FALSE,glm::value_ptr(m));
    glUniformMatrix4fv(mViewMatrixLocation,1,GL_FALSE,glm::value_ptr(camera->mViewMatrix));
    glUniformMatrix4fv(mProjectionMatrixLocation,1,GL_FALSE,glm::value_ptr(camera->mProjectionMatrix));
    //set attribute
    glEnableVertexAttribArray(mPositionLocation);
    glVertexAttribPointer(mPositionLocation,4,GL_FLOAT,GL_FALSE,sizeof(Vertice),0);
    glEnableVertexAttribArray(mTexCoordLocation);
    glVertexAttribPointer(mTexCoordLocation,4,GL_FLOAT,GL_FALSE,sizeof(Vertice),(void*)(sizeof(float)*4));
    glEnableVertexAttribArray(mTexCoord1Location);
    glVertexAttribPointer(mTexCoord1Location,4,GL_FLOAT,GL_FALSE,sizeof(Vertice),(void*)(sizeof(float)*8));
    glEnableVertexAttribArray(mNormalLocation);
    glVertexAttribPointer(mNormalLocation,4,GL_FLOAT,GL_FALSE,sizeof(Vertice),(void*)(sizeof(float)*12));
}
void Shader::EndDraw() {
    glUseProgram(0);
}