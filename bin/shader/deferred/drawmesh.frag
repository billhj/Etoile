#version 400 compatibility
#extension GL_EXT_texture_array : enable


in vec3 N;
in vec2 TextureCoord;
in vec3 vertexPos;

uniform In_Transform
{
	mat4 _ProjectionMatrix;
	mat4 _ViewMatrix;
	mat4 _ViewProjectionMatrix;
	mat4 _NormalMatrix;
}In_transform;

uniform mat4 In_WorldMatrix;

uniform sampler2D diffuseTexture;
uniform sampler2D bumpMap;
uniform float maskid; 

out vec4 FragColor[6];



void main (void) {
   
    
	
  	vec4 worldPos = /*In_WorldMatrix */ vec4(vertexPos, 1.0);
	vec3 n = normalize(gl_NormalMatrix * N);
	
	/*if(n.z < -0.4)
		discard;*/
	vec4 tex = texture(diffuseTexture, TextureCoord);
	FragColor[0] = vec4(n, 0.5);
	FragColor[1] = vec4(worldPos);
	
	FragColor[2] = gl_FrontMaterial.diffuse * tex;
	
	FragColor[3] = vec4(gl_FrontMaterial.specular.xyz, gl_FrontMaterial.shininess);
	FragColor[4] = vec4(gl_FrontMaterial.ambient * tex);
	vec4 worldNormal = /*In_WorldMatrix */ vec4(N,0);
	FragColor[5] = vec4(worldNormal.xyz,  1);
}



