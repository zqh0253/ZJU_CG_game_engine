#include "skybox_renderer.h"



skybox_renderer::skybox_renderer(Shader shader, GLfloat vertices[])
{
	this->shader = shader;
	this->initRenderData(vertices);
}

skybox_renderer::~skybox_renderer()
{
}

void skybox_renderer::Draw(Camera * camera, GLuint id, GLfloat H, GLfloat W)
{
	glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
	this->shader.Use();
	glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom), W / H, 0.1f, 100.0f);
	glm::mat4 view = glm::mat4(glm::mat3(camera->GetViewMatrix())); // remove translation from the view matrix
	this->shader.SetMatrix4("view", view);
	this->shader.SetMatrix4("projection", projection);
	// skybox cube
	glBindVertexArray(this->quadVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, id);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS); // set depth function back to default
}

void skybox_renderer::initRenderData(GLfloat vertices[])
{
	GLfloat local_vertices[36 * 3];
	memcpy(local_vertices, vertices, 36 * 3 * sizeof(GLfloat));

	// Configure VAO/VBO
	GLuint VBO;
	glGenVertexArrays(1, &this->quadVAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(local_vertices), local_vertices, GL_STATIC_DRAW);

	glBindVertexArray(this->quadVAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
