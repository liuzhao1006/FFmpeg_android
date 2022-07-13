#ifdef GL_ES
precision mediump float;
#endif
uniform sampler2D U_TextureY;
uniform sampler2D U_TextureU;
uniform sampler2D U_TextureV;
varying vec4 V_Texcoord;
vec3 YUV420ToRGB(float y,float u,float v){
    return vec3(y+1.402*v,y-0.34413*u-0.71414*v,y+1.772*u);
}
vec3 YUV420ToRGBViaMatrix(float y,float u,float v){
    mat3 transform_matrix=mat3(
        1.0,1.0,1.0,
        0.0,-0.34413,1.772,
        1.402,-0.71414,0.0
    );
    return transform_matrix*vec3(y,u,v);
}
void main(){
    vec2 texcoord=vec2(V_Texcoord.x,1.0-V_Texcoord.y);
    float Y=texture2D(U_TextureY,texcoord).a;
    float U=texture2D(U_TextureU,texcoord).a-0.5;
    float V=texture2D(U_TextureV,texcoord).a-0.5;
    gl_FragColor=vec4(YUV420ToRGBViaMatrix(Y,U,V),1.0);
}