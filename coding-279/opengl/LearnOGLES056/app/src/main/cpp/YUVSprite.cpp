//
// Created by Heck on 2020/11/6.
//

#include "Geometry.h"
#include "YUVSprite.h"
#include "Utils.h"

void YUVSprite::Init() {
    float scale=5.0f;
    float texcoord_t=720.0f/(720.0f+360.0f);
    SetVertexCount(4);
    SetPosition(0,-128.0f*scale,-72.0f*scale,0.0f);
    SetTexcoord(0,0.0f,0.0f,0.0f,0.0f);
    SetTexcoord1(0,0.0F,texcoord_t,0.5f,texcoord_t);

    SetPosition(1,128.0f*scale,-72.0f*scale,0.0f);
    SetTexcoord(1,1.0f,0.0f,0.0f,0.0f);
    SetTexcoord1(1,0.5f,texcoord_t,1.0f,texcoord_t);

    SetPosition(2,-128.0f*scale,72.0f*scale,0.0f);
    SetTexcoord(2,0.0f,texcoord_t,0.0f,0.0f);
    SetTexcoord1(2,0.0f,1.0f,0.5f,1.0f);

    SetPosition(3,128.0f*scale,72.0f*scale,0.0f);
    SetTexcoord(3,1.0f,texcoord_t,0.0f,0.0f);
    SetTexcoord1(3,0.5f,1.0f,1.0f,1.0f);

    Submit();
    mPrimitiveType=GL_TRIANGLE_STRIP;
}

GLuint YUVSprite::InitYUVTexture(){
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
    return textureYUV;
}
