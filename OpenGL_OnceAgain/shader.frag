#version 330									
												
out vec4 color;									
in vec3 vCol;			
void main()										
{												
	color =  vec4(vCol,1.0f);
}		