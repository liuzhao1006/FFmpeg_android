#ifdef GL_ES
precision mediump float;
#endif
uniform sampler2D U_TextureYUV;
varying vec4 V_TexcoordY;
varying vec4 V_TexcoordUV;
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
    float texcoord_t=720.0/(720.0+360.0);
    vec2 texcoord_y_channel=vec2(V_TexcoordY.x,texcoord_t-V_TexcoordY.y);
    vec2 texcoord_u_channel=vec2(V_TexcoordUV.x,1.0+texcoord_t-V_TexcoordUV.y);
    vec2 texcoord_v_channel=vec2(V_TexcoordUV.z,1.0+texcoord_t-V_TexcoordUV.w);
    float Y=texture2D(U_TextureYUV,texcoord_y_channel).a;
    float U=texture2D(U_TextureYUV,texcoord_u_channel).a-0.5;
    float V=texture2D(U_TextureYUV,texcoord_v_channel).a-0.5;
    gl_FragColor=vec4(YUV420ToRGB(Y,U,V),1.0);
}