#version 300 es
in vec4 position;
in vec4 texcoord;
uniform mat4 U_ModelMatrix;
uniform mat4 U_ViewMatrix;
uniform mat4 U_ProjectionMatrix;
out vec4 V_Texcoord;
void main(){
    V_Texcoord=texcoord;
    gl_Position=U_ProjectionMatrix*U_ViewMatrix*U_ModelMatrix*position;
}