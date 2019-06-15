#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "texture.h"
#include "shader.h"
#include "camera.h"
#include "model.h"

class square_renderer
{
public:
	square_renderer(Shader shader, GLfloat vertices[]);
	~square_renderer();
	void Draw(Camera* camera, GLfloat H, GLfloat W);
private:
	Shader shader;
	GLuint quadVAO;
	void initRenderData(GLfloat vertices[]);
};