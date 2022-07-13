//
// Created by Heck on 2020/11/6.
//

#include "SceneNode.h"
#include "Camera.h"
#include "Geometry.h"
#include "Material.h"
#include "Shader.h"

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
    mGeometry->BeginDraw();
    mMaterial->BeginDraw(camera,mModelMatrix);
    mGeometry->Draw();
    mGeometry->EndDraw();
    mMaterial->EndDraw();
    if(mNext!= nullptr){
        Next<SceneNode>()->Render(camera);
    }
}