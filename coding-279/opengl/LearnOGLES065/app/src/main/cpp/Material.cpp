//
// Created by Heck on 2020/11/6.
//

#include "Material.h"
#include "Shader.h"
Material::Material() {
    mbEnableBlend=false;
    mbEnableDepthTest=true;
    mBlendSrcFunc=GL_SRC_ALPHA;
    mBlendDstFunc=GL_ONE_MINUS_SRC_ALPHA;
}
void Material::SetTexture(const char *property_name, GLuint texture) {
    auto iter=mTextures.find(property_name);
    TextureProperty*tp= nullptr;
    if(iter==mTextures.end()){
        GLint location=glGetUniformLocation(mShader->mProgram,property_name);
        if(location!=-1){
            tp=new TextureProperty;
            tp->mLocation=location;
            tp->mTexture=texture;
            mTextures.insert(std::pair<std::string,TextureProperty*>(property_name,tp));
        }
    }else{
        tp=iter->second;
        tp->mTexture=texture;
    }
}
void Material::SetVec4(const char *property_name, float *v){
    auto iter=mVec4s.find(property_name);
    Vec4Property*vp= nullptr;
    if(iter==mVec4s.end()){
        GLint location=glGetUniformLocation(mShader->mProgram,property_name);
        if(location!=-1){
            vp=new Vec4Property;
            vp->mLocation=location;
            vp->v[0]=v[0];
            vp->v[1]=v[1];
            vp->v[2]=v[2];
            vp->v[3]=v[3];
            mVec4s.insert(std::pair<std::string,Vec4Property*>(property_name,vp));
        }
    }else{
        vp=iter->second;
        vp->v[0]=v[0];
        vp->v[1]=v[1];
        vp->v[2]=v[2];
        vp->v[3]=v[3];
    }
}
void Material::BeginDraw(Camera*camera,glm::mat4 &m) {
    if(mbEnableDepthTest){
        glEnable(GL_DEPTH_TEST);
    }else{
        glDisable(GL_DEPTH_TEST);
    }
    if(mbEnableBlend){
        glEnable(GL_BLEND);
        glBlendFunc(mBlendSrcFunc,mBlendDstFunc);
    }else{
        glDisable(GL_BLEND);
    }
    mShader->BeginDraw(camera,m);
    int texture_unit_index=0;
    for (auto iter = mTextures.begin(); iter!=mTextures.end() ; ++iter) {
        glActiveTexture(GL_TEXTURE0+texture_unit_index);
        glBindTexture(GL_TEXTURE_2D,iter->second->mTexture);
        glUniform1i(iter->second->mLocation,texture_unit_index++);
    }
    for (auto iter = mVec4s.begin(); iter!=mVec4s.end() ; ++iter) {
        Vec4Property*vp=iter->second;
        glUniform4fv(vp->mLocation,1,vp->v);
    }
}
void Material::EndDraw() {
    mShader->EndDraw();
}