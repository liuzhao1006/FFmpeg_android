#ifdef GL_ES
precision mediump float;
#endif
uniform sampler2D U_Texture;
uniform vec4 U_Setting;
varying vec4 V_Texcoord;
float Blur(){
    float radius_in_pixel=1.0;
    float radius_in_texcoord_s=radius_in_pixel/U_Setting.y;
    float radius_in_texcoord_t=radius_in_pixel/U_Setting.z;
    float weight[20];
    for(int i=0;i<20;i++){
        if(i<12){
            weight[i]=1.0;
        }else{
            weight[i]=2.0;
        }
    }
    float color_g_channel=texture2D(U_Texture,V_Texcoord.xy).g*20.0;
    vec2 offset[20];
    offset[0]=vec2(0.0,-10.0);
    offset[1]=vec2(0.0,10.0);
    offset[2]=vec2(10.0,0.0);
    offset[3]=vec2(-10.0,0.0);
    offset[4]=vec2(5.0,-8.0);
    offset[5]=vec2(5.0,8.0);
    offset[6]=vec2(-5.0,-8.0);
    offset[7]=vec2(-5.0,8.0);
    offset[8]=vec2(8.0,-5.0);
    offset[9]=vec2(8.0,5.0);
    offset[10]=vec2(-8.0,-5.0);
    offset[11]=vec2(-8.0,5.0);
    offset[12]=vec2(0.0,-6.0);
    offset[13]=vec2(0.0,6.0);
    offset[14]=vec2(6.0,0.0);
    offset[15]=vec2(-6.0,0.0);
    offset[16]=vec2(4.0,-4.0);
    offset[17]=vec2(4.0,4.0);
    offset[18]=vec2(-4.0,-4.0);
    offset[19]=vec2(-4.0,4.0);
    for(int i=0;i<20;i++){
        float texcoord_x=V_Texcoord.x+float(offset[i].x)*radius_in_texcoord_s;
        float texcoord_y=V_Texcoord.y+float(offset[i].y)*radius_in_texcoord_t;
        color_g_channel+=texture2D(U_Texture,vec2(texcoord_x,texcoord_y)).g*weight[i];
    }
    return color_g_channel/48.0;
}
float HardLight(float luminance){
    if(luminance<=0.5){
        luminance=2.0*luminance*luminance;
    }else{
        luminance=1.0-2.0*(1.0-luminance)*(1.0-luminance);
    }
    return luminance;
}
void main(){
    vec3 center_color=texture2D(U_Texture,V_Texcoord.xy).rgb;
    float color_g_channel=Blur();
    float highPass = center_color.g - color_g_channel + 0.5;
    for(int i=0;i<5;i++){
        highPass=HardLight(highPass);
    }
    //dot(v1,v2)=v1.x*v2.x+v1.y*v2.y+v1.z*v2.z
    float gray_level = dot(center_color, vec3(0.299,0.587,0.114));
    float smooth_level=U_Setting.x;
    float alpha = pow(gray_level, smooth_level);
    vec3 smooth_color = center_color + (center_color-vec3(highPass))*alpha*0.1;
    //mix(v1,v2,a)=v1*a+v2*(1-a)
    gl_FragColor=vec4(mix(smooth_color, max(smooth_color, center_color), alpha), 1.0);
}