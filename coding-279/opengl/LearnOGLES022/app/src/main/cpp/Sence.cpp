#include "Sence.h"
#include "Utils.h"
static AAssetManager *sAssetManager= nullptr;
GLuint vbo;//vertex buffer object
GLuint ibo;//index buffer object ,element array buffer object
GLuint program;
GLint modelMatrixLocation,viewMatrixLocation,projectionMatrixLocation;
GLint attrPositionLocation,attrColorLocation;
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

    vertices[0].mPosition[0]=-50.0f;//x
    vertices[0].mPosition[1]=-50.0f;//y
    vertices[0].mPosition[2]=0.0f;//z
    vertices[0].mPosition[3]=1.0f;//w
    vertices[0].mColor[0]=1.0f;//r
    vertices[0].mColor[1]=1.0f;//g
    vertices[0].mColor[2]=0.0f;//b
    vertices[0].mColor[3]=1.0f;//a

    vertices[1].mPosition[0]=50.0f;//x
    vertices[1].mPosition[1]=-50.0f;//y
    vertices[1].mPosition[2]=0.0f;//z
    vertices[1].mPosition[3]=1.0f;//w
    vertices[1].mColor[0]=1.0f;//r
    vertices[1].mColor[1]=0.0f;//g
    vertices[1].mColor[2]=1.0f;//b
    vertices[1].mColor[3]=1.0f;//a

    vertices[2].mPosition[0]=-50.0f;//x
    vertices[2].mPosition[1]=50.0f;//y
    vertices[2].mPosition[2]=0.0f;//z
    vertices[2].mPosition[3]=1.0f;//w
    vertices[2].mColor[0]=0.0f;//r
    vertices[2].mColor[1]=1.0f;//g
    vertices[2].mColor[2]=1.0f;//b
    vertices[2].mColor[3]=1.0f;//a

    vertices[3].mPosition[0]=50.0f;//x
    vertices[3].mPosition[1]=50.0f;//y
    vertices[3].mPosition[2]=0.0f;//z
    vertices[3].mPosition[3]=1.0f;//w
    vertices[3].mColor[0]=0.0f;//r
    vertices[3].mColor[1]=1.0f;//g
    vertices[3].mColor[2]=1.0f;//b
    vertices[3].mColor[3]=1.0f;//a

    modelMatrix=glm::translate(0.0f,0.0f,-1.0f);
    modelMatrix2=glm::translate(50.0f,0.0f,-2.0f);

    unsigned short indexes[]={ 0,1,2,1,3,2};
    ibo=CreateBufferObject(GL_ELEMENT_ARRAY_BUFFER,indexes,sizeof(unsigned short)*6,GL_STATIC_DRAW);
    vbo=CreateBufferObject(GL_ARRAY_BUFFER,vertices,sizeof(Vertice)*4,GL_STATIC_DRAW);

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
    float half_width=float(width)/2.0f;
    float half_height=float(height)/2.0f;
    projectionMatrix=glm::ortho(-half_width,half_width,-half_height,half_height,0.1f,100.0f);
}
extern "C" JNIEXPORT void JNICALL Java_cn_battlefire_learnogles_MainActivity_Render(
        JNIEnv*env,
        jobject MainActivity
){
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
    glBindBuffer(GL_ARRAY_BUFFER,vbo);
    glUseProgram(program);
    glEnable(GL_DEPTH_TEST);
    glUniformMatrix4fv(modelMatrixLocation,1,GL_FALSE,glm::value_ptr(modelMatrix));
    glUniformMatrix4fv(viewMatrixLocation,1,GL_FALSE,glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(projectionMatrixLocation,1,GL_FALSE,glm::value_ptr(projectionMatrix));
    //set attribute
    glEnableVertexAttribArray(attrPositionLocation);
    glVertexAttribPointer(attrPositionLocation,4,GL_FLOAT,GL_FALSE,sizeof(Vertice),0);
    glEnableVertexAttribArray(attrColorLocation);
    glVertexAttribPointer(attrColorLocation,4,GL_FLOAT,GL_FALSE,sizeof(Vertice),(void*)(sizeof(float)*4));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ibo);

    glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_SHORT,0);
    //glDrawArrays(GL_TRIANGLE_STRIP,0,4);

    glUniformMatrix4fv(modelMatrixLocation,1,GL_FALSE,glm::value_ptr(modelMatrix2));
    glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_SHORT,0);
    //glDrawArrays(GL_TRIANGLE_STRIP,0,4);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glUseProgram(0);
}
