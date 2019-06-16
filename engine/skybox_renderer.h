#pragma once


#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "texture.h"
#include "shader.h"
#include "camera.h"
#include "model.h"

class skybox_renderer
{
public:
	skybox_renderer(Shader shader, GLfloat vertices[]);
	~skybox_renderer();
	void Draw(Camera* camera, GLuint id, GLfloat H, GLfloat W);
private:
	Shader shader;
	GLuint quadVAO;
	void initRenderData(GLfloat vertices[]);
};

