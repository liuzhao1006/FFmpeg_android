//
// Created by Heck on 2020/11/6.
//

#include "SceneNode.h"
#include "Camera.h"
#include "Geometry.h"
#include "Material.h"

void SceneNode::Update(float delta) {
    //update self
    if(mNext!= nullptr){
        Next<SceneNode>()->Update(delta);
    }
}
void SceneNode::Render(Camera *camera) {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    mGeometry->BeginDraw();
    mMaterial->BeginDraw(camera,mModelMatrix);
    mGeometry->Draw();
    mGeometry->EndDraw();
    mMaterial->EndDraw();
    if(mNext!= nullptr){
        Next<SceneNode>()->Render(camera);
    }
}