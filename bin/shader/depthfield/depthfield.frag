#extension GL_EXT_gpu_shader4: enable
uniform sampler2D mvPosMap;
uniform sampler2D inputMap;

uniform int SamplesNumber; //SamplesNumber
uniform vec4 PatternSamples[500]; //PatternSamples
uniform int PatternSize;  //PatternSize
uniform float focDepth = 0.015;
uniform float range = 0.014;
uniform float radius = 3;
const float width = 800.0;
const float height = 600.0;
const float inverWidth =1.0/width;
const float inverHeight = 1.0/height;

uniform int addDepthField;
uniform mat4 In_ProjectionMatrix;

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

float square (float x) {
	return x*x;
}

float wendland(float h, float t){
	float thinvers = t/h;
	return (float(t < h) * square (square(1.0-thinvers))*(4.0*thinvers+1.0));
}

void main (void) {
		vec4 final  = vec4(0.0);
		vec4 p = texture2D(mvPosMap, gl_TexCoord[0].st);
		float dis  = abs(-p.z - focDepth );
        dis = max((dis - range)/(dis), 0.0);
		int k = 1 * (int(gl_FragCoord.x)  % int(PatternSize)) + int(PatternSize) * (int (gl_FragCoord.y) % int (PatternSize));
		
		for (int i = 0; i < SamplesNumber; ++i) {
			int patternIndex = (i + k * int (SamplesNumber))%500;	
			vec2 offset = PatternSamples[patternIndex].xy * radius * dis*dis*vec2(inverWidth, inverHeight);
			vec4  input = texture2D(inputMap, gl_TexCoord[0].st + offset);
			final += input;
		}
		vec4  input = texture2D(inputMap, gl_TexCoord[0].st);
		final /= float(SamplesNumber);
        gl_FragColor = vec4(final); 
}

