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
struct Vec4Property{
    GLint mLocation;
    float v[4];
    Vec4Property(){
        memset(v,0,sizeof(float)*4);
    }
};
class Material{
public:
    Shader *mShader;
    std::unordered_map<std::string,TextureProperty*> mTextures;
    std::unordered_map<std::string,Vec4Property*> mVec4s;
    GLenum mBlendSrcFunc,mBlendDstFunc;
    bool mbEnableBlend;
    bool mbEnableDepthTest;
    Material();
    void SetTexture(const char * property_name,GLuint texture);
    void SetVec4(const char * property_name,float*v);
    void BeginDraw(Camera*camera,glm::mat4&m);
    void EndDraw();
};
