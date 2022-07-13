#include <jni.h>
#include <string>

extern "C" JNIEXPORT jstring JNICALL
Java_cn_battlefire_learnogles_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}
extern "C" JNIEXPORT jint JNICALL Java_cn_battlefire_learnogles_MainActivity_Test(
        JNIEnv*env,
        jobject MainActivity
        ){
    return 100;
}