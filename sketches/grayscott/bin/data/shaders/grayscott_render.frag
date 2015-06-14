#version 150
precision highp float;

uniform sampler2D tex0;
uniform vec2 tex0_size;

in vec2 varyingtexcoord;

out vec4 fragColor;

uniform vec4 minColor;
uniform vec4 maxColor;
uniform float borderWidth;
uniform float softness;
uniform float minVal;
uniform float maxVal;

uniform float roundEdge1;
uniform float roundEdge2;

float map(float value, float inputMin, float inputMax, float outputMin, float outputMax, bool clamp)
{
		float outVal = ((value - inputMin) / (inputMax - inputMin) * (outputMax - outputMin) + outputMin);

		if( clamp ){
			if(outputMax < outputMin){
				if( outVal < outputMax )outVal = outputMax;
				else if( outVal > outputMin )outVal = outputMin;
			}else{
				if( outVal > outputMax )outVal = outputMax;
				else if( outVal < outputMin )outVal = outputMin;
			}
		}
		return outVal;
}

// square cell borders
vec4 cellBordersSquare(vec4 color)
{
	float edge0 = borderWidth/2;
	float edge1 = 1.0 - borderWidth/2;
	vec2 t = vec2(fract(varyingtexcoord.x * tex0_size.x),
				  fract(varyingtexcoord.y * tex0_size.y));
	vec2 a = smoothstep(edge0, edge0+softness, t) - smoothstep(edge1-softness, edge1, t);
	return vec4(color.r*a.x*a.y, color.g*a.x*a.y, color.b*a.x*a.y, 1.0);
}

vec4 cellBordersRound(vec4 color)
{
	vec2 t = vec2(fract(varyingtexcoord.x * tex0_size.x),
				  fract(varyingtexcoord.y * tex0_size.y));
	float r = length(abs(t-vec2(0.5, 0.5)));
	float a = smoothstep(roundEdge1, roundEdge2, r);
	return vec4(a*color.r, a*color.g, a*color.b, 1.0);
}

void main(){

	float val = map(texture(tex0, varyingtexcoord).g, minVal, maxVal, 0.0, 1.0, false);
	vec4 v = minColor + val * (maxColor-minColor);

//	fragColor = cellBordersSquare(v);
	fragColor = cellBordersRound(v);


//	fragColor = vec4(varyingtexcoord, 0.0, 1.0);
//	fragColor = texture(tex0, varyingtexcoord);
}
