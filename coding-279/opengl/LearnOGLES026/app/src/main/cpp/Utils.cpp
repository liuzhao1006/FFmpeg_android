//
// Created by Heck on 2020/10/28.
//

#include "Utils.h"
#include "stb_image_aug.h"
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
        __android_log_print(ANDROID_LOG_ERROR,ALICE_LOG_TAG,"CompileShader %s\ncode:\n%s\n",szLog,source_code);
        glDeleteShader(shader_to_ret);
        shader_to_ret=0;
    }
    return shader_to_ret;
}
GLuint CreateProgram(GLuint vertex_shader,GLuint fragment_shader){
    GLuint program_to_ret=glCreateProgram();
    glAttachShader(program_to_ret,vertex_shader);
    glAttachShader(program_to_ret,fragment_shader);
    glLinkProgram(program_to_ret);
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    GLint link_result=GL_TRUE;
    glGetProgramiv(program_to_ret,GL_LINK_STATUS,&link_result);
    if(GL_FALSE==link_result){
        char szLog[1024]={0};
        GLsizei logLen=0;
        glGetProgramInfoLog(program_to_ret,1024,&logLen,szLog);
        __android_log_print(ANDROID_LOG_ERROR,ALICE_LOG_TAG,"CreateProgram %s\n",szLog);
        glDeleteProgram(program_to_ret);
        program_to_ret=0;
    }
    return program_to_ret;
}
GLuint CreateStandardProgram(const char *vertex_shader_path,const char * fragment_shader_path){
    int filesize=0;
    unsigned char * filecontent=LoadFileContent(vertex_shader_path,filesize);
    GLuint vsshader=CompileShader(GL_VERTEX_SHADER,(char*)filecontent);
    delete [] filecontent;
    filecontent=LoadFileContent(fragment_shader_path,filesize);
    GLuint fsshader=CompileShader(GL_FRAGMENT_SHADER,(char*)filecontent);
    delete [] filecontent;
    GLuint program=CreateProgram(vsshader,fsshader);
    glDeleteShader(vsshader);
    glDeleteShader(fsshader);
    return program;
}
GLuint CreateBufferObject(GLenum type,void * data,int size,GLenum usage){
    GLuint object_to_ret;
    glGenBuffers(1,&object_to_ret);
    glBindBuffer(type,object_to_ret);
    glBufferData(type,size,data,usage);//cpu -> gpu
    glBindBuffer(type,0);
    return object_to_ret;
}
void UpdateBufferObject(GLuint object,GLenum type,void * data,int size,int offset){
    glBindBuffer(type,object);
    glBufferSubData(type,offset,size,data);//cpu -> gpu
    glBindBuffer(type,0);
}
void SwapPixelRB(unsigned char * pixel,int pixel_data_offset){//bgr -> rgb
    unsigned  char b=pixel[pixel_data_offset];
    pixel[pixel_data_offset]=pixel[pixel_data_offset+2];
    pixel[pixel_data_offset+2]=b;
}
unsigned char * DecodeBMP(unsigned char *bmp_file_content,int&width,int&height){
    if(0x4D42==*((unsigned short*)bmp_file_content)){
        int pixel_data_offset=*((int*)(bmp_file_content+10));
        width=*((int*)(bmp_file_content+18));
        height=*((int*)(bmp_file_content+22));
        unsigned char * pixel=bmp_file_content+pixel_data_offset;
        int pixel_data_count=width*height;
        for (int i = 0; i < pixel_data_count; ++i) {
            SwapPixelRB(pixel,i*3);
        }
        return pixel;
    }
    return nullptr;
}
GLuint CreateTexture2D(void*pixel,int width,int height,GLenum gpu_format,GLenum cpu_format){
    GLuint texture;
    glGenTextures(1,&texture);
    glBindTexture(GL_TEXTURE_2D,texture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_MIRRORED_REPEAT);
    glTexImage2D(GL_TEXTURE_2D,0,gpu_format,width,height,0,cpu_format,GL_UNSIGNED_BYTE,pixel);
    glBindTexture(GL_TEXTURE_2D,0);
    return texture;
}
GLuint CreateTextureFromFile(const char *path){
    int file_size=0;
    unsigned char* filecontent=LoadFileContent(path,file_size);
    int image_width,image_height,channel_count;
    unsigned char * rgb_pixel=stbi_bmp_load_from_memory(filecontent,file_size,&image_width,&image_height,&channel_count,0);
    //unsigned char * rgb_pixel=DecodeBMP(filecontent,image_width,image_height);
    GLuint texture=CreateTexture2D(rgb_pixel,image_width,image_height,GL_RGB,GL_RGB);
    delete [] rgb_pixel;
    delete [] filecontent;
    return texture;
}