#version 330 compatibility
#extension GL_EXT_gpu_shader4: enable


uniform sampler2D normalIDMap;
uniform sampler2D worldPosMap;
uniform sampler2D diffuseComponent;
uniform sampler2D specularComponent;
uniform sampler2D ambientComponent;

uniform mat4 In_ViewMatrix;

uniform In_Transform
{
	mat4 _ProjectionMatrix;
	mat4 _ViewMatrix;
	mat4 _ViewProjectionMatrix;
	mat4 _NormalMatrix;
}In_transform;

uniform mat4 In_WorldMatrix;
out vec4 FragColor[6];

void main (void) {
	
	vec4 normalID = texture(normalIDMap, gl_TexCoord[0].st);
	float id = normalID.w;
	if(id < 0.2)
		discard;
	
	vec3 normal = normalID.xyz;


	vec4 worldPos = texture(worldPosMap, gl_TexCoord[0].st);
	vec4 diffuseCp = texture(diffuseComponent, gl_TexCoord[0].st);
	vec4 specularCp = texture(specularComponent, gl_TexCoord[0].st);
	vec4 ambientCp = texture(ambientComponent, gl_TexCoord[0].st);
	
	vec4 modelViewPos = In_ViewMatrix *In_WorldMatrix* worldPos;
	
	vec4 diffuseMap = vec4(0.0);
    vec4 specularMap = vec4(0.0);
    
    vec3  lv = gl_LightSource[0].position.xyz - modelViewPos.xyz;
    vec3  l = normalize(lv);
    float diffuse = max(dot (l, normal), 0.0);
    
    vec3 v = normalize( -modelViewPos.xyz);
    vec3 h = normalize(l + v);
    float spec = max(dot(normal, h), 0.0);
    spec = pow(spec, specularCp.w);
    spec = max(0.0, spec) * ( length(lv) + 1.0);
    
    diffuseMap += diffuse *  gl_LightSource[0].diffuse ;
    specularMap += spec  * gl_LightSource[0].specular ;
	
	diffuseMap *= diffuseCp;
	specularMap *= specularCp;
	vec4 ambientMap = ambientCp * gl_LightSource[0].ambient;
	
	vec4 all = diffuseMap + specularMap + ambientMap;
	FragColor[0] = vec4(all);
	FragColor[1] = vec4(specularMap);
	FragColor[2] = ambientMap;
	FragColor[3] = vec4(modelViewPos);
	
	
	/*vec3 pos = modelViewPos.xyz;
	for(int i = -1; i <= 1; ++i)
	{
		float dis = texture(diffuseComponent, gl_TexCoord[0].st+ vec2(1.0/800,0)).x;
		vec3 posN = (In_ViewMatrix * texture(worldPosMap, gl_TexCoord[0].st + vec2(1.0/800,0))).xyz;
		vec3 normalN = texture(normalIDMap, gl_TexCoord[0].st + vec2(1.0/800,0)).xyz;
		
	}*/
	
	
}

