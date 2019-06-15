
#include <vector>

#include "game.h"
#include "camera.h"
#include "resource_manager.h"
#include "sprite_renderer.h"

std::vector<GLchar*> tex_color;
std::vector<GLchar*> tex_spec;
std::vector<GLchar*> tex_norm, tex_high;
std::vector<SpriteRenderer*>  Renderer;

Game::Game(GLuint width, GLuint height)
	: State(GAME_ACTIVE), Keys(), Width(width), Height(height), firstMouse(true)
{

}

Game::~Game()
{

}

void Game::Init()
{
	GLfloat far_wall_vertices[] = {
		// positions          // normals           // texture coords
		//far
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  3.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  3.0f,  3.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  3.0f,  3.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  3.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
	};
	GLfloat near_wall_vertices[] = {
		//near
		-0.5f, -0.5f,  3.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  3.5f,  0.0f,  0.0f,  1.0f,  3.0f,  0.0f,
		0.5f,  0.5f,  3.5f,  0.0f,  0.0f,  1.0f,  3.0f,  3.0f,
		0.5f,  0.5f,  3.5f,  0.0f,  0.0f,  1.0f,  3.0f,  3.0f,
		-0.5f,  0.5f,  3.5f,  0.0f,  0.0f,  1.0f,  0.0f,  3.0f,
		-0.5f, -0.5f,  3.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
	};
	GLfloat left_wall_vertices[] = {
		//left
		-0.5f,  0.5f,  3.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  3.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  3.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  3.0f,  1.0f,
		-0.5f, -0.5f,  3.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f,  0.5f,  3.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
	};
	GLfloat right_wall_vertices[] = {
		//right
		0.5f,  0.5f,  3.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  4.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  4.0f,  1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  4.0f,  1.0f,
		0.5f, -0.5f,  3.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  3.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
	};
	GLfloat down_wall_vertices[] = {
		//down
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  6.0f,  1.0f,
		0.5f, -0.5f,  3.5f,  0.0f, -1.0f,  0.0f,  6.0f,  0.0f,
		0.5f, -0.5f,  3.5f,  0.0f, -1.0f,  0.0f,  6.0f,  0.0f,
		-0.5f, -0.5f,  3.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
	};
	GLfloat up_wall_vertices[] = {
		//up
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  3.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  5.0f,  3.0f,
		0.5f,  0.5f,  3.5f,  0.0f,  1.0f,  0.0f,  5.0f,  0.0f,
		0.5f,  0.5f,  3.5f,  0.0f,  1.0f,  0.0f,  5.0f,  0.0f,
		-0.5f,  0.5f,  3.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  3.0f
	};

	// Load shaders
	ResourceManager::LoadShader("shader/sprite.vs", "shader/sprite.fs", nullptr, "sprite");
	// Set render-specific controls

	Renderer.push_back(new SpriteRenderer(ResourceManager::GetShader("sprite"), far_wall_vertices, 6 * 8));
	tex_color.push_back((char*)"waywall");
	tex_spec.push_back((char*)"waywall_spec");
	tex_norm.push_back((char*)"waywall_norm");
	tex_high.push_back((char*)"waywall_high");
	Renderer.push_back(new SpriteRenderer(ResourceManager::GetShader("sprite"), near_wall_vertices, 6 * 8));
	tex_color.push_back((char*)"waywall");
	tex_spec.push_back((char*)"waywall_spec");
	tex_norm.push_back((char*)"waywall_norm");
	tex_high.push_back((char*)"waywall_high");
	Renderer.push_back(new SpriteRenderer(ResourceManager::GetShader("sprite"), left_wall_vertices, 6 * 8));
	tex_color.push_back((char*)"waywall");
	tex_spec.push_back((char*)"waywall_spec");
	tex_norm.push_back((char*)"waywall_norm");
	tex_high.push_back((char*)"waywall_high");
	Renderer.push_back(new SpriteRenderer(ResourceManager::GetShader("sprite"), right_wall_vertices, 6 * 8));
	tex_color.push_back((char*)"waywall");
	tex_spec.push_back((char*)"waywall_spec");
	tex_norm.push_back((char*)"waywall_norm");
	tex_high.push_back((char*)"waywall_high");
	Renderer.push_back(new SpriteRenderer(ResourceManager::GetShader("sprite"), down_wall_vertices, 6 * 8));
	tex_color.push_back((char*)"floor");
	tex_spec.push_back((char*)"floor_spec");
	tex_norm.push_back((char*)"floor_norm");
	tex_high.push_back((char*)"floor_high");
	Renderer.push_back(new SpriteRenderer(ResourceManager::GetShader("sprite"), up_wall_vertices, 6 * 8));
	tex_color.push_back((char*)"waywall");
	tex_spec.push_back((char*)"waywall_spec");
	tex_norm.push_back((char*)"waywall_norm");
	tex_high.push_back((char*)"waywall_high");

	camera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f));
	// Configure shaders
	glm::mat4 projection = glm::perspective(glm::radians(this->camera->Zoom), (float)this->Width / (float)this->Height, 0.1f, 100.0f);
	glm::mat4 view = this->camera->GetViewMatrix();
	ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
	ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
	ResourceManager::GetShader("sprite").SetMatrix4("view", view);
	// Load textures
	ResourceManager::LoadTexture("obj/stonewall/Stone_02_COLOR.jpg", GL_TRUE, "stonewall");
	ResourceManager::LoadTexture("obj/stonewall/Stone_02_SPEC.jpg", GL_TRUE, "stonewall_spec");
	ResourceManager::LoadTexture("obj/stonewall/Stone_02_NRM.jpg", GL_TRUE, "stonewall_norm");

	ResourceManager::LoadTexture("obj/wall2/Terracotta_Tiles_002_Base_Color.png", GL_TRUE, "waywall");
	ResourceManager::LoadTexture("obj/wall2/Terracotta_Tiles_002_ambientOcclusion.png", GL_TRUE, "waywall_spec");
	ResourceManager::LoadTexture("obj/wall2/Terracotta_Tiles_002_Normal.png", GL_TRUE, "waywall_norm");
	ResourceManager::LoadTexture("obj/wall2/Terracotta_Tiles_002_Height.png", GL_TRUE, "waywall_high");

	ResourceManager::LoadTexture("obj/Wood Floor_007_SD/Wood_Floor_007_COLOR.jpg", GL_TRUE, "floor"); 
	ResourceManager::LoadTexture("obj/Wood Floor_007_SD/Wood_Floor_007_DISP.png", GL_TRUE, "floor_spec");
	ResourceManager::LoadTexture("obj/Wood Floor_007_SD/Wood_Floor_007_NORM.jpg", GL_TRUE, "floor_norm");
	ResourceManager::LoadTexture("obj/Wood Floor_007_SD/Wood_Floor_007_ROUGH.jpg", GL_TRUE, "floor_high");

	glEnable(GL_DEPTH_TEST);
}

