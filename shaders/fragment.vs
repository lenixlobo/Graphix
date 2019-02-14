#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D ourTexture;
//uniform vec4 ourColor;


void main(void)
{
	//FragColor = vec4(ourColor,1.0);
	FragColor = texture(ourTexture,TexCoord);
}
