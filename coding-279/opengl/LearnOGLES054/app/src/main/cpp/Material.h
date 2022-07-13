#pragma once

#include "AlicePrefix.h"
class Shader;
class Camera;
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
    void BeginDraw(Camera*camera,glm::mat4&m);
    void EndDraw();
};
