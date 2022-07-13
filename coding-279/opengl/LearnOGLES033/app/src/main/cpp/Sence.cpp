#include "Sence.h"
#include "Utils.h"
static AAssetManager *sAssetManager= nullptr;
GLuint vbo;//vertex buffer object
GLuint ibo;//index buffer object ,element array buffer object
GLuint program;
GLint modelMatrixLocation,viewMatrixLocation,projectionMatrixLocation,imagesizeLocation,mskRangeLocation,mskSettingLocation;
GLint attrPositionLocation,attrTexCoordLocation;
GLuint texture;
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
extern "C" JNIEXPORT void JNICALL Java_cn_battlefire_learnogles_MainActivity_Init(
        JNIEnv*env,
        jobject MainActivity,jobject am
){
    sAssetManager=AAssetManager_fromJava(env,am);
    __android_log_print(ANDROID_LOG_INFO,ALICE_LOG_TAG,"Init");
    glClearColor(0.6f,0.4f,0.1f,1.0f);
    Vertice vertices[4];//cpu -> gpu

    float scale=0.5f;

    vertices[0].mPosition[0]=-128.0f*scale;//x
    vertices[0].mPosition[1]=-128.0f*scale;//y
    vertices[0].mPosition[2]=0.0f;//z
    vertices[0].mPosition[3]=1.0f;//w
    vertices[0].mTexcoord[0]=0.0f;//u
    vertices[0].mTexcoord[1]=0.0f;//v
    vertices[0].mTexcoord[2]=0.0f;//
    vertices[0].mTexcoord[3]=0.0f;//

    vertices[1].mPosition[0]=128.0f*scale;//x
    vertices[1].mPosition[1]=-128.0f*scale;//y
    vertices[1].mPosition[2]=0.0f;//z
    vertices[1].mPosition[3]=1.0f;//w
    vertices[1].mTexcoord[0]=1.0f;//u
    vertices[1].mTexcoord[1]=0.0f;//v
    vertices[1].mTexcoord[2]=0.0f;//
    vertices[1].mTexcoord[3]=0.0f;//

    vertices[2].mPosition[0]=-128.0f*scale;//x
    vertices[2].mPosition[1]=128.0f*scale;//y
    vertices[2].mPosition[2]=0.0f;//z
    vertices[2].mPosition[3]=1.0f;//w
    vertices[2].mTexcoord[0]=0.0f;//u
    vertices[2].mTexcoord[1]=1.0f;//v
    vertices[2].mTexcoord[2]=0.0f;//
    vertices[2].mTexcoord[3]=0.0f;//

    vertices[3].mPosition[0]=128.0f*scale;//x
    vertices[3].mPosition[1]=128.0f*scale;//y
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

    program=CreateStandardProgram("test.vs","MSK.fs");
    attrPositionLocation=glGetAttribLocation(program,"position");
    attrTexCoordLocation=glGetAttribLocation(program,"texcoord");
    modelMatrixLocation=glGetUniformLocation(program,"U_ModelMatrix");
    viewMatrixLocation=glGetUniformLocation(program,"U_ViewMatrix");
    projectionMatrixLocation=glGetUniformLocation(program,"U_ProjectionMatrix");
    imagesizeLocation=glGetUniformLocation(program,"U_ImageSize");
    mskRangeLocation=glGetUniformLocation(program,"U_MSKRange");
    mskSettingLocation=glGetUniformLocation(program,"U_MSKSetting");

    texture=CreateTextureFromFile("head.png");
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
    glBindTexture(GL_TEXTURE_2D,texture);
    glUniform1i(glGetUniformLocation(program,"U_Texture"),0);
    glUniformMatrix4fv(modelMatrixLocation,1,GL_FALSE,glm::value_ptr(modelMatrix));
    glUniformMatrix4fv(viewMatrixLocation,1,GL_FALSE,glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(projectionMatrixLocation,1,GL_FALSE,glm::value_ptr(projectionMatrix));
    float imagesize[]={108.0f,108.0f,0.0f,0.0f};
    float mskrange[]={32.0f,32.0f,64.0f,32.0f};
    float msksetting[]={8.0f,0.0f,0.0f,0.0f};
    glUniform4fv(imagesizeLocation,1,imagesize);
    glUniform4fv(mskRangeLocation,1,mskrange);
    glUniform4fv(mskSettingLocation,1,msksetting);
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
