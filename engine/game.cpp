
#include <vector>
#include <assert.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "game.h"
#include "camera.h"
#include "resource_manager.h"
#include "sprite_renderer.h"
#include "model_renderer.h"
#include "LightRenderer.h"
#include "square_renderer.h"

std::vector<GLchar*> tex_color;
std::vector<GLchar*> tex_spec;
std::vector<GLchar*> tex_norm, tex_high;
std::vector<SpriteRenderer*>  Renderer;
std::vector<ModelRenderer*> Model_renders;
std::vector<glm::vec3> Model_place;
std::vector<glm::vec3> Model_size;
std::vector<LightRenderer*> Light_renders;
std::vector<square_renderer*> square_render;

Game::Game(GLuint width, GLuint height)
	: State(GAME_ROOM1), Keys(), Width(width), Height(height), firstMouse(true)
{

}

Game::~Game()
{

}

void Game::Init()
{
	//set trigger_square
	static GLfloat square1[12] = {
		-0.9, 0.0, -3.5, 
		0.9 , 0.0, -3.5,
		0.9, 2.2, -3.5,
		-0.9, 2.2, -3.5
	};
	this->trigger_square.push_back((GLfloat*)square1);
	static GLfloat square2[12] = {

	};
	this->trigger_square.push_back((GLfloat*)square2);

	GLfloat far_wall_vertices[] = {
		// positions          // normals           // texture coords
		//far
		-4.0f, -4.0f, -4.0f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
		4.0f, -4.0f, -4.0f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
		4.0f,  4.0f, -4.0f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		4.0f,  4.0f, -4.0f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		-4.0f,  4.0f, -4.0f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
		-4.0f, -4.0f, -4.0f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
	};
	GLfloat near_wall_vertices[] = {
		//near
		-4.0f, -4.0f,  25.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
		4.0f, -4.0f,  25.0f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
		4.0f,  4.0f,  25.0f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		4.0f,  4.0f,  25.0f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		-4.0f,  4.0f,  25.0f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
		-4.0f, -4.0f,  25.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
	};
	GLfloat left_wall_vertices[] = {
		//left
		-4.0f,  4.0f,  25.0f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		-4.0f,  4.0f, -4.0f, -1.0f,  0.0f,  0.0f,  3.0f,  0.0f,
		-4.0f, -4.0f, -4.0f, -1.0f,  0.0f,  0.0f,  3.0f,  1.0f,
		-4.0f, -4.0f, -4.0f, -1.0f,  0.0f,  0.0f,  3.0f,  1.0f,
		-4.0f, -4.0f,  25.0f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-4.0f,  4.0f,  25.0f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
	};
	GLfloat right_wall_vertices[] = {
		//right
		4.0f,  4.0f,  25.0f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		4.0f,  4.0f, -4.0f,  1.0f,  0.0f,  0.0f,  4.0f,  0.0f,
		4.0f, -4.0f, -4.0f,  1.0f,  0.0f,  0.0f,  4.0f,  1.0f,
		4.0f, -4.0f, -4.0f,  1.0f,  0.0f,  0.0f,  4.0f,  1.0f,
		4.0f, -4.0f,  25.0f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		4.0f,  4.0f,  25.0f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
	};
	GLfloat down_wall_vertices[] = {
		//down
		-4.0f, -4.0f, -4.0f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
		4.0f, -4.0f, -4.0f,  0.0f, -1.0f,  0.0f,  6.0f,  1.0f,
		4.0f, -4.0f,  25.0f,  0.0f, -1.0f,  0.0f,  6.0f,  0.0f,
		4.0f, -4.0f,  25.0f,  0.0f, -1.0f,  0.0f,  6.0f,  0.0f,
		-4.0f, -4.0f,  25.0f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
		-4.0f, -4.0f, -4.0f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
	};
	GLfloat up_wall_vertices[] = {
		//up
		-4.0f,  4.0f, -4.0f,  0.0f,  1.0f,  0.0f,  0.0f,  2.0f,
		4.0f,  4.0f, -4.0f,  0.0f,  1.0f,  0.0f,  2.0f,  2.0f,
		4.0f,  4.0f,  25.0f,  0.0f,  1.0f,  0.0f,  2.0f,  0.0f,
		4.0f,  4.0f,  25.0f,  0.0f,  1.0f,  0.0f,  2.0f,  0.0f,
		-4.0f,  4.0f,  25.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
		-4.0f,  4.0f, -4.0f,  0.0f,  1.0f,  0.0f,  0.0f,  2.0f
	};

	// Load shaders
	ResourceManager::LoadShader("shader/sprite.vs", "shader/sprite.fs", nullptr, "sprite");
	ResourceManager::LoadShader("shader/lamp.vs", "shader/lamp.fs", nullptr, "lamp");
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

	Model_renders.push_back (new ModelRenderer(ResourceManager::GetShader("lamp"),"obj/td2/td2.obj"));
	Model_place.push_back(glm::vec3(-2.5, -4.0, -1.5));
	Model_size.push_back(glm::vec3(2.0, 2.0, 2.0));
	Model_renders.push_back(new ModelRenderer(ResourceManager::GetShader("lamp"), "obj/xk/xk.obj"));
	Model_place.push_back(glm::vec3(0.0, 0.0, -3.8));
	Model_size.push_back(glm::vec3(1.5, 1.5, 1.5));

	float light_vertices[] = {
		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f, -0.5f,  0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,

		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,

		0.5f,  0.5f,  0.5f,
		0.5f,  0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,

		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f,  0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		0.5f,  0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
	};

	ResourceManager::LoadShader("shader/light.vs", "shader/light.fs", nullptr, "light");
	Light_renders.push_back(new LightRenderer(ResourceManager::GetShader("light"), light_vertices));

	GLfloat x = 0.9f, y = -0.0f , z =-3.5f , w = -0.9f ,v = 2.2f;
	GLfloat square[] = {
		x,y,z,
		w,y,z,
		w,v,z,
		x,y,z,
		x,v,z,
		w,v,z
	};
	ResourceManager::LoadShader("shader/light.vs", "shader/light.fs", nullptr, "square");
	square_render.push_back(new square_renderer(ResourceManager::GetShader("square"), square));

	glEnable(GL_DEPTH_TEST);
}

