#include "Sence.h"
#include "Utils.h"
static AAssetManager *sAssetManager= nullptr;
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
extern "C" JNIEXPORT void JNICALL Java_cn_battlefire_learnogles_MainActivity_Init(
        JNIEnv*env,
        jobject MainActivity,jobject am
){
    sAssetManager=AAssetManager_fromJava(env,am);
    __android_log_print(ANDROID_LOG_INFO,ALICE_LOG_TAG,"Init");
    glClearColor(0.6f,0.4f,0.1f,1.0f);
    Vertice vertices[3];//cpu -> gpu
    vertices[0].mPosition[0]=-0.5f;//x
    vertices[0].mPosition[1]=-0.5f;//y
    vertices[0].mPosition[2]=-2.0f;//z
    vertices[0].mPosition[3]=1.0f;//w

    vertices[1].mPosition[0]=0.5f;//x
    vertices[1].mPosition[1]=-0.5f;//y
    vertices[1].mPosition[2]=-2.0f;//z
    vertices[1].mPosition[3]=1.0f;//w

    vertices[2].mPosition[0]=0.0f;//x
    vertices[2].mPosition[1]=0.5f;//y
    vertices[2].mPosition[2]=-2.0f;//z
    vertices[2].mPosition[3]=1.0f;//w
}
extern "C" JNIEXPORT void JNICALL Java_cn_battlefire_learnogles_MainActivity_OnViewportChanged(
        JNIEnv*env,
        jobject MainActivity,jint width,jint height
){
    __android_log_print(ANDROID_LOG_INFO,ALICE_LOG_TAG,"OnViewportChanged %dx%d",width,height);
    glViewport(0,0,width,height);
}
extern "C" JNIEXPORT void JNICALL Java_cn_battlefire_learnogles_MainActivity_Render(
        JNIEnv*env,
        jobject MainActivity
){
    __android_log_print(ANDROID_LOG_INFO,ALICE_LOG_TAG,"Render %f",GetFrameTime());
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
}
