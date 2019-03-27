#version 330 core

out vec4 FragColor;

struct Material{
	sampler2D diffuse;
	vec3 specular;
	float shininess;	
};

struct	Light{
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 viewPos;

uniform Material mater;
uniform Light lit;

void main(){
	//ambient
	vec3 ambient = lit.ambient * texture(mater.diffuse,TexCoords).rgb;

	//diffuse
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lit.position - FragPos);
	float diff = max(dot(norm,lightDir),0.0);
	vec3 diffuse = lit.diffuse * diff*texture(mater.diffuse,TexCoords).rgb;

	//specular
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir,norm);
	float spec = pow(max(dot(viewDir,reflectDir),0.0),mater.shininess);
	vec3 specular = lit.specular*(spec*mater.specular);

	vec3 result = diffuse + ambient + specular;
	FragColor = vec4(result,1.0);


}