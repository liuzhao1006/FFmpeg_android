#include "Sence.h"
#include "Utils.h"
#include "Geometry.h"
#include "Shader.h"
#include "Material.h"
#include "Camera.h"
#include "YUVSprite.h"
#include "SceneNode.h"
#include "FrameBufferObject.h"
#include "Ground.h"
#include "ParticleSystem.h"
static AAssetManager *sAssetManager= nullptr;
static SceneNode*sRootNode= nullptr,*sDefaultFBORootNode= nullptr;
static int sViewportWidth=0,sViewportHeight=0;
Camera camera;
Shader *ground_shader,*particle_shader;
Material*ground_material,*particle_material;
Ground*ground;
ParticleSystem*particles;
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
void InitScene(){
    /*ground_shader=new Shader;
    ground_shader->InitStandardShader("Ground.vs","Ground.fs");

    ground_material=new Material;
    ground_material->mShader=ground_shader;
    GLuint texture=CreateTextureFromFile("jason.jpg");
    glBindTexture(GL_TEXTURE_2D,texture);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
    glBindTexture(GL_TEXTURE_2D,0);
    ground_material->SetTexture("U_Texture",texture);

    ground=new Ground;
    ground->Init();*/

    particle_shader=new Shader;
    particle_shader->InitStandardShader("particle.vs","particle.fs");

    particle_material=new Material;
    particle_material->mShader=particle_shader;
    particle_material->mbEnableBlend=true;
    particle_material->mbEnableDepthTest=false;
    particle_material->mBlendDstFunc=GL_ONE;
    particle_material->SetTexture("U_Texture",CreateProcedureTexture(64));

    particles=new ParticleSystem;
    particles->Init();

    SceneNode*node=new SceneNode;
    node->mMaterial=particle_material;
    node->mGeometry=particles;
    node->mModelMatrix=glm::translate(0.0f,0.0f,0.0f);
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
    InitScene();
}
extern "C" JNIEXPORT void JNICALL Java_cn_battlefire_learnogles_MainActivity_OnViewportChanged(
        JNIEnv*env,
        jobject MainActivity,jint width,jint height
){
    __android_log_print(ANDROID_LOG_INFO,ALICE_LOG_TAG,"OnViewportChanged %dx%d",width,height);
    sViewportWidth=width;
    sViewportHeight=height;
    glViewport(0,0,width,height);
    float half_width=float(width)/2.0f;
    float half_height=float(height)/2.0f;
    camera.mViewMatrix=glm::lookAt(glm::vec3(0.0f,0.0f,0.0f),
            glm::vec3(0.0,0.0,-1.0f),
            glm::vec3(0.0,1.0f,0.0f));
    camera.mProjectionMatrix=glm::perspective(45.0f,float(width)/float(height),0.1f,100.0f);
}
extern "C" JNIEXPORT void JNICALL Java_cn_battlefire_learnogles_MainActivity_Render(
        JNIEnv*env,
        jobject MainActivity
){
    glViewport(0,0,sViewportWidth,sViewportHeight);
    glClearColor(0.1f,0.4f,0.6f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
    if(sRootNode!= nullptr){
        sRootNode->Update(GetFrameTime());
        sRootNode->Render(&camera);
    }
}
