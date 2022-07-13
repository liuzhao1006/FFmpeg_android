#pragma once

#include "AlicePrefix.h"
class Shader;
struct TextureProperty{
    GLint mLocation;
    GLuint mTexture;
    TextureProperty(){
        mTexture=0;
    }
};
class Material{
public:
    Shader *mShader;
    std::unordered_map<std::string,TextureProperty*> mTextures;
    void SetTexture(const char * property_name,GLuint texture);
    void BeginDraw(glm::mat4&m,glm::mat4&v,glm::mat4&p);
    void EndDraw();
};
