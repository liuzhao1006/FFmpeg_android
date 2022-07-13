#version 300 es
#ifdef GL_ES
precision mediump float;
#endif
uniform sampler2D U_Texture;
in vec4 V_Texcoord;
layout(location=0)out vec4 FragColor0;
void main(){
    FragColor0=texture(U_Texture,V_Texcoord.xy);
}