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
    float weight[9];
    weight[0]=1.0;weight[1]=2.0;weight[2]=1.0;
    weight[3]=2.0;weight[4]=4.0;weight[5]=2.0;
    weight[6]=1.0;weight[7]=2.0;weight[8]=1.0;
    float color_g_channel=0.0;
    for(int y=-1;y<=1;y++){
        for(int x=-1;x<=1;x++){
            int weight_index=1+x+(y+1)*3;
            float texcoord_x=V_Texcoord.x+float(x)*radius_in_texcoord_s;
            float texcoord_y=V_Texcoord.y+float(y)*radius_in_texcoord_t;
            color_g_channel+=texture2D(U_Texture,vec2(texcoord_x,texcoord_y)).g*weight[weight_index];
        }
    }
    return color_g_channel/16.0;
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