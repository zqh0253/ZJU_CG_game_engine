#include "bare_sprite_renderer.h"

const GLint bare_sprite_renderer::MAX_VERTICE_NUM = 100;

bare_sprite_renderer::bare_sprite_renderer(Shader shader, GLfloat vertices[], GLint length)
{
	this->shader = shader;
	this->initRenderData(vertices, length);
}


bare_sprite_renderer::~bare_sprite_renderer()
{
}

void bare_sprite_renderer::Draw(Camera * camera, GLfloat H, GLfloat W, Texture2D texture, glm::vec3 position, glm::vec3 size, GLfloat rotate, glm::vec3 color)
{
	this->shader.Use();
	glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom), W / H, 0.1f, 100.0f);
	glm::mat4 view = camera->GetViewMatrix();
	this->shader.SetMatrix4("projection", projection);
	this->shader.SetMatrix4("view", view);

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, position);  // First translate (transformations are: scale happens first, then rotation and then finall translation happens; reversed order)

	model = glm::rotate(model, rotate, glm::vec3(0.0f, 0.0f, 1.0f)); // Then rotate

	model = glm::scale(model, size); // Last scale

	this->shader.SetMatrix4("model", model);
	this->shader.SetVector3f("lightColor", 1.0f, 1.0f, 1.0f);
	this->shader.SetVector3f("lightPos", -2.5, 0.0, -1.5);
	this->shader.SetVector3f("viewPos", camera->Position);
	this->shader.SetInteger("material.diffuse", 0);
	glActiveTexture(GL_TEXTURE0);
	texture.Bind();
	
	glBindVertexArray(this->quadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}

void bare_sprite_renderer::initRenderData(GLfloat vertices[], GLint length)
{
	GLfloat local_vertices[MAX_VERTICE_NUM];
	memcpy(local_vertices, vertices, length * sizeof(GLfloat));

	// Configure VAO/VBO
	GLuint VBO;
	glGenVertexArrays(1, &this->quadVAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(local_vertices), local_vertices, GL_STATIC_DRAW);

	glBindVertexArray(this->quadVAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
