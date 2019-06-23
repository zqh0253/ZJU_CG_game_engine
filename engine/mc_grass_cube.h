#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "texture.h"
#include "shader.h"
#include "bare_sprite_renderer.h"

#include "camera.h"
class mc_grass_cube
{
public:
	mc_grass_cube();
	~mc_grass_cube();
	void Draw(Camera* camera, GLfloat H, GLfloat W, glm::vec3 position, glm::vec3 size, GLfloat rotate, GLint type);
	static void initRenderData();
private:
	//Shader shader;
	GLuint quadVAO;
	const static GLint MAX_VERTICE_NUM;
	//static GLfloat farside[48], nearside[48], upside[48], downside[48], leftside[48], rightside[48];
	static std::vector<bare_sprite_renderer*> mc_renders[];
	static std::vector<GLchar*> mc_tex_color[];
};
