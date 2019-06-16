
#include "model_renderer.h"

ModelRenderer::ModelRenderer(Shader shader, const char * path):
model(path)
{
	this->shader = shader;
}

ModelRenderer::~ModelRenderer()
{

}

void ModelRenderer::Draw(Camera* camera, GLfloat H, GLfloat W, glm::vec3 position, glm::vec3 size, GLfloat rotate, glm::vec3 color)
{
	// Prepare transformations
	this->shader.Use();
	glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom), W / H, 0.1f, 100.0f);
	glm::mat4 view = camera->GetViewMatrix();
	this->shader.SetMatrix4("projection", projection);
	this->shader.SetMatrix4("view", view);

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, position);  
	model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 1.0f, 0.0f)); 
	model = glm::scale(model, size); // Last scale

	this->shader.SetMatrix4("model", model);

	this->model.Draw(this->shader);
}

void ModelRenderer::initRenderData()
{

}
