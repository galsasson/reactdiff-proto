#version 150
precision highp float;

uniform sampler2D tex0;
uniform vec2 tex0_size;

in vec2 varyingtexcoord;

out vec4 fragColor;

uniform float minColor;
uniform float maxColor;
//const float COLOR_MIN = 0.0, COLOR_MAX = 0.2;

void main(){
	//this is the fragment shader
	//this is where the pixel level drawing happens
	//gl_FragCoord gives us the x and y of the current pixel its drawing

	float v = (maxColor - texture(tex0, varyingtexcoord).g) / (maxColor - minColor);
	fragColor = vec4(v, v, v, 1.0);

//	fragColor = vec4(varyingtexcoord, 0.0, 1.0);
//	fragColor = texture(tex0, varyingtexcoord);
}