void Game::Update(GLfloat dt)
{

}


void Game::ProcessInput(GLfloat dt)
{
	//keyboard input
	if (this->Keys[GLFW_KEY_W] == GLFW_PRESS)
		this->camera->ProcessKeyboard(FORWARD, dt);
	if (this->Keys[GLFW_KEY_S] == GLFW_PRESS)
		this->camera->ProcessKeyboard(BACKWARD, dt);
	if (this->Keys[GLFW_KEY_A] == GLFW_PRESS)
		this->camera->ProcessKeyboard(LEFT, dt);
	if (this->Keys[GLFW_KEY_D] == GLFW_PRESS)
		this->camera->ProcessKeyboard(RIGHT, dt);
}

void Game::ProcessMouseMovement(GLfloat dt) {
	//mouse input
	if (this->firstMouse)
	{
		this->lastX = this->xpos;
		this->lastY = this->ypos;
		this->firstMouse = false;
	}
	float dx = this->xpos - this->lastX;
	float dy = this->lastY - this->ypos; // reversed since y-coordinates go from bottom to top
	this->lastX = this->xpos;
	this->lastY = this->ypos;
	this->camera->ProcessMouseMovement(dx, dy);
}

void Game::ProcessScrollMovement(GLfloat yoffset) {
	//scroll input
	this->camera->ProcessMouseScroll(yoffset);
}

void Game::Render()
{
	std::vector<SpriteRenderer*>::iterator si;
	std::vector<GLchar*>::iterator tci, tsi, tni, thi;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	for (si = Renderer.begin(),
		 tci = tex_color.begin(),
		 tsi = tex_spec.begin(),
		 tni = tex_norm.begin(),
		 thi = tex_high.begin();
		 si != Renderer.cend();
		 si++,tci++,tsi++,tni++,thi++
		)
	(*si)->DrawSprite(this->camera, this->Height, this->Width, ResourceManager::GetTexture(*tci), ResourceManager::GetTexture(*tsi), ResourceManager::GetTexture(*tni), ResourceManager::GetTexture(*thi),glm::vec3(0,0,0), glm::vec3(1,1,1), 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
}

