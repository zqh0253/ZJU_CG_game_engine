
#ifndef SPRITE_RENDERER_H
#define SPRITE_RENDERER_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "texture.h"
#include "shader.h"
#include "camera.h"

class SpriteRenderer
{
public:
	// Constructor (inits shaders/shapes)
	SpriteRenderer(Shader shader, GLfloat vertices[], GLint length);
	// Destructor
	~SpriteRenderer();
	// Renders a defined quad textured with given sprite
	void DrawSprite(Camera* camera, GLfloat H, GLfloat W, Texture2D diffuse_texture, Texture2D specular_texture, Texture2D normal_texture, Texture2D high_texture, glm::vec3 position, glm::vec3 size = glm::vec3(1, 1, 1), GLfloat rotate = 0.0f, glm::vec3 color = glm::vec3(1.0f));
private:
	// Render state
	Shader shader;
	GLuint quadVAO;
	// Initializes and configures the quad's buffer and vertex attributes
	void initRenderData(GLfloat vertices[], GLint length);
	const static GLint MAX_VERTICE_NUM;
};

#endif

