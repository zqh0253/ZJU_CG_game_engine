#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoord;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 models[21*21*10];

void main()
{
	mat4 model = models[gl_InstanceID];
    gl_Position = projection * view * model * vec4(aPos, 1.0); 
	FragPos = vec3(model*vec4(aPos, 1.0));
    Normal = aNormal;
	TexCoord = aTexCoord;
}