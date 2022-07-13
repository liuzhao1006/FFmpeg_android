attribute vec4 position;
attribute vec4 texcoord;
attribute vec4 texcoord1;
uniform mat4 U_ModelMatrix;
uniform mat4 U_ViewMatrix;
uniform mat4 U_ProjectionMatrix;
varying vec4 V_TexcoordY;
varying vec4 V_TexcoordUV;
void main(){
    V_TexcoordY=texcoord;
    V_TexcoordUV=texcoord1;
    gl_Position=U_ProjectionMatrix*U_ViewMatrix*U_ModelMatrix*position;
}