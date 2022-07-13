#ifdef GL_ES
precision mediump float;
#endif
uniform sampler2D U_Texture;
uniform sampler2D U_Texture2;
varying vec4 V_Texcoord;
void main(){
    gl_FragColor=texture2D(U_Texture,V_Texcoord.xy)*texture2D(U_Texture2,V_Texcoord.xy);
}