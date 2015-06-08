#version 150
precision highp float;

uniform sampler2D tex0;
uniform vec2 tex0_size;
uniform vec4 globalColor;

in vec2 varyingtexcoord;

out vec4 fragColor;

uniform float feedRate;
uniform float killRate;
uniform float aDiffRate;
uniform float bDiffRate;
uniform float TIMESTEP;

vec2 getDefaultLaplacian(vec2 p)
{
	vec2 n = p + vec2(0.0, 1.0) / tex0_size,
	e = p + vec2(1.0, 0.0) / tex0_size,
	s = p + vec2(0.0, -1.0) / tex0_size,
	w = p + vec2(-1.0, 0.0) / tex0_size;

	vec2 val = texture(tex0, p).xy,
	lap = texture(tex0, n).xy
	+ texture(tex0, e).xy
	+ texture(tex0, s).xy
	+ texture(tex0, w).xy
	- 4.0 * val;

	return lap;
}


vec2 getPatricioLaplacian(vec2 p)
{
	float kernel[9];
	vec2 offset[9];

	kernel[0] = 0.707106781;
	kernel[1] = 1.0;
	kernel[2] = 0.707106781;
	kernel[3] = 1.0;
	kernel[4] = -6.82842712;
	kernel[5] = 1.0;
	kernel[6] = 0.707106781;
	kernel[7] = 1.0;
	kernel[8] = 0.707106781;

	offset[0] = vec2( -1.0, -1.0) / tex0_size;
	offset[1] = vec2(  0.0, -1.0) / tex0_size;
	offset[2] = vec2(  1.0, -1.0) / tex0_size;

	offset[3] = vec2( -1.0, 0.0) / tex0_size;
	offset[4] = vec2(  0.0, 0.0) / tex0_size;
	offset[5] = vec2(  1.0, 0.0) / tex0_size;

	offset[6] = vec2( -1.0, 1.0) / tex0_size;
	offset[7] = vec2(  0.0, 1.0) / tex0_size;
	offset[8] = vec2(  1.0, 1.0) / tex0_size;

	vec2 lap = vec2( 0.0, 0.0 );

	for( int i=0; i < 9; i++ ){
		vec2 tmp = texture(tex0, p + offset[i] ).rg;
		lap += tmp * kernel[i];
	}

	return lap;
}

vec2 getGalLaplacian(vec2 p)
{

	vec2 n = vec2(0.0, 1.0) / tex0_size;
	vec2 e = vec2(1.0, 0.0) / tex0_size;
	vec2 s = vec2(0.0, -1.0) / tex0_size;
	vec2 w = vec2(-1.0, 0.0) / tex0_size;

	vec2 val = texture(tex0, p).rg;

	vec2 lap = -val
	+ texture(tex0, p+n).rg * vec2(0.2)
	+ texture(tex0, p+e).rg * vec2(0.2)
	+ texture(tex0, p+s).rg * vec2(0.2)
	+ texture(tex0, p+w).rg * vec2(0.2)
	+ texture(tex0, p+n+e).rg * vec2(0.05)
	+ texture(tex0, p+e+s).rg * vec2(0.05)
	+ texture(tex0, p+s+w).rg * vec2(0.05)
	+ texture(tex0, p+w+n).rg * vec2(0.05);

	return lap;
}

vec2 getGalLaplacian2(vec2 p)
{

	vec2 adjCoeff = vec2(0.2);
	vec2 diagCoeff = vec2(0.05);

	vec2 n = vec2(0.0, 1.0) / tex0_size;
	vec2 e = vec2(1.0, 0.0) / tex0_size;
	vec2 s = vec2(0.0, -1.0) / tex0_size;
	vec2 w = vec2(-1.0, 0.0) / tex0_size;

	vec2 val = texture(tex0, p).rg;

	vec2 lap = -val
	+ texture(tex0, p+n).rg * adjCoeff
	+ texture(tex0, p+e).rg * adjCoeff
	+ texture(tex0, p+s).rg * adjCoeff
	+ texture(tex0, p+w).rg * adjCoeff
	+ texture(tex0, p+n+e).rg * diagCoeff
	+ texture(tex0, p+e+s).rg * diagCoeff
	+ texture(tex0, p+s+w).rg * diagCoeff
	+ texture(tex0, p+w+n).rg * diagCoeff;

	return lap;
}

void main(){
	vec2 p = varyingtexcoord.xy;

	vec2 val = texture(tex0, p).rg;

//	vec2 laplacian = getPatricioLaplacian(p);
	vec2 laplacian = getGalLaplacian2(p);
//	vec2 laplacian = getDefaultLaplacian(p);

	vec2 delta = vec2(aDiffRate * laplacian.x - val.x*val.y*val.y + feedRate * (1.0-val.x),
					  bDiffRate * laplacian.y + val.x*val.y*val.y - (killRate+feedRate) * val.y);

	fragColor = vec4(val + delta * TIMESTEP, 0.0, 1.0);

//	fragColor = texture(tex0, varyingtexcoord.xy);
//	fragColor = vec4(varyingtexcoord.x, varyingtexcoord.y, 0.0, 1.0);
}
