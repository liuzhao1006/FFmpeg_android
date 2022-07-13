#include "Sence.h"
#include "Utils.h"
static AAssetManager *sAssetManager= nullptr;
GLuint vbo;//vertex buffer object
GLuint ibo;//index buffer object ,element array buffer object
GLuint program;
GLint modelMatrixLocation,viewMatrixLocation,projectionMatrixLocation,textureYLocation,textureULocation,textureVLocation;
GLint attrPositionLocation,attrTexCoordLocation;
GLuint textureY,textureU,textureV;
glm::mat4 modelMatrix,viewMatrix,projectionMatrix,modelMatrix2;
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
    textureY=CreateTexture2D(nullptr,1280,720,GL_ALPHA,GL_ALPHA);
    textureU=CreateTexture2D(nullptr,640,360,GL_ALPHA,GL_ALPHA);
    textureV=CreateTexture2D(nullptr,640,360,GL_ALPHA,GL_ALPHA);
    //ffmpeg -> yuv
    SubmitTexture2D(textureY,YData,0,0,1280,720,GL_ALPHA,GL_UNSIGNED_BYTE);
    SubmitTexture2D(textureU,UData,0,0,640,360,GL_ALPHA,GL_UNSIGNED_BYTE);
    SubmitTexture2D(textureV,VData,0,0,640,360,GL_ALPHA,GL_UNSIGNED_BYTE);
    delete [] filecontent;
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

    vertices[0].mPosition[0]=-128.0f*scale;//x
    vertices[0].mPosition[1]=-72.0f*scale;//y
    vertices[0].mPosition[2]=0.0f;//z
    vertices[0].mPosition[3]=1.0f;//w
    vertices[0].mTexcoord[0]=0.0f;//u
    vertices[0].mTexcoord[1]=0.0f;//v
    vertices[0].mTexcoord[2]=0.0f;//
    vertices[0].mTexcoord[3]=0.0f;//

    vertices[1].mPosition[0]=128.0f*scale;//x
    vertices[1].mPosition[1]=-72.0f*scale;//y
    vertices[1].mPosition[2]=0.0f;//z
    vertices[1].mPosition[3]=1.0f;//w
    vertices[1].mTexcoord[0]=1.0f;//u
    vertices[1].mTexcoord[1]=0.0f;//v
    vertices[1].mTexcoord[2]=0.0f;//
    vertices[1].mTexcoord[3]=0.0f;//

    vertices[2].mPosition[0]=-128.0f*scale;//x
    vertices[2].mPosition[1]=72.0f*scale;//y
    vertices[2].mPosition[2]=0.0f;//z
    vertices[2].mPosition[3]=1.0f;//w
    vertices[2].mTexcoord[0]=0.0f;//u
    vertices[2].mTexcoord[1]=1.0f;//v
    vertices[2].mTexcoord[2]=0.0f;//
    vertices[2].mTexcoord[3]=0.0f;//

    vertices[3].mPosition[0]=128.0f*scale;//x
    vertices[3].mPosition[1]=72.0f*scale;//y
    vertices[3].mPosition[2]=0.0f;//z
    vertices[3].mPosition[3]=1.0f;//w
    vertices[3].mTexcoord[0]=1.0f;//u
    vertices[3].mTexcoord[1]=1.0f;//v
    vertices[3].mTexcoord[2]=0.0f;//
    vertices[3].mTexcoord[3]=0.0f;//

    modelMatrix=glm::translate(0.0f,0.0f,-1.0f);
    modelMatrix2=glm::translate(50.0f,0.0f,-2.0f);

    unsigned short indexes[]={ 0,1,2,1,3,2};
    ibo=CreateBufferObject(GL_ELEMENT_ARRAY_BUFFER,indexes,sizeof(unsigned short)*6,GL_STATIC_DRAW);
    vbo=CreateBufferObject(GL_ARRAY_BUFFER,vertices,sizeof(Vertice)*4,GL_STATIC_DRAW);

    program=CreateStandardProgram("yuv420.vs","yuv420.fs");
    attrPositionLocation=glGetAttribLocation(program,"position");
    attrTexCoordLocation=glGetAttribLocation(program,"texcoord");
    modelMatrixLocation=glGetUniformLocation(program,"U_ModelMatrix");
    viewMatrixLocation=glGetUniformLocation(program,"U_ViewMatrix");
    projectionMatrixLocation=glGetUniformLocation(program,"U_ProjectionMatrix");
    textureYLocation=glGetUniformLocation(program,"U_TextureY");
    textureULocation=glGetUniformLocation(program,"U_TextureU");
    textureVLocation=glGetUniformLocation(program,"U_TextureV");

    InitYUVTextures();
    __android_log_print(ANDROID_LOG_INFO,ALICE_LOG_TAG,"%d,%d,%d,%d",
            attrPositionLocation,modelMatrixLocation,viewMatrixLocation,projectionMatrixLocation);
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
    if(program==0){
        return ;
    }
    glBindBuffer(GL_ARRAY_BUFFER,vbo);
    glUseProgram(program);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,textureY);
    glUniform1i(textureYLocation,0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D,textureU);
    glUniform1i(textureULocation,1);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D,textureV);
    glUniform1i(textureVLocation,2);

    glUniformMatrix4fv(modelMatrixLocation,1,GL_FALSE,glm::value_ptr(modelMatrix));
    glUniformMatrix4fv(viewMatrixLocation,1,GL_FALSE,glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(projectionMatrixLocation,1,GL_FALSE,glm::value_ptr(projectionMatrix));
    //set attribute
    glEnableVertexAttribArray(attrPositionLocation);
    glVertexAttribPointer(attrPositionLocation,4,GL_FLOAT,GL_FALSE,sizeof(Vertice),0);
    glEnableVertexAttribArray(attrTexCoordLocation);
    glVertexAttribPointer(attrTexCoordLocation,4,GL_FLOAT,GL_FALSE,sizeof(Vertice),(void*)(sizeof(float)*4));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ibo);
    glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_SHORT,0);//dc
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glUseProgram(0);
}