#include "Geometry.h"

Geometry::Geometry() {
    mbDrawWidthIBO=false;
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