//
// Created by Heck on 2020/11/9.
//

#include "FrameBufferObject.h"
FrameBufferObject::FrameBufferObject() {
    mFrameBufferObjectBeforeSwitch=0;
    mFrameBufferObject=0;
    mColorBuffer=0;
}
void FrameBufferObject::Init(int width, int height) {
    glGenFramebuffers(1,&mFrameBufferObject);
    //init color buffer
    GLuint color_buffer;
    glGenTextures(1,&color_buffer);
    glBindTexture(GL_TEXTURE_2D,color_buffer);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE, nullptr);
    glBindTexture(GL_TEXTURE_2D,0);
    //init depth buffer -> texture2d,render buffer object (read only)
    GLuint render_buffer;
    glGenRenderbuffers(1,&render_buffer);
    glBindRenderbuffer(GL_RENDERBUFFER,render_buffer);
    glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH24_STENCIL8_OES,width,height);
    glBindRenderbuffer(GL_RENDERBUFFER,0);

    mColorBuffer=color_buffer;
    //associate fbo with color buffer & depth buffer
    glGetIntegerv(GL_FRAMEBUFFER_BINDING,&mFrameBufferObjectBeforeSwitch);
    glBindFramebuffer(GL_FRAMEBUFFER,mFrameBufferObject);
    glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,color_buffer,0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_RENDERBUFFER,render_buffer);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_STENCIL_ATTACHMENT,GL_RENDERBUFFER,render_buffer);
    GLenum status=glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if(GL_FRAMEBUFFER_COMPLETE==status){
        __android_log_print(ANDROID_LOG_INFO,ALICE_LOG_TAG,"fbo create success %s\n",glGetString(GL_VERSION));
    }else{
        __android_log_print(ANDROID_LOG_ERROR,ALICE_LOG_TAG,"%s fbo create failed %d[%d]\n",
                glGetString(GL_VERSION),status,GL_FRAMEBUFFER_COMPLETE);
    }
    glBindFramebuffer(GL_FRAMEBUFFER,mFrameBufferObjectBeforeSwitch);
}
void FrameBufferObject::BeginDraw() {
    glGetIntegerv(GL_FRAMEBUFFER_BINDING,&mFrameBufferObjectBeforeSwitch);
    glBindFramebuffer(GL_FRAMEBUFFER,mFrameBufferObject);
    glClearColor(0.0f,0.0f,0.0f,0.0f);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
}
void FrameBufferObject::EndDraw() {
    glBindFramebuffer(GL_FRAMEBUFFER,mFrameBufferObjectBeforeSwitch);
}