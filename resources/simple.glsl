#Vertex
#version 330 core
layout (location = 0) in vec3 aPos;
uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;
void main()
{
    gl_Position =  proj * view * model * vec4(aPos, 1.0);
}				  

#Geometry
#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;
uniform float time;
vec4 displace(vec4 x,vec3 y){return x;}
vec3 GetNormal(vec4 x,vec4 y,vec4 z){ vec3 a=( vec3(z) + vec3(y) + vec3(x))/3.0;return normalize(a); }
vec4 halfDist(vec4 x, vec4 y){ return normalize(x+y);}
out vec4 aColor;
uniform vec4 internal;
uniform vec4 grid;
void main() {    
	vec3 normal = GetNormal(gl_in[0].gl_Position,gl_in[1].gl_Position,gl_in[2].gl_Position) ;
	gl_Position = displace(gl_in[0].gl_Position,normal);  aColor=grid ; EmitVertex();   
    gl_Position = displace(gl_in[1].gl_Position,normal);  aColor=grid ; EmitVertex();   
    gl_Position = displace(gl_in[2].gl_Position,normal);  aColor=grid ; EmitVertex();  
	EndPrimitive();
}


#Fragment
#version 330 core
out vec4 FragColor;
in vec4 aColor;
// uniform sampler2D text1;
void main()
{
    FragColor = aColor;
}



