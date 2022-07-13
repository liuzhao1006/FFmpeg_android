//
// Created by Heck on 2020/11/6.
//

#include "SceneNode.h"
#include "Camera.h"
#include "Geometry.h"
#include "Material.h"
#include "Shader.h"
#include "Utils.h"

void SceneNode::Update(float delta) {
    //update self
    mGeometry->Update(delta);
    if(mNext!= nullptr){
        Next<SceneNode>()->Update(delta);
    }
}
void SceneNode::Render(Camera *camera) {
    if(mMaterial->mShader->mProgram==0){
        return ;
    }
    if(mGeometry->mVAO==0){
        glGenVertexArrays(1,&mGeometry->mVAO);
        glBindVertexArray(mGeometry->mVAO);//
        glBindBuffer(GL_ARRAY_BUFFER,mGeometry->mVBO);//
        //set attribute
        if(mMaterial->mShader->mPositionLocation!=-1){
            glEnableVertexAttribArray(mMaterial->mShader->mPositionLocation);
            glVertexAttribPointer(mMaterial->mShader->mPositionLocation,4,GL_FLOAT,GL_FALSE,sizeof(Vertice),0);
        }
        if(mMaterial->mShader->mTexCoordLocation!=-1){
            glEnableVertexAttribArray(mMaterial->mShader->mTexCoordLocation);
            glVertexAttribPointer(mMaterial->mShader->mTexCoordLocation,4,GL_FLOAT,GL_FALSE,sizeof(Vertice),(void*)(sizeof(float)*4));
        }
        if(mMaterial->mShader->mTexCoord1Location!=-1){
            glEnableVertexAttribArray(mMaterial->mShader->mTexCoord1Location);
            glVertexAttribPointer(mMaterial->mShader->mTexCoord1Location,4,GL_FLOAT,GL_FALSE,sizeof(Vertice),(void*)(sizeof(float)*8));
        }
        if(mMaterial->mShader->mNormalLocation!=-1){
            glEnableVertexAttribArray(mMaterial->mShader->mNormalLocation);
            glVertexAttribPointer(mMaterial->mShader->mNormalLocation,4,GL_FLOAT,GL_FALSE,sizeof(Vertice),(void*)(sizeof(float)*12));
        }
        glBindBuffer(GL_ARRAY_BUFFER,0);
        glBindVertexArray(0);
    }
    mGeometry->BeginDraw();
    mMaterial->BeginDraw(camera,mModelMatrix);
    mGeometry->Draw();
    mGeometry->EndDraw();
    mMaterial->EndDraw();
    if(mNext!= nullptr){
        Next<SceneNode>()->Render(camera);
    }
}