GLint check(GLfloat* square, Camera* camera) {
	glm::vec3 pos = camera->Position;
	glm::vec3 front = camera->Front;
	glm::vec3 vert[4];
	for (int i = 0; i < 4; i++) {
		vert[i] = glm::vec3(*(square + 3 * i + 0), *(square + 3 * i + 1), *(square + 3 * i + 2));
	}
	assert(vert[0].x + vert[2].x == vert[1].x + vert[3].x);
	assert(vert[0].y + vert[2].y == vert[1].y + vert[3].y);
	assert(vert[0].z + vert[2].z == vert[1].z + vert[3].z);
	glm::mat3 vertM = transpose(glm::make_mat3x3((const GLfloat*)vert));
	glm::mat3 inv_vertM = inverse(vertM);
	/*
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			printf("%f ", inv_vertM[i][j]);
	printf("\n");
	glm::mat3 plane = inv_vertM * glm::mat3(1.0f);
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			printf("%f ", plane[i][j]);
	printf("\n");
	*/
	glm::vec3 plane = inv_vertM * glm::vec3(1.0, 1.0, 1.0);
	GLfloat k = (1 - plane[0] * pos[0] - plane[1] * pos[1] - plane[2] * pos[2]) / (plane[0] * front[0] + plane[1] * front[1] + plane[2] * front[2]);
	glm::vec3 new_pos = pos + k * front;
	
	glm::vec3 flag = cross(new_pos - vert[3], new_pos - vert[0]);
	for (int i = 0; i < 3; i++) {
		GLfloat dotres = dot(flag, cross(new_pos - vert[i], new_pos - vert[i + 1]));
		if (dotres < 0) {
			return 0;
		}
	}
	return 1;
}

void Game::Update(GLfloat dt)
{
	static GLfloat stare_count = 0;
	if (check(this->trigger_square[this->State], this->camera)) {
		stare_count += dt;
		if (stare_count > 5.0) {
			this->State = END;
		}
	}
	else stare_count = 0.0;

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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	std::vector<SpriteRenderer*>::iterator si;
	std::vector<GLchar*>::iterator tci, tsi, tni, thi;
	for (si = Renderer.begin(),
		 tci = tex_color.begin(),
		 tsi = tex_spec.begin(),
		 tni = tex_norm.begin(),
		 thi = tex_high.begin();
		 si != Renderer.cend();
		 si++,tci++,tsi++,tni++,thi++
		)
	(*si)->DrawSprite(this->camera, this->Height, this->Width, ResourceManager::GetTexture(*tci), ResourceManager::GetTexture(*tsi), ResourceManager::GetTexture(*tni), ResourceManager::GetTexture(*thi),glm::vec3(0,0,0), glm::vec3(1,1,1), 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	
	std::vector<ModelRenderer*>::iterator mi;
	std::vector<glm::vec3>::iterator mpi, msi;
	for (mi = Model_renders.begin(),
		 mpi = Model_place.begin(),
		 msi = Model_size.begin(); mi != Model_renders.cend(); mi++, mpi++, msi++)
		(*mi)->Draw(this->camera, this->Height, this->Width, *mpi, *msi, 0.0f, glm::vec3(0.0f, 1.0f, 0.0f));

	std::vector<LightRenderer*>::iterator li;
	for (li = Light_renders.begin(); li != Light_renders.cend(); li++)
		(*li)->Draw(this->camera, this->Height, this->Width, glm::vec3(-2.5, 0.0, -1.5));

	std::vector<square_renderer*>::iterator sqi;
	for (sqi = square_render.begin(); sqi != square_render.cend(); sqi++)
		(*sqi)->Draw(this->camera, this->Height, this->Width);

}

