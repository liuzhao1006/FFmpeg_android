#include "Sence.h"
#include "Utils.h"
static AAssetManager *sAssetManager= nullptr;
GLuint vbo;//vertex buffer object
GLuint program;
GLint modelMatrixLocation,viewMatrixLocation,projectionMatrixLocation;
GLint attrPositionLocation,attrColorLocation;
glm::mat4 modelMatrix,viewMatrix,projectionMatrix;
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
    vertices[0].mPosition[2]=0.0f;//z
    vertices[0].mPosition[3]=1.0f;//w
    vertices[0].mColor[0]=1.0f;//r
    vertices[0].mColor[1]=1.0f;//g
    vertices[0].mColor[2]=0.0f;//b
    vertices[0].mColor[3]=1.0f;//a

    vertices[1].mPosition[0]=0.5f;//x
    vertices[1].mPosition[1]=-0.5f;//y
    vertices[1].mPosition[2]=0.0f;//z
    vertices[1].mPosition[3]=1.0f;//w
    vertices[1].mColor[0]=1.0f;//r
    vertices[1].mColor[1]=0.0f;//g
    vertices[1].mColor[2]=1.0f;//b
    vertices[1].mColor[3]=1.0f;//a

    vertices[2].mPosition[0]=0.0f;//x
    vertices[2].mPosition[1]=0.5f;//y
    vertices[2].mPosition[2]=0.0f;//z
    vertices[2].mPosition[3]=1.0f;//w
    vertices[2].mColor[0]=0.0f;//r
    vertices[2].mColor[1]=1.0f;//g
    vertices[2].mColor[2]=1.0f;//b
    vertices[2].mColor[3]=1.0f;//a

    modelMatrix=glm::translate(0.0f,0.0f,-2.0f)*glm::scale(0.5f,0.5f,0.5f)
            *glm::rotate(30.0f,0.0f,0.0f,1.0f);

    glGenBuffers(1,&vbo);
    glBindBuffer(GL_ARRAY_BUFFER,vbo);
    glBufferData(GL_ARRAY_BUFFER,sizeof(Vertice)*3, nullptr,GL_STATIC_DRAW);//alloc gpu
    glBufferSubData(GL_ARRAY_BUFFER,0,sizeof(Vertice)*3,vertices);//cpu -> gpu
    //glBufferData(GL_ARRAY_BUFFER,sizeof(Vertice)*3,vertices,GL_STATIC_DRAW);//cpu -> gpu
    glBindBuffer(GL_ARRAY_BUFFER,0);

    program=CreateStandardProgram("test.vs","test.fs");
    attrPositionLocation=glGetAttribLocation(program,"position");
    attrColorLocation=glGetAttribLocation(program,"color");
    modelMatrixLocation=glGetUniformLocation(program,"U_ModelMatrix");
    viewMatrixLocation=glGetUniformLocation(program,"U_ViewMatrix");
    projectionMatrixLocation=glGetUniformLocation(program,"U_ProjectionMatrix");
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
    projectionMatrix=glm::perspective(45.0f,float(width)/float(height),0.1f,1000.0f);
}
extern "C" JNIEXPORT void JNICALL Java_cn_battlefire_learnogles_MainActivity_Render(
        JNIEnv*env,
        jobject MainActivity
){
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
    glBindBuffer(GL_ARRAY_BUFFER,vbo);
    glUseProgram(program);
    glUniformMatrix4fv(modelMatrixLocation,1,GL_FALSE,glm::value_ptr(modelMatrix));
    glUniformMatrix4fv(viewMatrixLocation,1,GL_FALSE,glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(projectionMatrixLocation,1,GL_FALSE,glm::value_ptr(projectionMatrix));
    //set attribute
    glEnableVertexAttribArray(attrPositionLocation);
    glVertexAttribPointer(attrPositionLocation,4,GL_FLOAT,GL_FALSE,sizeof(Vertice),0);
    glEnableVertexAttribArray(attrColorLocation);
    glVertexAttribPointer(attrColorLocation,4,GL_FLOAT,GL_FALSE,sizeof(Vertice),(void*)(sizeof(float)*4));
    glDrawArrays(GL_TRIANGLES,0,3);
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glUseProgram(0);
}
