#include "Sence.h"
extern "C" JNIEXPORT void JNICALL Java_cn_battlefire_learnogles_MainActivity_Init(
        JNIEnv*env,
        jobject MainActivity
){
    __android_log_print(ANDROID_LOG_INFO,ALICE_LOG_TAG,"Init");
    glClearColor(0.6f,0.4f,0.1f,1.0f);
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
    __android_log_print(ANDROID_LOG_INFO,ALICE_LOG_TAG,"Render");
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
}
