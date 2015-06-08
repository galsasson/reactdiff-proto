#version 150
precision highp float;

uniform sampler2D tex0;
uniform vec2 tex0_size;

in vec2 varyingtexcoord;

out vec4 fragColor;

uniform float minColor;
uniform float maxColor;
uniform float borderWidth;
uniform float softness;
//const float COLOR_MIN = 0.0, COLOR_MAX = 0.2;

void main(){
	//this is the fragment shader
	//this is where the pixel level drawing happens
	//gl_FragCoord gives us the x and y of the current pixel its drawing

	float v = (maxColor - texture(tex0, varyingtexcoord).g) / (maxColor - minColor);
	fragColor = vec4(v, v, v, 1.0);

//	// cell borders
//	if (fract(varyingtexcoord.x * tex0_size.x) < borderWidth ||
//		fract(varyingtexcoord.y * tex0_size.y) < borderWidth) {
//		fragColor = vec4(0.0, 0.0, 0.0, 1.0);
//	}

	// cell borders
	float edge0 = borderWidth/2;
	float edge1 = 1.0 - borderWidth/2;
	vec2 t = vec2(fract(varyingtexcoord.x * tex0_size.x),
				  fract(varyingtexcoord.y * tex0_size.y));
	vec2 a = smoothstep(edge0, edge0+softness, t) - smoothstep(edge1-softness, edge1, t);
	fragColor = vec4(v*a.x*a.y, v*a.x*a.y, v*a.x*a.y, 1.0);
//	}

//	fragColor = vec4(varyingtexcoord, 0.0, 1.0);
//	fragColor = texture(tex0, varyingtexcoord);
}
