//
// Created by Heck on 2020/11/6.
//

#include "Material.h"
#include "Shader.h"
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
        if(tp->mTexture!=0&&tp->mTexture!=texture){
            glDeleteTextures(1,&tp->mTexture);
        }
        tp->mTexture=texture;
    }
}
void Material::BeginDraw(Camera*camera,glm::mat4 &m) {
    mShader->BeginDraw(camera,m);
    int texture_unit_index=0;
    for (auto iter = mTextures.begin(); iter!=mTextures.end() ; ++iter) {
        glActiveTexture(GL_TEXTURE0+texture_unit_index);
        glBindTexture(GL_TEXTURE_2D,iter->second->mTexture);
        glUniform1i(iter->second->mLocation,texture_unit_index++);
    }
}
void Material::EndDraw() {
    mShader->EndDraw();
}