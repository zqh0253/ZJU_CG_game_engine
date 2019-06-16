
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
#include "post_processor.h"
#include "bare_sprite_renderer.h"
#include "skybox_renderer.h"
std::vector<GLchar*> tex_color, tex_color2;
std::vector<GLchar*> tex_spec, tex_spec2;
std::vector<GLchar*> tex_norm, tex_high, tex_norm2, tex_high2;
std::vector<SpriteRenderer*>  Renderer, Renderer2;
std::vector<ModelRenderer*> Model_renders, Model_renders2;
std::vector<glm::vec3> Model_place, Model_place2;
std::vector<glm::vec3> Model_size, Model_size2;
std::vector<GLfloat> Model_rotate, Model_rotate2;
std::vector<LightRenderer*> Light_renders;
std::vector<square_renderer*> square_render, square_render2;

std::vector<bare_sprite_renderer*> Bare_renders;
std::vector<GLchar*> bare_tex_color;

skybox_renderer* sky_renders;
GLchar* tex_sky;

Game::Game(GLuint width, GLuint height)
	: State(GAME_ROOM1), Keys(), Width(width), Height(height), firstMouse(true), stare_count(0.0)
{

}

Game::~Game()
{

}

void Game::Init()
{
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
	ResourceManager::LoadTexture("obj/room2floor/Wood_Floor_006_COLOR.jpg", GL_TRUE, "floor2");
	ResourceManager::LoadTexture("obj/room2floor/Wood_Floor_006_DISP.png", GL_TRUE, "floor2_spec");
	ResourceManager::LoadTexture("obj/room2floor/Wood_Floor_006_NORM.jpg", GL_TRUE, "floor2_norm");
	ResourceManager::LoadTexture("obj/room2floor/Wood_Floor_006_ROUGH.jpg", GL_TRUE, "floor2_high");
	ResourceManager::LoadTexture("obj/wallpaper.jpg", GL_TRUE, "wallpaper");
	ResourceManager::LoadShader("shader/light.vs", "shader/light.fs", nullptr, "light");
	ResourceManager::LoadShader("shader/light.vs", "shader/light.fs", nullptr, "square");
	ResourceManager::LoadShader("shader/baresprite.vs", "shader/baresprite.fs", nullptr, "baresprite");
	ResourceManager::LoadShader("shader/sky.vs", "shader/sky.fs", nullptr, "sky");

	this->Effects = new PostProcessor(ResourceManager::LoadShader("shader/post.vs", "shader/post.fs", nullptr, "post"), this->Width, this->Height);
	//set trigger_square
	static GLfloat square1[12] = {
		-0.9, 0.0, -3.5, 
		0.9 , 0.0, -3.5,
		0.9, 2.2, -3.5,
		-0.9, 2.2, -3.5
	};
	this->trigger_square.push_back((GLfloat*)square1);
	static GLfloat square2[12] = {
		1.0, -2.3, 21.0,
		1.5 , -2.3, 21.0,
		1.5, -2.2, 21.0,
		1.0, -2.2, 21.0
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
		-4.0f,  4.0f, -4.0f,  0.0f,  1.0f,  0.0f,  0.0f,  5.0f,
		4.0f,  4.0f, -4.0f,  0.0f,  1.0f,  0.0f,  1.5f,  5.0f,
		4.0f,  4.0f,  25.0f,  0.0f,  1.0f,  0.0f,  1.5f,  0.0f,
		4.0f,  4.0f,  25.0f,  0.0f,  1.0f,  0.0f,  1.5f,  0.0f,
		-4.0f,  4.0f,  25.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
		-4.0f,  4.0f, -4.0f,  0.0f,  1.0f,  0.0f,  0.0f,  5.0f
	};

	GLfloat far_bare_wall_vertices[] = {
		// positions          // normals           // texture coords
		//far
		-4.0f, -4.0f, -4.0f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
		4.0f, -4.0f, -4.0f,  0.0f,  0.0f, -1.0f,  3.0f,  0.0f,
		4.0f,  4.0f, -4.0f,  0.0f,  0.0f, -1.0f,  3.0f,  1.0f,
		4.0f,  4.0f, -4.0f,  0.0f,  0.0f, -1.0f,  3.0f,  1.0f,
		-4.0f,  4.0f, -4.0f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
		-4.0f, -4.0f, -4.0f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
	};
	GLfloat near_bare_wall_vertices[] = {
		//near
		-4.0f, -4.0f,  25.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
		4.0f, -4.0f,  25.0f,  0.0f,  0.0f,  1.0f,  3.0f,  0.0f,
		4.0f,  4.0f,  25.0f,  0.0f,  0.0f,  1.0f,  3.0f,  1.0f,
		4.0f,  4.0f,  25.0f,  0.0f,  0.0f,  1.0f,  3.0f,  1.0f,
		-4.0f,  4.0f,  25.0f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
		-4.0f, -4.0f,  25.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
	};
	GLfloat left_bare_wall_vertices[] = {
		//left
		-4.0f,  4.0f,  25.0f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		-4.0f,  4.0f, -4.0f, -1.0f,  0.0f,  0.0f,  3.0f,  0.0f,
		-4.0f, -4.0f, -4.0f, -1.0f,  0.0f,  0.0f,  3.0f,  1.0f,
		-4.0f, -4.0f, -4.0f, -1.0f,  0.0f,  0.0f,  3.0f,  1.0f,
		-4.0f, -4.0f,  25.0f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-4.0f,  4.0f,  25.0f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
	};
	GLfloat right_bare_wall_vertices[] = {
		//right
		4.0f,  4.0f,  25.0f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		4.0f,  4.0f, -4.0f,  1.0f,  0.0f,  0.0f,  4.0f,  0.0f,
		4.0f, -4.0f, -4.0f,  1.0f,  0.0f,  0.0f,  4.0f,  1.0f,
		4.0f, -4.0f, -4.0f,  1.0f,  0.0f,  0.0f,  4.0f,  1.0f,
		4.0f, -4.0f,  25.0f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		4.0f,  4.0f,  25.0f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
	};
	GLfloat down_bare_wall_vertices[] = {
		//down
		-4.0f, -4.0f, -4.0f,  0.0f, -1.0f,  0.0f,  0.0f,  3.0f,
		4.0f, -4.0f, -4.0f,  0.0f, -1.0f,  0.0f,  6.0f,  3.0f,
		4.0f, -4.0f,  25.0f,  0.0f, -1.0f,  0.0f,  6.0f,  0.0f,
		4.0f, -4.0f,  25.0f,  0.0f, -1.0f,  0.0f,  6.0f,  0.0f,
		-4.0f, -4.0f,  25.0f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
		-4.0f, -4.0f, -4.0f,  0.0f, -1.0f,  0.0f,  0.0f,  3.0f,
	};
	GLfloat up_bare_wall_vertices[] = {
		//up
		-4.0f,  4.0f, -4.0f,  0.0f,  1.0f,  0.0f,  0.0f,  4.0f,
		4.0f,  4.0f, -4.0f,  0.0f,  1.0f,  0.0f,  4.0f,  4.0f,
		4.0f,  4.0f,  25.0f,  0.0f,  1.0f,  0.0f,  4.0f,  0.0f,
		4.0f,  4.0f,  25.0f,  0.0f,  1.0f,  0.0f,  4.0f,  0.0f,
		-4.0f,  4.0f,  25.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
		-4.0f,  4.0f, -4.0f,  0.0f,  1.0f,  0.0f,  0.0f,  4.0f
	};

	// Load shaders
	ResourceManager::LoadShader("shader/sprite.vs", "shader/sprite.fs", nullptr, "sprite");
	ResourceManager::LoadShader("shader/lamp.vs", "shader/lamp.fs", nullptr, "model");
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

	camera = new Camera(glm::vec3(0.0, 0.0, 20.0));
	// Configure shaders
	glm::mat4 projection = glm::perspective(glm::radians(this->camera->Zoom), (float)this->Width / (float)this->Height, 0.1f, 100.0f);
	glm::mat4 view = this->camera->GetViewMatrix();

	Model_renders.push_back(new ModelRenderer(ResourceManager::GetShader("model"), "obj/td2/td2.obj"));
	Model_place.push_back(glm::vec3(-2.5, -4.0, -1.5));
	Model_size.push_back(glm::vec3(2.0, 2.0, 2.0));
	Model_rotate.push_back(0.0);
	Model_renders.push_back(new ModelRenderer(ResourceManager::GetShader("model"), "obj/td2/td2.obj"));
	Model_place.push_back(glm::vec3(2.5, -4.0, -1.5));
	Model_size.push_back(glm::vec3(2.0, 2.0, 2.0));
	Model_rotate.push_back(0.0);
	Model_renders.push_back(new ModelRenderer(ResourceManager::GetShader("model"), "obj/xk/xk.obj"));
	Model_place.push_back(glm::vec3(0.0, 0.0, -3.8));
	Model_size.push_back(glm::vec3(1.5, 1.5, 1.5));
	Model_rotate.push_back(0.0);

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


	Light_renders.push_back(new LightRenderer(ResourceManager::GetShader("light"), light_vertices));
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
	square_render.push_back(new square_renderer(ResourceManager::GetShader("square"), square));
	x = 0.9f; y = -0.23f; z = 20.8f; w = 0.4f; v = -0.33; 
	GLfloat squaree[] = {
			x,y,z,
			w,y,z,
			w,v,z,
			x,y,z,
			x,v,z,
			w,v,z
		};
	square_render2.push_back(new square_renderer(ResourceManager::GetShader("square"), squaree));

	Bare_renders.push_back(new bare_sprite_renderer(ResourceManager::GetShader("baresprite"), far_bare_wall_vertices, 6 * 8));
	bare_tex_color.push_back((char*)"wallpaper");
	Bare_renders.push_back(new bare_sprite_renderer(ResourceManager::GetShader("baresprite"), near_bare_wall_vertices, 6 * 8));
	bare_tex_color.push_back((char*)"wallpaper");
	Bare_renders.push_back(new bare_sprite_renderer(ResourceManager::GetShader("baresprite"), left_bare_wall_vertices, 6 * 8));
	bare_tex_color.push_back((char*)"wallpaper");
	Bare_renders.push_back(new bare_sprite_renderer(ResourceManager::GetShader("baresprite"), right_bare_wall_vertices, 6 * 8));
	bare_tex_color.push_back((char*)"wallpaper");
	Bare_renders.push_back(new bare_sprite_renderer(ResourceManager::GetShader("baresprite"), up_bare_wall_vertices, 6 * 8));
	bare_tex_color.push_back((char*)"wallpaper");
	Renderer2.push_back(new SpriteRenderer(ResourceManager::GetShader("sprite"), down_bare_wall_vertices, 6 * 8));
	tex_color2.push_back((char*)"floor2");
	tex_spec2.push_back((char*)"floor2_spec");
	tex_norm2.push_back((char*)"floor2_norm");
	tex_high2.push_back((char*)"floor2_high");

	Model_renders2.push_back(new ModelRenderer(ResourceManager::GetShader("model"), "obj/room2_obj/dad/hm.obj"));
	Model_place2.push_back(glm::vec3(-7.0, -3.6, 12.8));
	Model_size2.push_back(glm::vec3(2.7, 2.7, 2.7));
	Model_rotate2.push_back(40.0);
	Model_renders2.push_back(new ModelRenderer(ResourceManager::GetShader("model"), "obj/room2_obj/mom_and_child/hm3.obj"));
	Model_place2.push_back(glm::vec3(1.0, -2.3, 21.0));
	Model_size2.push_back(glm::vec3(2.5, 2.5, 2.5));
	Model_rotate2.push_back(180.0);
	Model_renders2.push_back(new ModelRenderer(ResourceManager::GetShader("model"), "obj/room2_obj/door/door.obj"));
	Model_place2.push_back(glm::vec3(6.8, -4.0, -3.8));
	Model_size2.push_back(glm::vec3(3.0, 3.0, 3.0));
	Model_rotate2.push_back(0.0);
	Model_renders2.push_back(new ModelRenderer(ResourceManager::GetShader("model"), "obj/room2_obj/table/table.obj"));
	Model_place2.push_back(glm::vec3(1.0, -4.0, 15.0));
	Model_size2.push_back(glm::vec3(4.0, 4.0, 4.0));
	Model_rotate2.push_back(0.0);
	Model_renders2.push_back(new ModelRenderer(ResourceManager::GetShader("model"), "obj/room2_obj/sf/sf.obj"));
	Model_place2.push_back(glm::vec3(1.0, -4.0, 22.0));
	Model_size2.push_back(glm::vec3(4.0, 4.0, 4.0));
	Model_rotate2.push_back(180.0);
	Model_renders2.push_back(new ModelRenderer(ResourceManager::GetShader("model"), "obj/room2_obj/window/window.obj"));
	Model_place2.push_back(glm::vec3(12.0, -1.0, 12.0));
	Model_size2.push_back(glm::vec3(1.0, 1.0, 1.0));
	Model_rotate2.push_back(90.0);
	Model_renders2.push_back(new ModelRenderer(ResourceManager::GetShader("model"), "obj/room2_obj/tvdesk/dd.obj"));
	Model_place2.push_back(glm::vec3(-0.8, -4.0, -1.8));
	Model_size2.push_back(glm::vec3(3.0, 3.0, 3.0));
	Model_rotate2.push_back(0.0);
	Model_renders2.push_back(new ModelRenderer(ResourceManager::GetShader("model"), "obj/room2_obj/tv/tv.obj"));
	Model_place2.push_back(glm::vec3(-0.8, -2.0, -1.8));
	Model_size2.push_back(glm::vec3(3.0, 3.0, 3.0));
	Model_rotate2.push_back(0.0);

	//sky box
	GLfloat skyboxVertices[] = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};

	sky_renders=(new skybox_renderer(ResourceManager::GetShader("sky"), skyboxVertices));

