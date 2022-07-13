#ifdef GL_ES
precision mediump float;
#endif
uniform sampler2D U_Texture;
varying vec4 V_Texcoord;
float HardLight(float lluminance){
    if(lluminance<=0.5){
        lluminance=2.0*lluminance*lluminance;
    }else{
        lluminance=1.0-2.0*(1.0-lluminance)*(1.0-lluminance);
    }
    return lluminance;
}
void main(){
    vec4 color=vec4(0.0);
    color=texture2D(U_Texture,V_Texcoord.xy);
    float intensity=0.6;
    intensity=HardLight(intensity);
    color=vec4(color.rgb*intensity,1.0);
    gl_FragColor=color;
}