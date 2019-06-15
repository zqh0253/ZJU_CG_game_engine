
#ifndef MODEL_RENDERER_H
#define MODEL_RENDERER_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "texture.h"
#include "shader.h"
#include "camera.h"
#include "model.h"

class ModelRenderer
{
public:
	// Constructor (inits shaders/shapes)
	ModelRenderer(Shader shader, const char* path);
	// Destructor
	~ModelRenderer();
	// Renders a defined quad textured with given sprite
	void Draw(Camera* camera, GLfloat H, GLfloat W,  glm::vec3 position, glm::vec3 size = glm::vec3(1, 1, 1), GLfloat rotate = 0.0f, glm::vec3 color = glm::vec3(1.0f));
private:
	// Render state
	Shader shader;
	GLuint quadVAO;
	Model model;
	// Initializes 
	void initRenderData();
};

#endif

