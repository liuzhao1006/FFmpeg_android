#include "Sence.h"
#include "Utils.h"
#include "Geometry.h"
#include "Shader.h"
#include "Material.h"
#include "Camera.h"
#include "YUVSprite.h"
#include "SceneNode.h"
#include "FrameBufferObject.h"
static AAssetManager *sAssetManager= nullptr;
static SceneNode*sRootNode= nullptr,*sDefaultFBORootNode= nullptr;
Camera camera;
Shader *ui_shader;
Material*ui_material;
Geometry *quad;
FrameBufferObject fbo;
unsigned char * LoadFileContent(const char *path,int&filesize){
    unsigned char *filecontent=nullptr;
    filesize=0;
    AAsset*asset=AAssetManager_open(sAssetManager,path,AASSET_MODE_UNKNOWN);
    if(asset!= nullptr){
        filesize=AAsset_getLength(asset);
        filecontent=new unsigned char[filesize+1];
        AAsset_read(asset,filecontent,filesize);
        filecontent[filesize]=0;
        AAsset_close(asset);
    }
    return filecontent;
}
void AddSceneNode(SceneNode*node){
    if(sRootNode== nullptr){
        sRootNode=node;
    }else{
        sRootNode->Append(node);
    }
}
void InitDefaultFBONodes(){
    ui_shader=new Shader;
    ui_shader->InitStandardShader("ui.vs","ui.fs");

    ui_material=new Material;
    ui_material->mShader=ui_shader;

    quad=new Geometry;
    quad->SetVertexCount(4);
    quad->mPrimitiveType=GL_TRIANGLE_STRIP;
    quad->SetPosition(0,-128.0f,-128.0f,0.0f);
    quad->SetTexcoord(0,0.0f,0.0f,0.0f);
    quad->SetPosition(1,128.0f,-128.0f,0.0f);
    quad->SetTexcoord(1,1.0f,0.0f,0.0f);
    quad->SetPosition(2,-128.0f,128.0f,0.0f);
    quad->SetTexcoord(2,0.0f,1.0f,0.0f);
    quad->SetPosition(3,128.0f,128.0f,0.0f);
    quad->SetTexcoord(3,1.0f,1.0f,0.0f);
    quad->Submit();

    sDefaultFBORootNode=new SceneNode;
    sDefaultFBORootNode->mMaterial=ui_material;
    sDefaultFBORootNode->mGeometry=quad;
    sDefaultFBORootNode->mModelMatrix=glm::translate(0.0f,0.0f,-1.0f);
}
void InitFBONodes(){
    SceneNode*node=new SceneNode;
    node->mMaterial=ui_material;
    node->mGeometry=quad;
    node->mModelMatrix=glm::translate(0.0f,0.0f,-1.0f);
    ui_material->SetTexture("U_Texture",CreateTextureFromFile("jason.jpg"));
    AddSceneNode(node);
}
extern "C" JNIEXPORT void JNICALL Java_cn_battlefire_learnogles_MainActivity_Init(
        JNIEnv*env,
        jobject MainActivity,jobject am
){
    sAssetManager=AAssetManager_fromJava(env,am);
    __android_log_print(ANDROID_LOG_INFO,ALICE_LOG_TAG,"GL Version[%s]",glGetString(GL_VERSION));
    __android_log_print(ANDROID_LOG_INFO,ALICE_LOG_TAG,"GLSL Version[%s]",glGetString(GL_SHADING_LANGUAGE_VERSION));
    glClearColor(0.6f,0.4f,0.1f,1.0f);
    InitDefaultFBONodes();
    InitFBONodes();
}
extern "C" JNIEXPORT void JNICALL Java_cn_battlefire_learnogles_MainActivity_OnViewportChanged(
        JNIEnv*env,
        jobject MainActivity,jint width,jint height
){
    __android_log_print(ANDROID_LOG_INFO,ALICE_LOG_TAG,"OnViewportChanged %dx%d",width,height);
    glViewport(0,0,width,height);
    float half_width=float(width)/2.0f;
    float half_height=float(height)/2.0f;
    camera.mViewMatrix=glm::lookAt(glm::vec3(0.0f,0.0f,0.0f),
            glm::vec3(0.0,0.0,-1.0f),
            glm::vec3(0.0,1.0f,0.0f));
    camera.mProjectionMatrix=glm::ortho(-half_width,half_width,-half_height,half_height,0.1f,100.0f);
    fbo.Init(width,height);
    fbo.BeginDraw();
    if(sRootNode!=nullptr){
        sRootNode->Update(GetFrameTime());
        sRootNode->Render(&camera);
    }
    fbo.EndDraw();
    ui_material->SetTexture("U_Texture",fbo.mColorBuffer);
}
extern "C" JNIEXPORT void JNICALL Java_cn_battlefire_learnogles_MainActivity_Render(
        JNIEnv*env,
        jobject MainActivity
){
    glClearColor(0.1f,0.4f,0.6f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
    if(sDefaultFBORootNode!= nullptr){
        sDefaultFBORootNode->Update(GetFrameTime());
        sDefaultFBORootNode->Render(&camera);
    }
}
