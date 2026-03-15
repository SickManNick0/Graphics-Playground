#version 330									
											
layout (location = 0) in vec3 pos;				
												
uniform mat4 model;			
uniform mat4 projection;
uniform float time;

out vec3 vCol;

void main()										
{												
	gl_Position = projection * model * vec4(pos.x + clamp(sin(time),0.2,1.0),pos.y,pos.z + clamp(sin(time),0.3,1.0),1.0f);	
	vCol = (clamp(pos, 0.0,1.0));
}