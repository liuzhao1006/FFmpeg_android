#include "Geometry.h"
#include "Utils.h"
Geometry::Geometry() {
    mbDrawWidthIBO=false;
    mPrimitiveType=GL_TRIANGLES;
}
void Geometry::SetIndexCount(int index_count)  {
    mbDrawWidthIBO=true;
    mIndexCount=index_count;
    mIndexes=new unsigned short[index_count];
    mIBO=CreateBufferObject(GL_ELEMENT_ARRAY_BUFFER, nullptr,sizeof(unsigned short)*index_count,GL_STATIC_DRAW);
}
void Geometry::SetVertexCount(int vertex_count) {
    mVertexCount=vertex_count;
    mVertices=new Vertice[vertex_count];
    mVBO=CreateBufferObject(GL_ARRAY_BUFFER,nullptr,sizeof(Vertice)*vertex_count,GL_STATIC_DRAW);
}
void Geometry::SetIndex(int i, unsigned short index) {
    mIndexes[i]=index;
}
void Geometry::SetPosition(int index, float x, float y, float z, float w) {
    mVertices[index].mPosition[0]=x;
    mVertices[index].mPosition[1]=y;
    mVertices[index].mPosition[2]=z;
    mVertices[index].mPosition[3]=w;
}
void Geometry::SetTexcoord(int index, float x, float y, float z, float w) {
    mVertices[index].mTexcoord[0]=x;
    mVertices[index].mTexcoord[1]=y;
    mVertices[index].mTexcoord[2]=z;
    mVertices[index].mTexcoord[3]=w;
}
void Geometry::SetTexcoord1(int index, float x, float y, float z, float w) {
    mVertices[index].mTexcoord1[0]=x;
    mVertices[index].mTexcoord1[1]=y;
    mVertices[index].mTexcoord1[2]=z;
    mVertices[index].mTexcoord1[3]=w;
}
void Geometry::SetNormal(int index, float x, float y, float z, float w) {
    mVertices[index].mNormal[0]=x;
    mVertices[index].mNormal[1]=y;
    mVertices[index].mNormal[2]=z;
    mVertices[index].mNormal[3]=w;
}
void Geometry::Submit() {
    UpdateBufferObject(mVBO,GL_ARRAY_BUFFER,mVertices,sizeof(Vertice)*mVertexCount,0);
    if(mbDrawWidthIBO){
        UpdateBufferObject(mIBO,GL_ELEMENT_ARRAY_BUFFER,mIndexes,sizeof(unsigned short)*mIndexCount,0);
    }
}
void Geometry::BeginDraw() {
    glBindBuffer(GL_ARRAY_BUFFER,mVBO);
    if(mbDrawWidthIBO){
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,mIBO);
    }
}
void Geometry::Draw() {
    if(mbDrawWidthIBO){
        glDrawElements(mPrimitiveType,mIndexCount,GL_UNSIGNED_SHORT,0);
    }else{
        glDrawArrays(mPrimitiveType,0,mVertexCount);
    }
}
void Geometry::EndDraw() {
    glBindBuffer(GL_ARRAY_BUFFER,0);
    if(mbDrawWidthIBO){
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
    }
}