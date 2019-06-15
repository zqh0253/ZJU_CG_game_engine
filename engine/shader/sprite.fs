#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D normal;
	sampler2D height;
  //  float     shininess;
}; 

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform Material material;
void main()
{
	vec3 ambient = lightColor * vec3(texture(material.diffuse, TexCoord));

	vec3 normal = texture(material.normal, TexCoord).rgb;
	vec3 norm = normalize(normal);

	vec3 height = (normalize(texture(material.height, TexCoord)*2-1.0)*1).rgb;
	vec3 rFragPos = FragPos;
	//rFragPos.y += height.y;
	//height_map to add

	vec3 lightDir = normalize(lightPos - rFragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor * texture(material.diffuse, TexCoord).rgb;  

	vec3 viewDir = normalize(viewPos - rFragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = 0.1 * spec * lightColor * texture(material.specular, TexCoord).rgb;

	vec3 result = (ambient + diffuse + specular);
    FragColor = vec4(result, 1.0);
}