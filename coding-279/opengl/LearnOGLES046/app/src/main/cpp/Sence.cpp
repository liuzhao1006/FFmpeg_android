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

    vertices[0].mPosition[0]=-128.0f*scale;//x
    vertices[0].mPosition[1]=-72.0f*scale;//y
    vertices[0].mPosition[2]=0.0f;//z
    vertices[0].mPosition[3]=1.0f;//w

    vertices[0].mTexcoord[0]=0.0f;//u
    vertices[0].mTexcoord[1]=0.0f;//v
    vertices[0].mTexcoord[2]=0.0f;//
    vertices[0].mTexcoord[3]=0.0f;//

    vertices[0].mTexcoord1[0]=0.0f;//Uu
    vertices[0].mTexcoord1[1]=texcoord_t;//Uv
    vertices[0].mTexcoord1[2]=0.5f;//Vu
    vertices[0].mTexcoord1[3]=texcoord_t;//Vv

    vertices[1].mPosition[0]=128.0f*scale;//x
    vertices[1].mPosition[1]=-72.0f*scale;//y
    vertices[1].mPosition[2]=0.0f;//z
    vertices[1].mPosition[3]=1.0f;//w

    vertices[1].mTexcoord[0]=1.0f;//u
    vertices[1].mTexcoord[1]=0.0f;//v
    vertices[1].mTexcoord[2]=0.0f;//
    vertices[1].mTexcoord[3]=0.0f;//
    vertices[1].mTexcoord1[0]=0.5f;//u
    vertices[1].mTexcoord1[1]=texcoord_t;//v
    vertices[1].mTexcoord1[2]=1.0f;//
    vertices[1].mTexcoord1[3]=texcoord_t;//

    vertices[2].mPosition[0]=-128.0f*scale;//x
    vertices[2].mPosition[1]=72.0f*scale;//y
    vertices[2].mPosition[2]=0.0f;//z
    vertices[2].mPosition[3]=1.0f;//w

    vertices[2].mTexcoord[0]=0.0f;//u
    vertices[2].mTexcoord[1]=texcoord_t;//v
    vertices[2].mTexcoord[2]=0.0f;//
    vertices[2].mTexcoord[3]=0.0f;//
    vertices[2].mTexcoord1[0]=0.0f;//u
    vertices[2].mTexcoord1[1]=1.0f;//v
    vertices[2].mTexcoord1[2]=0.5f;//
    vertices[2].mTexcoord1[3]=1.0f;//

    vertices[3].mPosition[0]=128.0f*scale;//x
    vertices[3].mPosition[1]=72.0f*scale;//y
    vertices[3].mPosition[2]=0.0f;//z
    vertices[3].mPosition[3]=1.0f;//w

    vertices[3].mTexcoord[0]=1.0f;//u
    vertices[3].mTexcoord[1]=texcoord_t;//v
    vertices[3].mTexcoord[2]=0.0f;//
    vertices[3].mTexcoord[3]=0.0f;//
    vertices[3].mTexcoord1[0]=0.5f;//u
    vertices[3].mTexcoord1[1]=1.0f;//v
    vertices[3].mTexcoord1[2]=1.0f;//
    vertices[3].mTexcoord1[3]=1.0f;//

    modelMatrix=glm::translate(0.0f,0.0f,-1.0f);

    unsigned short indexes[]={ 0,1,2,1,3,2};
    geometry.mIBO=CreateBufferObject(GL_ELEMENT_ARRAY_BUFFER,indexes,sizeof(unsigned short)*6,GL_STATIC_DRAW);
    geometry.mVBO=CreateBufferObject(GL_ARRAY_BUFFER,vertices,sizeof(Vertice)*4,GL_STATIC_DRAW);
    geometry.mPrimitiveType=GL_TRIANGLES;
    geometry.mbDrawWidthIBO=true;
    geometry.mIndexCount=6;
    geometry.mVertexCount=4;
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
