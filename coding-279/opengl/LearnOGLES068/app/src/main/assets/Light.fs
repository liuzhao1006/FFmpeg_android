#version 300 es
#ifdef GL_ES
precision mediump float;
#endif
in vec4 V_Normal;
in vec4 V_WorldPos;
layout(location=0)out vec4 FragColor;
void main(){
    vec3 normal=normalize(V_Normal.xyz);
    vec3 L=vec3(1.0,1.0,0.0);
    L=normalize(L);
    vec3 ambient_color=vec3(0.05);
    float diffuse_light_intensity=max(0.0,dot(normal,L));
    vec3 diffuse_material=vec3(0.1,0.4,0.6);
    vec3 light_color=vec3(1.0);
    vec3 diffuse_color=diffuse_light_intensity*light_color*diffuse_material;
    vec3 specular_color=vec3(0.0);
    if(diffuse_light_intensity>0.0){
        vec3 reflect_dir=reflect(-L,normal);
        vec3 view_dir=normalize(vec3(0.0)-V_WorldPos.xyz);
        float shinness=128.0;
        specular_color=light_color*pow(max(0.0,dot(view_dir,reflect_dir)),shinness);
    }
    FragColor=vec4(ambient_color+diffuse_color+specular_color,1.0);
}