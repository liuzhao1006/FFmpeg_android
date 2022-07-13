//
// Created by Heck on 2020/10/28.
//

#include "Utils.h"
float GetFrameTime(){
    static unsigned long long lastTime=0,currentTime=0;
    timeval current;
    gettimeofday(&current, nullptr);
    currentTime=current.tv_sec*1000+current.tv_usec/1000;
    unsigned long long frameTime=lastTime==0?0:currentTime-lastTime;
    lastTime=currentTime;
    return float(frameTime)/1000.0f;
}
GLuint CompileShader(GLenum shader_type,const char * source_code){
    GLuint shader_to_ret=glCreateShader(shader_type);
    glShaderSource(shader_to_ret,1,&source_code, nullptr);
    glCompileShader(shader_to_ret);
    GLint compile_result=GL_TRUE;
    glGetShaderiv(shader_to_ret,GL_COMPILE_STATUS,&compile_result);
    if(compile_result==GL_FALSE){
        char szLog[1024]={0};
        GLsizei logLen=0;
        glGetShaderInfoLog(shader_to_ret,1024,&logLen,szLog);
        __android_log_print(ANDROID_LOG_ERROR,ALICE_LOG_TAG,"CompileShader %s\ncode:\n%s",szLog,source_code);
        glDeleteShader(shader_to_ret);
        shader_to_ret=0;
    }
    return shader_to_ret;
}