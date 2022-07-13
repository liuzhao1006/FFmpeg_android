#ifdef GL_ES
precision mediump float;
#endif
uniform sampler2D U_Texture;
varying vec4 V_Texcoord;
void main(){
    gl_FragColor=texture2D(U_Texture,vec2(gl_PointCoord.x,1.0-gl_PointCoord.y))*V_Texcoord;
}