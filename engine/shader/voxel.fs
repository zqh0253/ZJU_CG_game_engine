#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 color;
void main()
{
	vec3 ambient = lightColor * vec3(color);

	vec3 normal =Normal;
	vec3 norm = normalize(normal);

	vec3 rFragPos = FragPos;

	vec3 lightDir = normalize(lightPos - rFragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor*0.5;  

	vec3 viewDir = normalize(viewPos - rFragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	//vec3 specular = 0.1 * spec * lightColor * texture(material.specular, TexCoord).rgb;

	//vec3 result = (ambient + diffuse + specular);
    FragColor = vec4(ambient + diffuse, 1.0);
}