#include "Sence.h"
#include "Utils.h"
#include "Geometry.h"
#include "Shader.h"
#include "Material.h"
static AAssetManager *sAssetManager= nullptr;
glm::mat4 modelMatrix,viewMatrix,projectionMatrix;
Geometry geometry;
Shader shader;
Material*material;
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
void InitYUVTextures(){
    int filesize=0;
    unsigned char * filecontent=LoadFileContent("test.yuv",filesize);
    unsigned char * YData=filecontent;
    int YDataSize=1280*720;
    unsigned char * UData=filecontent+YDataSize;
    int UDataSize=640*360;
    unsigned char * VData=filecontent+(YDataSize+UDataSize);
    GLuint textureYUV=CreateTexture2D(nullptr,1280,720+360,GL_ALPHA,GL_ALPHA);
    //ffmpeg -> yuv
    SubmitTexture2D(textureYUV,YData,0,0,1280,720,GL_ALPHA,GL_UNSIGNED_BYTE);
    SubmitTexture2D(textureYUV,UData,0,720,640,360,GL_ALPHA,GL_UNSIGNED_BYTE);
    SubmitTexture2D(textureYUV,VData,640,720,640,360,GL_ALPHA,GL_UNSIGNED_BYTE);
    delete [] filecontent;

    material->SetTexture("U_TextureYUV",textureYUV);
}
extern "C" JNIEXPORT void JNICALL Java_cn_battlefire_learnogles_MainActivity_Init(
        JNIEnv*env,
        jobject MainActivity,jobject am
){
    sAssetManager=AAssetManager_fromJava(env,am);
    __android_log_print(ANDROID_LOG_INFO,ALICE_LOG_TAG,"Init");
    glClearColor(0.6f,0.4f,0.1f,1.0f);
    Vertice vertices[4];//cpu -> gpu
    float scale=5.0f;
    float texcoord_t=720.0f/(720.0f+360.0f);
    geometry.SetVertexCount(4);
    geometry.SetIndexCount(6);
    geometry.SetPosition(0,-128.0f*scale,-72.0f*scale,0.0f);
    geometry.SetTexcoord(0,0.0f,0.0f,0.0f,0.0f);
    geometry.SetTexcoord1(0,0.0F,texcoord_t,0.5f,texcoord_t);

    geometry.SetPosition(1,128.0f*scale,-72.0f*scale,0.0f);
    geometry.SetTexcoord(1,1.0f,0.0f,0.0f,0.0f);
    geometry.SetTexcoord1(1,0.5f,texcoord_t,1.0f,texcoord_t);

    geometry.SetPosition(2,-128.0f*scale,72.0f*scale,0.0f);
    geometry.SetTexcoord(2,0.0f,texcoord_t,0.0f,0.0f);
    geometry.SetTexcoord1(2,0.0f,1.0f,0.5f,1.0f);

    geometry.SetPosition(3,128.0f*scale,72.0f*scale,0.0f);
    geometry.SetTexcoord(3,1.0f,texcoord_t,0.0f,0.0f);
    geometry.SetTexcoord1(3,0.5f,1.0f,1.0f,1.0f);
    geometry.SetIndex(0,0);
    geometry.SetIndex(1,1);
    geometry.SetIndex(2,2);
    geometry.SetIndex(3,1);
    geometry.SetIndex(4,3);
    geometry.SetIndex(5,2);

    geometry.Submit();

    modelMatrix=glm::translate(0.0f,0.0f,-1.0f);

    geometry.mPrimitiveType=GL_TRIANGLES;
    shader.InitStandardShader("yuv420s.vs","yuv420s.fs");
    material=new Material;
    material->mShader=&shader;

    InitYUVTextures();
}
extern "C" JNIEXPORT void JNICALL Java_cn_battlefire_learnogles_MainActivity_OnViewportChanged(
        JNIEnv*env,
        jobject MainActivity,jint width,jint height
){
    __android_log_print(ANDROID_LOG_INFO,ALICE_LOG_TAG,"OnViewportChanged %dx%d",width,height);
    glViewport(0,0,width,height);
    viewMatrix=glm::lookAt(glm::vec3(0.0f,0.0f,0.0f),
            glm::vec3(0.0,0.0,-1.0f),
            glm::vec3(0.0,1.0f,0.0f));
    float half_width=float(width)/2.0f;
    float half_height=float(height)/2.0f;
    projectionMatrix=glm::ortho(-half_width,half_width,-half_height,half_height,0.1f,100.0f);
}
extern "C" JNIEXPORT void JNICALL Java_cn_battlefire_learnogles_MainActivity_Render(
        JNIEnv*env,
        jobject MainActivity
){
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    geometry.BeginDraw();
    material->BeginDraw(modelMatrix,viewMatrix,projectionMatrix);
    geometry.Draw();
    geometry.EndDraw();
    material->EndDraw();
}
