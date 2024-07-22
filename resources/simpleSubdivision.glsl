#Vertex
#version 330 core
layout (location = 0) in vec3 aPos;
uniform mat4 modelviewV;
void main(){
    gl_Position =  modelviewV * vec4(aPos, 1.0);
}				  
				  

#Geometry
#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

uniform float time;
uniform vec4 internal;
uniform vec4 grid;
uniform int animate;

uniform mat4 modelviewG;

in VS_OUT{
    vec3 color;
}gs_in[];
out vec4 aColor;
out vec2 Tex_Coords;
out vec2 Tex_Coords_2;

vec4 displace(vec4 x,vec3 y){
    if(animate==1){  
        return x + vec4(y,0.0)*(1.0-cos(time))*0.2 ;
    }return x;
}
vec3 GetNormal(vec4 x,vec4 y,vec4 z){return normalize(( vec3(z) + vec3(y) + vec3(x))/3.0); }
vec4 halfDist(vec4 x, vec4 y){   return normalize(x+y);}
mat4 rotationMatrix(vec3 axis, float angle)
{
    // axis = normalize(axis);
    float s = sin(angle);
    float c = cos(angle);
    float oc = 1.0 - c;
    return mat4(oc * axis.x * axis.x + c,           oc * axis.x * axis.y - axis.z * s,  oc * axis.z * axis.x + axis.y * s,  0.0,
                oc * axis.x * axis.y + axis.z * s,  oc * axis.y * axis.y + c,           oc * axis.y * axis.z - axis.x * s,  0.0,
                oc * axis.z * axis.x - axis.y * s,  oc * axis.y * axis.z + axis.x * s,  oc * axis.z * axis.z + c,           0.0,
                0.0,                                0.0,                                0.0,                                1.0);
}

void main() {    
	vec3 normal = GetNormal(gl_in[0].gl_Position,gl_in[1].gl_Position,gl_in[2].gl_Position) ;
    // mat4 modelview= proj*view*model;
    gl_Position = modelviewG * displace(gl_in[0].gl_Position,normal); Tex_Coords=vec2(0.0,0.0); aColor = grid; EmitVertex();   
    gl_Position = modelviewG * displace(gl_in[1].gl_Position,normal); Tex_Coords=vec2(0.5,1.0); aColor = grid; EmitVertex();   
    gl_Position = modelviewG * displace(gl_in[2].gl_Position,normal); Tex_Coords=vec2(1.0,0.0); aColor = grid; EmitVertex();  
	EndPrimitive();
	// vec4 h1 = halfDist(gl_in[1].gl_Position,gl_in[2].gl_Position);
	// vec4 h2 = halfDist(gl_in[2].gl_Position,gl_in[0].gl_Position);
	// vec4 h3 = halfDist(gl_in[0].gl_Position,gl_in[1].gl_Position);
	// vec3 hnormal=GetNormal(h1,h2,h3);

    // gl_Position = /*modelview*/displace((gl_in[0].gl_Position+gl_in[1].gl_Position)/2.0,normal); Tex_Coords_2=(vec2(1.0,0.0)+vec2(0.5,1.0))/2.0; aColor=grid; EmitVertex();
    // gl_Position = /*modelview*/displace((gl_in[1].gl_Position+gl_in[2].gl_Position)/2.0,normal); Tex_Coords_2=(vec2(0.0,0.0)+vec2(1.0,0.0))/2.0; aColor=grid; EmitVertex();
    // gl_Position = /*modelview*/displace((gl_in[2].gl_Position+gl_in[0].gl_Position)/2.0,normal); Tex_Coords_2=(vec2(0.0,0.0)+vec2(0.5,1.0))/2.0; aColor=grid; EmitVertex();
    // EndPrimitive();
}

#Fragment
#version 330 core
out vec4 FragColor;
in vec4 aColor;
uniform sampler2D text1;
uniform sampler2D text2;
in vec2 Tex_Coords;
in vec2 Tex_Coords_2;
uniform int textureOn;
void main()
{
    if(textureOn==1){
        FragColor = texture(text1,Tex_Coords);//mix(texture(text1,Tex_Coords),texture(text2,Tex_Coords_2),0.3);
    }else{
        FragColor = aColor;
    }
}



