#version 400 compatibility
uniform sampler2D input;
uniform float luminanceMax;
uniform float gamma;

void main (void) {

	vec4  texel = texture2D(input, gl_TexCoord[0].st);
	float luminance = (texel.g +  texel.r +  texel.b)/3.0;
	float scaledLuminance = pow(luminance / luminanceMax, 1.0 / gamma);
	float ratio = scaledLuminance / luminance;
    gl_FragColor = vec4( ratio * texel.rgb , 1);
	//gl_FragColor = vec4(texel);
}
