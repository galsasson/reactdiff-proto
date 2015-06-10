#version 150

in vec4 position;
in vec2 texcoord;

uniform mat4 modelViewProjectionMatrix;

// this is something we're creating for this shader
out vec2 varyingtexcoord;

void main(){
    
	//get our current vertex position so we can modify it
	vec4 pos = modelViewProjectionMatrix * position;

	varyingtexcoord = texcoord;
	//finally set the pos to be that actual position rendered
	gl_Position = pos;
}
