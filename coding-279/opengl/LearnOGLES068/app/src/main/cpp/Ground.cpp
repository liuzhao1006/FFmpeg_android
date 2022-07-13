//
// Created by Heck on 2020/11/10.
//

#include "Ground.h"
void Ground::Init() {
    SetVertexCount(1600);
    mPrimitiveType=GL_TRIANGLE_STRIP;
    for (int z = 0; z < 20; ++z) {
        float zStart=100.0f-z*10.0f;
        for (int x = 0; x < 20; ++x) {
            float xStart=-100.0f+10.0f*x;
            int quad_index=x+z*20;
            int quad_first_vertice_index=quad_index*4;
            SetPosition(quad_first_vertice_index,xStart,-1.0f,zStart);
            SetTexcoord(quad_first_vertice_index,0.0f,0.0f,0.0f);
            SetNormal(quad_first_vertice_index,0.0f,1.0f,0.0f);

            SetPosition(quad_first_vertice_index+1,xStart+10.0f,-1.0f,zStart);
            SetTexcoord(quad_first_vertice_index+1,1.0f,0.0f,0.0f);
            SetNormal(quad_first_vertice_index+1,0.0f,1.0f,0.0f);

            SetPosition(quad_first_vertice_index+2,xStart,-1.0f,zStart-10.0f);
            SetTexcoord(quad_first_vertice_index+2,0.0f,1.0f,0.0f);
            SetNormal(quad_first_vertice_index+2,0.0f,1.0f,0.0f);

            SetPosition(quad_first_vertice_index+3,xStart+10.0f,-1.0f,zStart-10.0f);
            SetTexcoord(quad_first_vertice_index+3,1.0f,1.0f,0.0f);
            SetNormal(quad_first_vertice_index+3,0.0f,1.0f,0.0f);
        }
    }
    Submit();
}
void Ground::Draw() {
    for (int i = 0; i < 400; ++i) {
        glDrawArrays(GL_TRIANGLE_STRIP,i*4,4);
    }
}
