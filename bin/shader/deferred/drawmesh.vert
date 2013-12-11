#version 400 compatibility
#extension GL_ARB_uniform_buffer_object : enable

out vec3 N;
out vec2 TextureCoord;
out vec3 vertexPos;

in vec3 In_Normal;
in vec3 In_Vertex;
in vec2 In_TextureCoord;



uniform mat4 In_ViewMatrix;
uniform mat4 In_ProjectionMatrix;
uniform mat4 In_WorldMatrix;
uniform mat4 In_NormalMatrix;
uniform mat4 In_ViewProjectionMatrix;

uniform In_Transform
{
	mat4 _ProjectionMatrix;
	mat4 _ViewMatrix;
	mat4 _ViewProjectionMatrix;
	mat4 _NormalMatrix;
}In_transform;

void main() {
   
  
    N = In_Normal;
	vertexPos = In_Vertex;
	TextureCoord = In_TextureCoord;
	
    gl_Position =  In_ProjectionMatrix * In_ViewMatrix * In_WorldMatrix * vec4(In_Vertex, 1.0);
	//gl_Position =  In_transform._ProjectionMatrix * (In_transform._ViewMatrix * vec4(In_Vertex, 1.0));
	
}

