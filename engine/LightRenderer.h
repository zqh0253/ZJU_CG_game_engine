#pragma once
#ifndef LIGHT_RENDERER_H
#define LIGHT_RENDERER_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "texture.h"
#include "shader.h"
#include "camera.h"
#include "model.h"

class LightRenderer
{
public:
	LightRenderer(Shader shader, GLfloat vertices[]);
	~LightRenderer();
	void Draw(Camera* camera, GLfloat H, GLfloat W, glm::vec3 position);
private:
	Shader shader;
	GLuint quadVAO;
	void initRenderData(GLfloat vertices[]);
};

#endif