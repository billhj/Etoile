/*
getting error level of each pixel: blue= low  red = high
creating rainbow level for showing error level
*/

#version 150 compatibility



float rgbToLuminance(vec3 rgb)
{
	return (rgb.r * 0.2 + rgb.g * 0.7 + rgb.b * 0.1);
}


uniform float normalizeFactor;
uniform sampler2D input1;
uniform sampler2D input2;
uniform float errorFactor;

out vec4 FragColor[1];


void main (void) {

    ivec2 cordinate = ivec2(gl_FragCoord.xy);
    vec3 c1 = texelFetch(input1, cordinate, 0 ).xyz;
    vec3 c2 = texelFetch(input2, cordinate, 0 ).xyz;
    float dif = errorFactor * rgbToLuminance(c1 - c2);
    
    FragColor[0] = vec4 (max(dif,0) ,  max(- dif, 0), 0, 1);

}

