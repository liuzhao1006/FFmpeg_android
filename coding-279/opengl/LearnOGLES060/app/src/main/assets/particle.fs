#ifdef GL_ES
precision mediump float;
#endif
uniform sampler2D U_Texture;
void main(){
    gl_FragColor=texture2D(U_Texture,vec2(1.0)-gl_PointCoord.xy);
}