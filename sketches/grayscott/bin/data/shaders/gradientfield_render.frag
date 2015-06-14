#version 150

uniform sampler2D tex0;
uniform vec4 mag;

in vec2 varyingtexcoord;

out vec4 fragColor;

void main(){

	vec4 c = texture(tex0, varyingtexcoord);
	fragColor = vec4(c.rgb * mag.rgb, 1.0);
}
