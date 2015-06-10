#version 150

uniform sampler2D tex0;

in vec2 varyingtexcoord;

out vec4 fragColor;

void main(){

	vec4 c = texture(tex0, varyingtexcoord);
	fragColor = vec4(c.rg*10.0, c.b, 1.0);
}