#ifdef 	BOX1
	std::vector <std::string> faces
	{
		"obj/skybox/1/right.jpg",
		"obj/skybox/1/left.jpg",
		"obj/skybox/1/top.jpg",
		"obj/skybox/1/bottom.jpg",
		"obj/skybox/1/front.jpg",
		"obj/skybox/1/back.jpg"
	};
#else
	std::vector <std::string> faces
	{
		"obj/skybox/2/posx.jpg",
		"obj/skybox/2/negx.jpg",
		"obj/skybox/2/posy.jpg",
		"obj/skybox/2/negy.jpg",
		"obj/skybox/2/posz.jpg",
		"obj/skybox/2/negz.jpg"
	};
#endif
	ResourceManager::LoadSkybox(faces, "sky");
	tex_sky=(char*)"sky"; 

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
	
	glm::vec3 plane = inv_vertM * glm::vec3(1.0, 1.0, 1.0);
	GLfloat k = (1 - plane[0] * pos[0] - plane[1] * pos[1] - plane[2] * pos[2]) / (plane[0] * front[0] + plane[1] * front[1] + plane[2] * front[2]);
	if (k < 0.0) return 0;
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
	if (check(this->trigger_square[this->State], this->camera)) {
		this->stare_count += dt;
		if (this->stare_count > 5.0) {
			((int&)this->State)++;
			this->stare_count = 0.0;
			if (this->State == GAME_ROOM2)
				this->camera->Position = glm::vec3(8.0,0.0,18.0);
		}
	}
	else this->stare_count = 0.0;
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

	this->Effects->BeginRender();
	switch(this->State){
	case GAME_ROOM1:{
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
		(*si)->DrawSprite(this->camera, this->Height, this->Width, ResourceManager::GetTexture(*tci), ResourceManager::GetTexture(*tsi), ResourceManager::GetTexture(*tni), ResourceManager::GetTexture(*thi),glm::vec3(0,0,0), glm::vec3(1,1,1), 0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	
		std::vector<ModelRenderer*>::iterator mi;
		std::vector<glm::vec3>::iterator mpi, msi;
		std::vector<GLfloat>::iterator mri;
		for (mi = Model_renders.begin(),
			 mpi = Model_place.begin(),
			 msi = Model_size.begin(),
			 mri = Model_rotate.begin(); mi != Model_renders.cend(); mi++, mpi++, msi++, mri++)
			(*mi)->Draw(this->camera, this->Height, this->Width, *mpi, *msi, *mri, glm::vec3(0.0f, 1.0f, 0.0f));

		std::vector<LightRenderer*>::iterator li;
		for (li = Light_renders.begin(); li != Light_renders.cend(); li++)
			(*li)->Draw(this->camera, this->Height, this->Width, glm::vec3((li==Light_renders.begin())?-2.5:2.5, 0.0, -1.5));

#ifdef SQUARE
		std::vector<square_renderer*>::iterator sqi;
		for (sqi = square_render.begin(); sqi != square_render.cend(); sqi++)
			(*sqi)->Draw(this->camera, this->Height, this->Width);
#endif 
		sky_renders->Draw(this->camera, ResourceManager::GetSkybox("sky"), this->Height, this->Width);
		break; }
	case GAME_ROOM2: {
#ifdef SQUARE
		std::vector<square_renderer*>::iterator sqi;
		for (sqi = square_render2.begin(); sqi != square_render2.cend(); sqi++)
			(*sqi)->Draw(this->camera, this->Height, this->Width);
#endif 
		std::vector<bare_sprite_renderer*>::iterator bsi;
		std::vector<GLchar*>::iterator bci;
		for (bsi = Bare_renders.begin(),
			bci = bare_tex_color.begin();
			bsi != Bare_renders.cend();
			bsi++, bci++) {
			(*bsi)->Draw(this->camera, this->Height, this->Width, ResourceManager::GetTexture(*bci), glm::vec3(0, 0, 0), glm::vec3(3, 1, 1), 0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		}
		std::vector<SpriteRenderer*>::iterator si;
		std::vector<GLchar*>::iterator tci, tsi, tni, thi;
		for (si = Renderer2.begin(),
			tci = tex_color2.begin(),
			tsi = tex_spec2.begin(),
			tni = tex_norm2.begin(),
			thi = tex_high2.begin();
			si != Renderer2.cend();
			si++, tci++, tsi++, tni++, thi++
			)
			(*si)->DrawSprite(this->camera, this->Height, this->Width, ResourceManager::GetTexture(*tci), ResourceManager::GetTexture(*tsi), ResourceManager::GetTexture(*tni), ResourceManager::GetTexture(*thi), glm::vec3(0, 0, 0), glm::vec3(3, 1, 1), 0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		std::vector<ModelRenderer*>::iterator mi;
		std::vector<glm::vec3>::iterator mpi, msi;
		std::vector<GLfloat>::iterator mri;
		for (mi = Model_renders2.begin(),
			 mpi = Model_place2.begin(),
			 msi = Model_size2.begin(),
			 mri = Model_rotate2.begin(); mi != Model_renders2.cend(); mi++, mpi++, msi++, mri++)
			(*mi)->Draw(this->camera, this->Height, this->Width, *mpi, *msi, *mri, glm::vec3(0.0f, 1.0f, 0.0f));

		sky_renders->Draw(this->camera, ResourceManager::GetSkybox("sky"), this->Height, this->Width);

	}
	default: {
		break;
	}
	}
	this->Effects->EndRender();
	this->Effects->Render(glfwGetTime());
}

