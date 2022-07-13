#ifdef GL_ES
precision mediump float;
#endif
uniform sampler2D U_Texture;
uniform vec4 U_ImageSize;
uniform vec4 U_MSKRange;
uniform vec4 U_MSKSetting;
varying vec4 V_Texcoord;
void main(){
    vec4 color=vec4(0.0);
    float msk_size_in_pixel=U_MSKSetting.x;
    float msk_size_in_texcoord_s=msk_size_in_pixel/U_ImageSize.x;
    float msk_size_in_texcoord_t=msk_size_in_pixel/U_ImageSize.y;

    float msk_start_pos_x_in_pixel=U_MSKRange.x;
    float msk_start_pos_y_in_pixel=U_MSKRange.y;
    float msk_end_pos_x_in_pixel=msk_start_pos_x_in_pixel+U_MSKRange.z;
    float msk_end_pos_y_in_pixel=msk_start_pos_y_in_pixel+U_MSKRange.w;

    float msk_start_pos_x_texcoord_s=msk_start_pos_x_in_pixel/U_ImageSize.x;
    float msk_start_pos_y_texcoord_t=msk_start_pos_y_in_pixel/U_ImageSize.y;
    float msk_end_pos_x_texcoord_s=msk_end_pos_x_in_pixel/U_ImageSize.x;
    float msk_end_pos_y_texcoord_t=msk_end_pos_y_in_pixel/U_ImageSize.y;

    if(V_Texcoord.x>=msk_start_pos_x_texcoord_s&&V_Texcoord.x<=msk_end_pos_x_texcoord_s&&
        V_Texcoord.y>=msk_start_pos_y_texcoord_t&&V_Texcoord.y<=msk_end_pos_y_texcoord_t){
        float msk_quad_index_x=floor((V_Texcoord.x-msk_start_pos_x_texcoord_s)/msk_size_in_texcoord_s);
        float msk_quad_index_y=floor((V_Texcoord.y-msk_start_pos_y_texcoord_t)/msk_size_in_texcoord_t);
        float texcoord_x=msk_start_pos_x_texcoord_s+msk_quad_index_x*msk_size_in_texcoord_s;
        float texcoord_y=msk_start_pos_y_texcoord_t+msk_quad_index_y*msk_size_in_texcoord_t;
        color=texture2D(U_Texture,vec2(texcoord_x,texcoord_y));
    }else{
        color=texture2D(U_Texture,V_Texcoord.xy);
    }
    gl_FragColor=color;
}