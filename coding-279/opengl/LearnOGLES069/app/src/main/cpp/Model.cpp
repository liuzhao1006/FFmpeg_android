//
// Created by Heck on 2020/11/10.
//

#include "Model.h"
#include "Utils.h"
struct FloatData{
    float v[3];
};
struct VertexDefine{
    int mPositionIndex;
    int mTexcoordIndex;
    int mNormalIndex;
};
void DecodeVertexData(std::vector<FloatData>&positions,
        std::vector<FloatData>&texcoords,
        std::vector<FloatData>&normals,
        char*vertex_data_str){
    std::string temp_str;
    std::stringstream ssOneLine(vertex_data_str);
    if(vertex_data_str[1]=='t'){
        FloatData fd;
        ssOneLine>>temp_str;//vt
        ssOneLine>>fd.v[0];
        ssOneLine>>fd.v[1];
        texcoords.push_back(fd);
    }else if(vertex_data_str[1]=='n'){
        FloatData fd;
        ssOneLine>>temp_str;//vn
        ssOneLine>>fd.v[0];
        ssOneLine>>fd.v[1];
        ssOneLine>>fd.v[2];
        normals.push_back(fd);
    }else{
        FloatData fd;
        ssOneLine>>temp_str;//v
        ssOneLine>>fd.v[0];
        ssOneLine>>fd.v[1];
        ssOneLine>>fd.v[2];
        positions.push_back(fd);
    }
}
void Model::SetModel(const char *path) {
    std::vector<FloatData> positions,texcoords,normals;
    std::vector<VertexDefine> triangles;
    int filesize=0;
    unsigned char *obj_file_content=LoadFileContent(path,filesize);
    std::stringstream ssFileContent((char*)obj_file_content);
    std::string temp_str;
    char oneLine[256];
    while(ssFileContent.eof()==false){
        memset(oneLine,0,256);
        ssFileContent.getline(oneLine,256);
        if(strlen(oneLine)>0){
            if(oneLine[0]=='v'){
                DecodeVertexData(positions,texcoords,normals,oneLine);
            }else if(oneLine[0]=='f'){
                std::stringstream ssOneLine(oneLine);
                std::string  vertex_define_str;
                ssOneLine>>temp_str;//f
                for (int i = 0; i < 3; ++i) {
                    ssOneLine>>vertex_define_str;
                    size_t pos=vertex_define_str.find_first_of('/');
                    std::string position_index_str=vertex_define_str.substr(0,pos);
                    size_t pos2=vertex_define_str.find_first_of('/',pos+1);
                    std::string texcoord_index_str=vertex_define_str.substr(pos+1,pos2-1-pos);
                    std::string normal_index_str=vertex_define_str.substr(pos2+1,vertex_define_str.length()-1-pos2);
                    VertexDefine vd;
                    vd.mPositionIndex=atoi(position_index_str.c_str())-1;
                    vd.mTexcoordIndex=atoi(texcoord_index_str.c_str())-1;
                    vd.mNormalIndex=atoi(normal_index_str.c_str())-1;
                    triangles.push_back(vd);
                }
            }
        }
    }

    delete[] obj_file_content;
    int vertex_count=(int)triangles.size();
    SetVertexCount(vertex_count);
    for (int i = 0; i < vertex_count; ++i) {
        float *temp=positions[triangles[i].mPositionIndex].v;
        SetPosition(i,temp[0],temp[1],temp[2]);
        temp=texcoords[triangles[i].mTexcoordIndex].v;
        SetTexcoord(i,temp[0],temp[1],0.0f);
        temp=normals[triangles[i].mNormalIndex].v;
        SetNormal(i,temp[0],temp[1],temp[2]);
    }
    Submit();
    mPrimitiveType=GL_TRIANGLES;
}