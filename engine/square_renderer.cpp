#include "square_renderer.h"

square_renderer::square_renderer(Shader shader, GLfloat vertices[])
{
	this->shader = shader;
	this->initRenderData(vertices);
}

square_renderer::~square_renderer()
{
}

void square_renderer::Draw(Camera * camera, GLfloat H, GLfloat W)
{
	this->shader.Use();
	glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom), W / H, 0.1f, 100.0f);
	glm::mat4 view = camera->GetViewMatrix();
	this->shader.SetMatrix4("projection", projection);
	this->shader.SetMatrix4("view", view);
	glm::mat4 model = glm::mat4(1.0f);
	this->shader.SetMatrix4("model", model);

	glBindVertexArray(this->quadVAO);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBindVertexArray(0);
}

void square_renderer::initRenderData(GLfloat vertices[])
{
	GLfloat local_vertices[6 * 3];
	memcpy(local_vertices, vertices, 6 * 3 * sizeof(GLfloat));

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
