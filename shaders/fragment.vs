#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D ourTexture;
uniform sampler2D Texture2;

//uniform vec4 ourColor;


void main(void)
{
	//FragColor = vec4(ourColor,1.0);
	//FragColor = texture(ourTexture,TexCoord)*vec4(ourColor,1.0);
	FragColor = mix(texture(ourTexture,TexCoord),texture(Texture2,TexCoord),0.2);
}
