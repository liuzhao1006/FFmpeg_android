//
// Created by Heck on 2020/11/10.
//

#include "Model.h"
#include "Utils.h"

void Model::SetModel(const char *path) {
    struct FloatData{
        float v[3];
    };
    std::vector<FloatData> positions,texcoords,normals;
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
                std::stringstream ssOneLine(oneLine);
                if(oneLine[1]=='t'){
                    FloatData fd;
                    ssOneLine>>temp_str;//vt
                    ssOneLine>>fd.v[0];
                    ssOneLine>>fd.v[1];
                    texcoords.push_back(fd);
                }else if(oneLine[1]=='n'){
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
        }
    }
}