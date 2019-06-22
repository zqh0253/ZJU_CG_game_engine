#pragma once
#ifndef _BARE_SPRITE_RENDERER_H
#define _BARE_SPRITE_RENDERER_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "texture.h"
#include "shader.h"
#include "camera.h"

//This class is used to render the object with color_texture ONLY. (No spec/norm/... textures) 
class bare_sprite_renderer
{
public:
	bare_sprite_renderer(Shader shader, GLfloat vertices[], GLint length);
	~bare_sprite_renderer();
	void Draw(Camera* camera, GLfloat H, GLfloat W, Texture2D texture, glm::vec3 position, glm::vec3 size = glm::vec3(1, 1, 1), GLfloat rotate = 0.0f, glm::vec3 color = glm::vec3(1.0f));
private:
	Shader shader;
	GLuint quadVAO;
	void initRenderData(GLfloat vertices[], GLint length);
	const static GLint MAX_VERTICE_NUM;

};

#endif

