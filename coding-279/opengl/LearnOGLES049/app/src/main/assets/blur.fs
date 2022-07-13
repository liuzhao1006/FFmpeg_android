#ifdef GL_ES
precision mediump float;
#endif
uniform sampler2D U_Texture;
uniform vec4 U_ImageSize;
varying vec4 V_Texcoord;
void main(){
    vec3 color=vec3(0.0);
    float radius_in_pixel=1.0;
    float radius_in_texcoord_s=radius_in_pixel/U_ImageSize.x;
    float radius_in_texcoord_t=radius_in_pixel/U_ImageSize.y;
    float weight[9];
    weight[0]=1.0;weight[1]=2.0;weight[2]=1.0;
    weight[3]=2.0;weight[4]=4.0;weight[5]=2.0;
    weight[6]=1.0;weight[7]=2.0;weight[8]=1.0;
    for(int y=-1;y<=1;y++){
        for(int x=-1;x<=1;x++){
            int weight_index=1+x+(y+1)*3;
            float texcoord_x=V_Texcoord.x+float(x)*radius_in_texcoord_s;
            float texcoord_y=V_Texcoord.y+float(y)*radius_in_texcoord_t;
            color+=texture2D(U_Texture,vec2(texcoord_x,texcoord_y)).rgb*weight[weight_index];
        }
    }
    color/=16.0;
    gl_FragColor=vec4(color,texture2D(U_Texture,V_Texcoord.xy).a);
}