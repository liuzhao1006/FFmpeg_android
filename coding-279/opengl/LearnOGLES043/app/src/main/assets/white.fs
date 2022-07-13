#ifdef GL_ES
precision mediump float;
#endif
uniform sampler2D U_Texture;
uniform vec4 U_WhiteSetting;
varying vec4 V_Texcoord;
void main(){
    vec4 color=vec4(0.0);
    float base_factor=U_WhiteSetting.x;
    float white_level=U_WhiteSetting.y;
    color=texture2D(U_Texture,V_Texcoord.xy);
    color.r+=base_factor*white_level;
    color.g+=base_factor*white_level;
    color.b+=base_factor*white_level;
    gl_FragColor=color;
}