attribute vec4 position;
attribute vec4 normal;
uniform mat4 U_ModelMatrix;
uniform mat4 U_ViewMatrix;
uniform mat4 U_ProjectionMatrix;
uniform mat4 U_NormalMatrix;
varying vec4 V_Normal;
varying vec4 V_WorldPos;
void main(){
    V_Normal=U_NormalMatrix*normal;
    V_WorldPos=U_ModelMatrix*position;
    gl_Position=U_ProjectionMatrix*U_ViewMatrix*V_WorldPos;
}