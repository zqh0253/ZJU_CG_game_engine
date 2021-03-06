
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
#include "mc_grass_cube.h"
#include "voxel.h"
#include "text.h"

#define SQUARE
#define BMP_Header_Length 54
#define  _CRT_SECURE_NO_WARNINGS 
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

std::vector<bare_sprite_renderer*> Bare_renders, mc_renders;
std::vector<GLchar*> bare_tex_color, mc_tex_color;

GLfloat time_cnt_for_words=0.0f;

skybox_renderer* sky_renders;
GLchar* tex_sky;
voxel vox;
text tex;
GLint tv_channel = 0;
//feed in 3 vertices of a rectangle. 
//return the coefficient of the rectangle's plane.
glm::vec3 get_plane_coefficient(glm::vec3 vert[]) {
	assert(vert[0].x + vert[2].x == vert[1].x + vert[3].x);
	assert(vert[0].y + vert[2].y == vert[1].y + vert[3].y);
	assert(vert[0].z + vert[2].z == vert[1].z + vert[3].z);
	glm::mat3 vertM = transpose(glm::make_mat3x3((const GLfloat*)vert));
	glm::mat3 inv_vertM = inverse(vertM);
	glm::vec3 plane = inv_vertM * glm::vec3(1.0, 1.0, 1.0);
	return plane;
}

Game::Game(GLuint width, GLuint height)
	: State(GAME_ROOM1), Keys(), Width(width), Height(height), firstMouse(true), stare_count(0.0), lock_camera(0)
{

}

Game::~Game()
{

}

void Game::DrawCube(GLfloat xa, GLfloat xb, GLfloat ya, GLfloat yb, GLfloat za, GLfloat zb, vector<square_renderer*>&square_render, GLint state)
{
	GLfloat square1[] = {
		xa,ya,za,
		xb,ya,za,
		xb,ya,zb,
		xa,ya,za,
		xa,ya,zb,
		xb,ya,zb
	};
	glm::mat4x3 v1 = {
		xa,ya,za,
		xb,ya,za,
		xb,ya,zb,
		xa,ya,zb
	};
	planes[state].push_back(v1);
	square_render.push_back(new square_renderer(ResourceManager::GetShader("square"), square1));
	GLfloat square2[] = {
		xa,yb,za,
		xb,yb,za,
		xb,yb,zb,
		xa,yb,za,
		xa,yb,zb,
		xb,yb,zb
	};
	glm::mat4x3 v2 = {
		xa,yb,za,
		xb,yb,za,
		xb,yb,zb,
		xa,yb,zb
	};
	planes[state].push_back(v2);
	square_render.push_back(new square_renderer(ResourceManager::GetShader("square"), square2));
	GLfloat square3[] = {
		xa,ya,za,
		xa,yb,za,
		xa,yb,zb,
		xa,ya,za,
		xa,ya,zb,
		xa,yb,zb
	};
	glm::mat4x3 v3 = {
		xa,ya,za,
		xa,yb,za,
		xa,yb,zb,
		xa,ya,zb
	};
	planes[state].push_back(v3);
	square_render.push_back(new square_renderer(ResourceManager::GetShader("square"), square3));
	GLfloat square4[] = {
		xb,ya,za,
		xb,yb,za,
		xb,yb,zb,
		xb,ya,za,
		xb,ya,zb,
		xb,yb,zb
	};
	glm::mat4x3 v4 = {
		xb,ya,za,
		xb,yb,za,
		xb,yb,zb,
		xb,ya,zb
	};
	planes[state].push_back(v4);
	square_render.push_back(new square_renderer(ResourceManager::GetShader("square"), square4));
	GLfloat square5[] = {
		xa,ya,za,
		xa,yb,za,
		xb,yb,za,
		xa,ya,za,
		xb,ya,za,
		xb,yb,za
	};
	glm::mat4x3 v5 = {
		xa,ya,za,
		xa,yb,za,
		xb,yb,za,
		xb,ya,za
	};
	planes[state].push_back(v5);
	square_render.push_back(new square_renderer(ResourceManager::GetShader("square"), square5));
	GLfloat square6[] = {
		xa,ya,zb,
		xa,yb,zb,
		xb,yb,zb,
		xa,ya,zb,
		xb,ya,zb,
		xb,yb,zb
	};
	glm::mat4x3 v6 = {
		xa,ya,zb,
		xa,yb,zb,
		xb,yb,zb,
		xb,ya,zb
	};
	planes[state].push_back(v6);
	square_render.push_back(new square_renderer(ResourceManager::GetShader("square"), square6));
}
float persistence = 0.50;
int Number_Of_Octaves = 4;

double Noise(int x, int y)    // 根据(x,y)获取一个初步噪声值
{
	int n = x + y * 57;
	n = (n << 13) ^ n;
	return (1.0 - ((n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0);
}

double SmoothedNoise(int x, int y)   //光滑噪声
{
	double corners = (Noise(x - 1, y - 1) + Noise(x + 1, y - 1) + Noise(x - 1, y + 1) + Noise(x + 1, y + 1)) / 16;
	double sides = (Noise(x - 1, y) + Noise(x + 1, y) + Noise(x, y - 1) + Noise(x, y + 1)) / 8;
	double center = Noise(x, y) / 4;
	return corners + sides + center;
}
double Cosine_Interpolate(double a, double b, double x)  // 余弦插值
{
	double ft = x * 3.1415927;
	double f = (1 - cos(ft)) * 0.5;
	return a * (1 - f) + b * f;
}

double InterpolatedNoise(float x, float y)   // 获取插值噪声
{
	int integer_X = int(x);
	float  fractional_X = x - integer_X;
	int integer_Y = int(y);
	float fractional_Y = y - integer_Y;
	double v1 = SmoothedNoise(integer_X, integer_Y);
	double v2 = SmoothedNoise(integer_X + 1, integer_Y);
	double v3 = SmoothedNoise(integer_X, integer_Y + 1);
	double v4 = SmoothedNoise(integer_X + 1, integer_Y + 1);
	double i1 = Cosine_Interpolate(v1, v2, fractional_X);
	double i2 = Cosine_Interpolate(v3, v4, fractional_X);
	return Cosine_Interpolate(i1, i2, fractional_Y);
}

double PerlinNoise(float x, float y)    // 最终调用：根据(x,y)获得其对应的PerlinNoise值
{
	double total = 0;
	double p = persistence;
	int n = Number_Of_Octaves;
	for (int i = 0; i < n; i++)
	{
		double frequency = pow(2, i);
		double amplitude = pow(p, i);
		total = total + InterpolatedNoise(x * frequency, y * frequency) * amplitude;
	}
	total = pow(total + 0.5, 0.55);
	if (isnan(total)) return 0.1;
	return total;
}

mc_grass_cube cube;

void Game::Init()
{
	this->targetpos[0] = glm::vec3(0.3846, 1.4635, -1.8709);
	this->targetfrt[0] = glm::vec3(0.0, 0.0, -1.0);
	this->targetpos[1] = glm::vec3(-15.74456, 11.810843, 36.752373);
	this->targetfrt[1] = glm::vec3(0.623889, -0.4313, -0.6517);
	glm::vec3 vert[4];
	vert[0] = glm::vec3();

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
	ResourceManager::LoadTexture("obj/tv0.jpg", GL_TRUE, "tv0");
	ResourceManager::LoadTexture("obj/tv1.jpg", GL_TRUE, "tv1");
	ResourceManager::LoadTexture("obj/tv2.jpg", GL_TRUE, "tv2");
	//ResourceManager::LoadTexture("obj/tv.bmp", GL_TRUE, "tv3");
	//ResourceManager::LoadTexture("obj/tv.bmp", GL_TRUE, "tv4");
	ResourceManager::LoadTexture("obj/wallpaper.jpg", GL_TRUE, "wallpaper");
	ResourceManager::LoadTexture("obj/mc/useful/DefaultPack2.png", GL_TRUE, "mc2");
	ResourceManager::LoadShader("shader/light.vs", "shader/light.fs", nullptr, "light");
	ResourceManager::LoadShader("shader/light.vs", "shader/light.fs", nullptr, "square");
	ResourceManager::LoadShader("shader/baresprite.vs", "shader/baresprite.fs", nullptr, "baresprite");
	ResourceManager::LoadShader("shader/sky.vs", "shader/sky.fs", nullptr, "sky");
	ResourceManager::LoadShader("shader/voxel.vs", "shader/voxel.fs", nullptr, "voxel");
	ResourceManager::LoadShader("shader/text.vs", "shader/text.fs", nullptr, "text");


	this->Effects = new PostProcessor(ResourceManager::LoadShader("shader/post.vs", "shader/post.fs", nullptr, "post"), this->Width, this->Height);
	//set trigger_square
	static GLfloat square1[12] = {
		-0.9, 0.9, -3.5,
		1.2 , 0.9, -3.5,
		1.2, 2.4, -3.5,
		-0.9, 2.4, -3.5
	};

	this->trigger_square.push_back((GLfloat*)square1);
	static GLfloat square2[12] = {
		0.4, -0.43, 21.5,
		0.9 , -0.43, 21.5,
		0.9, -0.28, 21.5,
		0.4, -0.28, 21.5
	};
	this->trigger_square.push_back((GLfloat*)square2);
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
	GLfloat right_bare_wall1_vertices[] = {
		//right
		4.0f,  4.0f,  11.f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		4.0f,  4.0f, -4.0f,  1.0f,  0.0f,  0.0f,  2.07f,  0.0f,
		4.0f, -4.0f, -4.0f,  1.0f,  0.0f,  0.0f,  2.07f,  1.0f,
		4.0f, -4.0f, -4.0f,  1.0f,  0.0f,  0.0f,  2.07f,  1.0f,
		4.0f, -4.0f,  11.f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		4.0f,  4.0f,  11.f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
	};
	GLfloat right_bare_wall2_vertices[] = {
		//right
		4.0f,  4.0f,  25.0f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		4.0f,  4.0f, 13.0f,  1.0f,  0.0f,  0.0f,  1.66f,  0.0f,
		4.0f, -4.0f, 13.0f,  1.0f,  0.0f,  0.0f,  1.66f,  1.0f,
		4.0f, -4.0f, 13.0f,  1.0f,  0.0f,  0.0f,  1.66f,  1.0f,
		4.0f, -4.0f,  25.0f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		4.0f,  4.0f,  25.0f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
	};
	GLfloat right_bare_wall3_vertices[] = {
		//right
		4.0f,  4.0f,  13.0f,  1.0f,  0.0f,  0.0f,  2.17f,  0.0f,
		4.0f,  4.0f, 11.0f,  1.0f,  0.0f,  0.0f,  2.45f,  0.0f,
		4.0f, 1.8f, 11.f,  1.0f,  0.0f,  0.0f,  2.45f,  0.275f,
		4.0f, 1.8f, 11.f,  1.0f,  0.0f,  0.0f,  2.45f,  0.275f,
		4.0f, 1.8f,  13.f,  1.0f,  0.0f,  0.0f,  2.17f,  0.275f,
		4.0f,  4.0f,  13.f,  1.0f,  0.0f,  0.0f,  2.17f,  0.0f,
	};
	GLfloat right_bare_wall4_vertices[] = {
		//right
		4.0f,  -1.f,  13.0f,  1.0f,  0.0f,  0.0f,  2.17f,  0.0f,
		4.0f,  -1.f, 11.f,  1.0f,  0.0f,  0.0f,  2.45f,  0.0f,
		4.0f, -4.0f, 11.0f,  1.0f,  0.0f,  0.0f,  2.45f,  0.375f,
		4.0f, -4.0f, 11.0f,  1.0f,  0.0f,  0.0f,  2.45f,  0.375f,
		4.0f, -4.0f,  13.0f,  1.0f,  0.0f,  0.0f,  2.17f,  0.375f,
		4.0f,  -1.f,  13.0f,  1.0f,  0.0f,  0.0f,  2.17f,  0.0f,
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
	Model_place.push_back(glm::vec3(1.3, 1.8, -3.8));
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
	GLfloat x = 1.2f, y = 0.9f, z = -3.5f, w = -0.9f, v = 2.4f;
	GLfloat square[] = {
		x,y,z,
		w,y,z,
		w,v,z,
		x,y,z,
		x,v,z,
		w,v,z
	};
	square_render.push_back(new square_renderer(ResourceManager::GetShader("square"), square));
	x = -3.1f, y = -4.0f, z = -0.95f, w = -1.9f, v = 0.00f;
	DrawCube(-3.1, -1.9, -4.0, 0.1, -0.95, -2.05, square_render, 0);
	DrawCube(1.9, 3.1, -4.0, 0.1, -0.95, -2.05, square_render, 0);
	DrawCube(-3.85, 3.85, 3.85, -3.85, -3.85, 24.85, square_render, 0);

	DrawCube(-6.65, -7.4, -2, 0.3, 12.5, 13.6, square_render2, 1);
	DrawCube(-7.0, -8.15, -2, 0.3, 12.8, 14.1, square_render2, 1);
	DrawCube(-6.85, -7.6, 0.3, 1.0, 13.0, 13.8, square_render2, 1);
	DrawCube(-6.65, -7.4, -2, -4, 12.5, 13.6, square_render2, 1);
	DrawCube(-7.0, -8.15, -2, -4, 12.8, 14.1, square_render2, 1);

	DrawCube(-2.6, 4.6, -3, -1.9, 13.5, 16.5, square_render2, 1);
	DrawCube(-2.6, -1.9, -3, -4, 13.5, 14.2, square_render2, 1);
	DrawCube(-2.6, -1.9, -3, -4, 15.7, 16.5, square_render2, 1);
	DrawCube(3.9, 4.6, -3, -4, 13.5, 14.2, square_render2, 1);
	DrawCube(3.9, 4.6, -3, -4, 15.7, 16.5, square_render2, 1);

	DrawCube(-4.35, 6.35, -3.6, -2.15, 20.5, 23.75, square_render2, 1);
	DrawCube(-4.2, -3.7, -3.6, -4, 20.65, 21.2, square_render2, 1);
	DrawCube(-4.2, -3.7, -3.6, -4, 23.05, 23.6, square_render2, 1);
	DrawCube(5.7, 6.2, -3.6, -4, 23.05, 23.6, square_render2, 1);
	DrawCube(5.7, 6.2, -3.6, -4, 20.65, 21.2, square_render2, 1);
	DrawCube(-4.35, -3.6, -1.7, -2.15, 20.5, 23.75, square_render2, 1);
	DrawCube(5.6, 6.35, -1.7, -2.15, 20.5, 23.75, square_render2, 1);
	DrawCube(-3.0, -3.8, -1.3, -2.15, 20.5, 23.55, square_render2, 1);
	DrawCube(5.8, 5, -1.3, -2.15, 20.5, 23.55, square_render2, 1);
	DrawCube(-3.8, 5.8, -0.95, -2.15, 22.8, 23.55, square_render2, 1);
	DrawCube(-4.35, 6.35, -1.7, -2.15, 23.3, 23.75, square_render2, 1);
	DrawCube(-3.8, 5.8, -1.32, -2.15, 22.8, 22.53, square_render2, 1);
	DrawCube(-3.8, 5.8, -1.7, -2.15, 22.8, 22.4, square_render2, 1);

	DrawCube(-0.4, 1.6, -1.7, -2.15, 21.1, 22.4, square_render2, 1);
	DrawCube(-0.35, 1, -1.7, -2.15, 20.65, 21.1, square_render2, 1);
	DrawCube(0.2, 1.7, -1.7, -1.4, 21.1, 22.4, square_render2, 1);
	DrawCube(0.05, 0.7, -1.4, -1, 21.32, 22.3, square_render2, 1);
	DrawCube(0.15, 0.7, -1, -0.5, 21.32, 22.3, square_render2, 1);
	DrawCube(0.7, 1.85, -1.4, -0.5, 20.9, 22.4, square_render2, 1);
	DrawCube(0.8, 1.85, 0.1, -0.5, 20.9, 22, square_render2, 1);
	DrawCube(0.15, 0.8, 0.1, -0.5, 21.32, 22.1, square_render2, 1);


	DrawCube(-3.45, 1.85, -2.3, -3.63, -3.1, -1.4, square_render2, 1);
	DrawCube(-3.62, 2.10, -2.4, -2., -3.2, -1.3, square_render2, 1);
	DrawCube(-1.9, 0.3, -1.9, -2., -2.5, -1.4, square_render2, 1);
	DrawCube(-3.7, 2.10, -1.8, 1.4, -2.2, -1.4, square_render2, 1);
	DrawCube(-1.2, -0.4, -1.9, -1, -2.4, -1.9, square_render2, 1);

	DrawCube(-3.45, -3.05, -3.63, -4, -3.1, -2.7, square_render2, 1);
	DrawCube(1.45, 1.85, -3.63, -4, -3.1, -2.7, square_render2, 1);
	DrawCube(-3.45, -3.05, -3.63, -4, -1.8, -1.4, square_render2, 1);
	DrawCube(1.45, 1.85, -3.63, -4, -1.8, -1.4, square_render2, 1);

	DrawCube(-3.05, 1.45, -3.63, -3.85, -1.67, -1.4, square_render2, 1);

	DrawCube(1.45, 1.85, -3.63, -4, -1.8, -1.4, square_render2, 1);

	DrawCube(-11.85, 11.85, 3.85, -3.85, -3.85, 24.85, square_render2, 1);
	
	//DrawCube(1.0, -2.3, 21.0, 1.5, -2.2, 21.0, square_render2, 1);

	x = 0.9f; y = -0.28f; z = 21.5f; w = 0.4f; v = -0.43;
	GLfloat squaree[] = {
		x,y,z,
		w,y,z,
		w,v,z,
		x,y,z,
		x,v,z,
		w,v,z
	};
	square_render2.push_back(new square_renderer(ResourceManager::GetShader("square"), squaree));
	GLfloat tv[] = {
		0.50f,  1.2f,  -1.6f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		0.50f,  -1.37f, -1.6f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		-1.0f, 1.2f, -1.6f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		-1.0f, 1.2f, -1.6f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		0.50f, -1.37f,  -1.6f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		-1.0f,  -1.37f,  -1.6f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
	};
	Bare_renders.push_back(new bare_sprite_renderer(ResourceManager::GetShader("baresprite"), far_bare_wall_vertices, 6 * 8));
	bare_tex_color.push_back((char*)"wallpaper");
	Bare_renders.push_back(new bare_sprite_renderer(ResourceManager::GetShader("baresprite"), near_bare_wall_vertices, 6 * 8));
	bare_tex_color.push_back((char*)"wallpaper");
	Bare_renders.push_back(new bare_sprite_renderer(ResourceManager::GetShader("baresprite"), left_bare_wall_vertices, 6 * 8));
	bare_tex_color.push_back((char*)"wallpaper");
	Bare_renders.push_back(new bare_sprite_renderer(ResourceManager::GetShader("baresprite"), right_bare_wall1_vertices, 6 * 8));
	bare_tex_color.push_back((char*)"wallpaper");
	Bare_renders.push_back(new bare_sprite_renderer(ResourceManager::GetShader("baresprite"), right_bare_wall2_vertices, 6 * 8));
	bare_tex_color.push_back((char*)"wallpaper");
	Bare_renders.push_back(new bare_sprite_renderer(ResourceManager::GetShader("baresprite"), right_bare_wall3_vertices, 6 * 8));
	bare_tex_color.push_back((char*)"wallpaper");
	Bare_renders.push_back(new bare_sprite_renderer(ResourceManager::GetShader("baresprite"), right_bare_wall4_vertices, 6 * 8));
	bare_tex_color.push_back((char*)"wallpaper");
	Bare_renders.push_back(new bare_sprite_renderer(ResourceManager::GetShader("baresprite"), up_bare_wall_vertices, 6 * 8));
	bare_tex_color.push_back((char*)"wallpaper");
	Bare_renders.push_back(new bare_sprite_renderer(ResourceManager::GetShader("baresprite"), tv, 6 * 8));
	bare_tex_color.push_back((char*)"tv0");
	
	Renderer2.push_back(new SpriteRenderer(ResourceManager::GetShader("sprite"), down_bare_wall_vertices, 6 * 8));
	tex_color2.push_back((char*)"floor2");
	tex_spec2.push_back((char*)"floor2_spec");
	tex_norm2.push_back((char*)"floor2_norm");
	tex_high2.push_back((char*)"floor2_high");

	Model_renders2.push_back(new ModelRenderer(ResourceManager::GetShader("model"), "obj/room2_obj/dad/hm.obj"));
	Model_place2.push_back(glm::vec3(-7.0, -3.73, 12.8));
	Model_size2.push_back(glm::vec3(2.7, 2.7, 2.7));
	Model_rotate2.push_back(40.0);
	Model_renders2.push_back(new ModelRenderer(ResourceManager::GetShader("model"), "obj/room2_obj/mom_and_child/hm3.obj"));
	Model_place2.push_back(glm::vec3(1.0, -2.35, 21.7));
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
	Model_place2.push_back(glm::vec3(-0.8, -2.1, -2));
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

	sky_renders = (new skybox_renderer(ResourceManager::GetShader("sky"), skyboxVertices));

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
	std::vector <std::string> cosmic
	{
		"obj/skybox/mp_drakeq/drakeq_ft.tga",
		"obj/skybox/mp_drakeq/drakeq_bk.tga",
		"obj/skybox/mp_drakeq/drakeq_up.tga",
		"obj/skybox/mp_drakeq/drakeq_dn.tga",
		"obj/skybox/mp_drakeq/drakeq_rt.tga",
		"obj/skybox/mp_drakeq/drakeq_lf.tga"
	};
	ResourceManager::LoadSkybox(faces, "sky");
	ResourceManager::LoadSkybox(cosmic, "cosmic");
	tex_sky = (char*)"sky";

	GLint i, j, terrace = 6, delta = 4;
	for (i = 1; i <= MC_W; i++)
		for (j = 1; j <= MC_H; j++)
			if (!((i % delta) && (j % delta)))
				heightm[i][j] = PerlinNoise(i, j);
			else
			{
				GLint ti = i / delta * delta, tj = j / delta * delta;
				heightm[i][j] = (PerlinNoise(ti, tj)*(i - ti + j - tj) +
					PerlinNoise(ti + delta, tj)*(ti + delta - i + j - tj) +
					PerlinNoise(ti, tj + delta)*(i - ti + tj + delta - j) +
					PerlinNoise(ti + delta, tj + delta)*(ti + delta - i + tj + delta - j)) / 4 / delta;
			}

	cube.initRenderData();
	vox.init(ResourceManager::GetShader("voxel"));
	tex.init(ResourceManager::GetShader("text"));
	glEnable(GL_DEPTH_TEST);
}

GLint check(GLfloat* square, Camera* camera) {
	glm::vec3 pos = camera->Position;
	glm::vec3 front = camera->Front;
	glm::vec3 vert[4];
	for (int i = 0; i < 4; i++) {
		vert[i] = glm::vec3(*(square + 3 * i + 0), *(square + 3 * i + 1), *(square + 3 * i + 2));
	}
	glm::vec3 plane = get_plane_coefficient(vert);
	GLfloat k = (1 - plane[0] * pos[0] - plane[1] * pos[1] - plane[2] * pos[2]) / (plane[0] * front[0] + plane[1] * front[1] + plane[2] * front[2]);
	if (k < 0.0) {
		return 0;
	}
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

GLfloat ballon_count = 0.0f;
GLint isExit = 0;
GLint checkball(Camera* camera) {
	glm::vec3 pos = camera->Position;
	glm::vec3 front = camera->Front;
	glm::vec3 ball(7.5, -5, 5.5);
	GLfloat r = 0.2, s = 0;
	if (glm::dot(front, ball - pos) < 0) {
		s = glm::length(pos - ball);
	}
	else {
		glm::vec3 t = front;
		s = glm::length(glm::cross(t, ball - pos)) / glm::length(t);
	}
	return s<r;
}

void Game::Update(GLfloat dt)
{
	if (this->State == GAME_ROOM2) {
		time_cnt_for_words += dt;
	}
	//printf("%f %f %f %f %f %f %f %f\n", this->camera->Position[0], this->camera->Position[1], this->camera->Position[2], this->camera->Front[0], this->camera->Front[1], this->camera->Front[2], this->camera->Yaw, this->camera->Pitch);
	if (!lock_camera)
	{
		if (this->State == GAME_ROOM3)
		{
			if (checkball(this->camera)) {
				ballon_count += dt;
			}
			else
				ballon_count = 0.0f;
			if (ballon_count > 5.0f) {
				isExit = 1;
			}
		}
		if (check(this->trigger_square[this->State], this->camera)) {
			this->Effects->Blur = 1;
			this->stare_count += dt;
			if (this->stare_count > TRIGGER_TIME) {
				if (this->State == GAME_ROOM1)
				{
					this->lock_camera = 1;
					this->transfer_count = 0;
					this->Effects->Shake = 1;
					this->pos = this->camera->Position;
					this->frt = this->camera->Front;
					time_cnt_for_words = 0;
				}
				else
				{
					((int&)this->State)++;
					this->transfer_count = 0;
					this->lock_camera = 1;
					this->Effects->Chaos = 1;
				}
			}
		}
		else {
			this->stare_count = 0.0;
			this->pos = this->camera->Position;
			this->frt = this->camera->Front;
			this->Effects->Blur = 0;
		}
		this->Effects->updateBlur(this->stare_count);
	}
	else
	{
		if (this->State == GAME_ROOM1)
		{
			this->camera->Position = this->pos + (targetpos[0] - this->pos)*glm::vec3(this->transfer_count / 3.0, this->transfer_count / 3.0, this->transfer_count / 3.0);
			this->camera->Front = this->frt + (targetfrt[0] - this->frt)*glm::vec3(this->transfer_count / 3.0, this->transfer_count / 3.0, this->transfer_count / 3.0);
			transfer_count += dt;
			if (transfer_count > 3.0)
			{
				this->lock_camera = 0;
				((int&)this->State)++;
				this->camera->resetvector(-154.85, -0.0);
				this->stare_count = 0.0;
				this->Effects->Shake = 0;
				this->camera->Position = glm::vec3(10.0, 0.0, 23.0);
				this->camera->Front = glm::vec3(-0.893552, -0.159881, -0.419527);
				this->firstMouse = 1;
				//this->camera->
			}
		}
		else
		{
			this->camera->Position = this->pos + (targetpos[1] - this->pos)*glm::vec3(this->transfer_count / 10.0, this->transfer_count / 10.0, this->transfer_count / 10.0);
			this->camera->Front = this->frt + (targetfrt[1] - this->frt)*glm::vec3(this->transfer_count / 10.0, this->transfer_count / 10.0, this->transfer_count / 10.0);
			this->transfer_count += dt;
			if (transfer_count > 10.0)
			{
				this->lock_camera = 0;
				this->stare_count = 0.0;
				this->Effects->Chaos = 0;
				this->firstMouse = 1;
				this->camera->resetvector(-406.25, -0.0);
			}
		}
	}
}

int sign(GLfloat a)
{
	if (a < 0) return -1;
	if (a < 1e-8&&a>-1e-8) return 0;
	return 1;
}

GLint Game::check_collision(glm::vec3 dot1, glm::vec3 dot2)
{
	if (dot1 == dot2) {
		return 0;
	}
	int siz = planes[this->State].size();
	for (int i = 0; i < siz; i++) {
		glm::vec3 v[4];
		for (int j = 0; j < 4; j++) {
			v[j] = glm::vec3(planes[this->State][i][j][0], planes[this->State][i][j][1], planes[this->State][i][j][2]);
		}
		glm::vec3 v1 = v[0] - dot1;
		glm::vec3 v2 = v[1] - dot1;
		glm::vec3 v3 = v[2] - dot1;
		glm::vec3 v4 = v[0] - dot2;
		glm::vec3 v5 = v[1] - dot2;
		glm::vec3 v6 = v[2] - dot2;
		if (sign(dot(cross(v1, v2), v3))*sign(dot(cross(v4, v5), v6)) > 0) {
			continue;
		}
		int flag, a = 0, b = 1, col = 1;
		for (int j = 0; j < 4; j++) {
			int t = sign(dot(cross(v[a] - dot2, v[b] - dot2), dot1 - dot2));
			if (j == 0) flag = t;
			else if (t != flag) {
				col = 0;
				break;
			}
			a++, b++;
			if (b == 4) b = 0;
		}
		if (col) return puts("!"), 1;
	}
	return 0;
}

void Game::grab(void)
{
	FILE*    pDummyFile;  //指向另一bmp文件，用于复制它的文件头和信息头数据
	FILE*    pWritingFile;  //指向要保存截图的bmp文件
	GLubyte* pPixelData;    //指向新的空的内存，用于保存截图bmp文件数据
	GLubyte  BMP_Header[BMP_Header_Length];
	GLint    i, j;
	GLint    PixelDataLength;   //BMP文件数据总长度

								// 计算像素数据的实际长度
	i = this->Width * 3;   // 得到每一行的像素数据长度
	while (i % 4 != 0)      // 补充数据，直到i是的倍数
		++i;
	PixelDataLength = i * this->Height;  //补齐后的总位数

										 // 分配内存和打开文件
	pPixelData = (GLubyte*)malloc(PixelDataLength);
	if (pPixelData == 0)
		exit(0);

	pDummyFile = fopen("screengrab/bitmap1.bmp", "rb");//只读形式打开
	if (pDummyFile == 0)
		exit(0);

	pWritingFile = fopen("screengrab/grab.bmp", "wb"); //只写形式打开
	if (pWritingFile == 0)
		exit(0);

	//把读入的bmp文件的文件头和信息头数据复制，并修改宽高数据
	fread(BMP_Header, sizeof(BMP_Header), 1, pDummyFile);  //读取文件头和信息头，占据54字节
	fwrite(BMP_Header, sizeof(BMP_Header), 1, pWritingFile);
	fseek(pWritingFile, 0x0012, SEEK_SET); //移动到0X0012处，指向图像宽度所在内存
	i = this->Width;
	j = this->Height;
	fwrite(&i, sizeof(i), 1, pWritingFile);
	fwrite(&j, sizeof(j), 1, pWritingFile);

	// 读取当前画板上图像的像素数据
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);  //设置4位对齐方式
	glReadPixels(0, 0, this->Width, this->Height,
		GL_BGR_EXT, GL_UNSIGNED_BYTE, pPixelData);

	// 写入像素数据
	fseek(pWritingFile, 0, SEEK_END);
	//把完整的BMP文件数据写入pWritingFile
	fwrite(pPixelData, PixelDataLength, 1, pWritingFile);

	// 释放内存和关闭文件
	fclose(pDummyFile);
	fclose(pWritingFile);
	free(pPixelData);
}

void Game::ProcessInput(GLfloat dt)
{
	//keyboard input
	if (!lock_camera)
	{
		glm::vec3 prev_position = this->camera->Position;
		if (this->Keys[GLFW_KEY_W] == GLFW_PRESS) {
			this->camera->ProcessKeyboard(FORWARD, dt);
		}
		if (this->Keys[GLFW_KEY_S] == GLFW_PRESS) {
			this->camera->ProcessKeyboard(BACKWARD, dt);
		}
		if (this->Keys[GLFW_KEY_A] == GLFW_PRESS) {
			this->camera->ProcessKeyboard(LEFT, dt);
		}
		if (this->Keys[GLFW_KEY_D] == GLFW_PRESS) {
			this->camera->ProcessKeyboard(RIGHT, dt);
		}
		if (this->Keys[GLFW_KEY_P] == GLFW_PRESS) {
			grab();
		}
		if (this->Keys[GLFW_KEY_C] == GLFW_PRESS) {
			bare_tex_color.pop_back();
			bare_tex_color.push_back((char*)"tv2");
			tv_channel = 2;
		}
		if (this->Keys[GLFW_KEY_X] == GLFW_PRESS) {
			bare_tex_color.pop_back();
			bare_tex_color.push_back((char*)"tv1");
			tv_channel = 1;
		}
		if (this->Keys[GLFW_KEY_Z] == GLFW_PRESS) {
			bare_tex_color.pop_back();
			bare_tex_color.push_back((char*)"tv0");
			tv_channel = 0;
		}
		printf("%d ", tv_channel);
		if (check_collision(prev_position, this->camera->Position)) {
			this->camera->Position = prev_position;
		}
	}
}

void Game::ProcessMouseMovement(GLfloat dt) {
	//mouse input
	if (!lock_camera)
	{
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
}

void Game::ProcessScrollMovement(GLfloat yoffset) {
	//scroll input
	if (!lock_camera)
	{
		this->camera->ProcessMouseScroll(yoffset);
	}
}

void Game::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	this->Effects->BeginRender();
	switch (this->State) {
	case GAME_ROOM1: {
		std::vector<SpriteRenderer*>::iterator si;
		std::vector<GLchar*>::iterator tci, tsi, tni, thi;
		for (si = Renderer.begin(),
			tci = tex_color.begin(),
			tsi = tex_spec.begin(),
			tni = tex_norm.begin(),
			thi = tex_high.begin();
			si != Renderer.cend();
			si++, tci++, tsi++, tni++, thi++
			)
			(*si)->DrawSprite(this->camera, this->Height, this->Width, ResourceManager::GetTexture(*tci), ResourceManager::GetTexture(*tsi), ResourceManager::GetTexture(*tni), ResourceManager::GetTexture(*thi), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), 0.0f, glm::vec3(0.0f, 1.0f, 0.0f));

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
			(*li)->Draw(this->camera, this->Height, this->Width, glm::vec3((li == Light_renders.begin()) ? -2.5 : 2.5, 0.0, -1.5));

#ifdef SQUARE
		std::vector<square_renderer*>::iterator sqi;
		for (sqi = square_render.begin(); sqi != square_render.cend(); sqi++)
			(*sqi)->Draw(this->camera, this->Height, this->Width);
#endif 
		sky_renders->Draw(this->camera, ResourceManager::GetSkybox("sky"), this->Height, this->Width);
		tex.paintText("Welcome to this new world.", 65.0f, 65.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));
		tex.paintText("Focus on what you are insterested in.", 65.0f, 35.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));
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
		if (time_cnt_for_words > 3.0 && time_cnt_for_words <12.0)
			tex.paintText("(Hope you have not got dissy in the journey QAQ)", 35.0f, 35.0f + 50.f*(int(time_cnt_for_words - 3) / 3), 1.0f, glm::vec3(0.0, 0.0f, 0.0f));
		if (time_cnt_for_words > 6.0 && time_cnt_for_words <15.0)
			tex.paintText("(Anyway, you have entered another world!)", 35.0f, 35.0f + 50.f*(int(time_cnt_for_words - 6) / 3), 1.0f, glm::vec3(0.0, 0.0f, 0.0f));
		if (time_cnt_for_words > 9.0 && time_cnt_for_words <18.0)
			tex.paintText("MAN: Hey, where would you like to go for vacation?", 35.0f, 35.0f + 50.f*(int(time_cnt_for_words - 9) / 3), 1.0f, glm::vec3(0.0, 0.0f, 0.0f));
		if (time_cnt_for_words > 12.0 && time_cnt_for_words <21.0)
			tex.paintText("Woman: WOW! That a good and sweet question!", 35.0f, 35.0f + 50.f*(int(time_cnt_for_words - 12) / 3), 1.0f, glm::vec3(0.0, 0.0f, 0.0f));
		if (time_cnt_for_words > 15.0 && time_cnt_for_words <24.0)
			tex.paintText("Woman: maybe we should ask the baby first~", 35.0f, 35.0f + 50.f*(int(time_cnt_for_words - 15) / 3), 1.0f, glm::vec3(0.0, 0.0f, 0.0f));
		if (time_cnt_for_words > 18.0 && time_cnt_for_words <27.0)
			tex.paintText("Baby: ....", 35.0f, 35.0f + 50.f*(int(time_cnt_for_words - 18) / 3), 1.0f, glm::vec3(0.0, 0.0f, 0.0f));
		if (time_cnt_for_words > 21.0 && time_cnt_for_words <30.0)
			tex.paintText("(When you want to know about somebody's mind)", 35.0f, 35.0f + 50.f*(int(time_cnt_for_words - 21) / 3), 1.0f, glm::vec3(0.0, 0.0f, 0.0f));
		if (time_cnt_for_words > 24.0 && time_cnt_for_words <33.0)
			tex.paintText("(Looking into his eyes may be a good idea.)", 35.0f, 35.0f + 50.f*(int(time_cnt_for_words - 24) / 3), 1.0f, glm::vec3(0.0, 0.0f, 0.0f));

		break;
	}
	case GAME_ROOM3: {
		if (!isExit)
		{
			glDisable(GL_CULL_FACE);//面剔除

			glm::vec3 pos = glm::vec3(0, 0, 0);
			glm::vec3 siz = glm::vec3(1, 1, 1);
			GLfloat rot = 0.0f;
			//tex.paintText("This is sample text", 65.0f, 65.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));
			vox.drawtree1(this->camera, this->Height, this->Width, glm::vec3(4.5, -6, 10.5), GLfloat(0.4)*siz, glm::vec3(1.0, 0.5, 0.0));
			vox.drawtree1(this->camera, this->Height, this->Width, glm::vec3(7.5, -5, 10.5), GLfloat(0.4)*siz, glm::vec3(1.0, 0.5, 0.0));
			vox.drawtree1(this->camera, this->Height, this->Width, glm::vec3(4.5, -5, 6.5), GLfloat(0.4)*siz, glm::vec3(1.0, 0.5, 0.0));
			vox.drawtree1(this->camera, this->Height, this->Width, glm::vec3(3.5, -6, 3.5), GLfloat(0.4)*siz, glm::vec3(1.0, 0.5, 0.0));
			vox.drawtree1(this->camera, this->Height, this->Width, glm::vec3(1.5, -6, 1.5), GLfloat(0.4)*siz, glm::vec3(1.0, 0.5, 0.0));
			vox.drawtree1(this->camera, this->Height, this->Width, glm::vec3(10.5, -5, 10.5), GLfloat(0.4)*siz, glm::vec3(1.0, 0.5, 0.0));
			vox.drawtree2(this->camera, this->Height, this->Width, glm::vec3(8.5, -4, 7.5), GLfloat(0.2)*siz, glm::vec3(1.0, 0.5, 0.0));
			vox.drawtree2(this->camera, this->Height, this->Width, glm::vec3(6.5, -4, 4.5), GLfloat(0.2)*siz, glm::vec3(1.0, 0.5, 0.0));
			vox.drawtree3(this->camera, this->Height, this->Width, glm::vec3(7.5, -6, 5.5), GLfloat(0.4)*siz, glm::vec3(1.0, 0.5, 0.0));
			//sky_renders->Draw(this->camera, ResourceManager::GetSkybox("sky"), this->Height, this->Width);
			GLint i, j, k;
			glEnable(GL_CULL_FACE);//面剔除
			for (i = 1; i <= MC_H; i++)
				for (j = 1; j <= MC_W; j++)
				{
					GLint h = GLint(5 * (heightm[i][j])) + 1;
					for (k = 1; k <= h; k++)
					{
						pos = glm::vec3(i, k - 10, j);
						cube.Draw(this->camera, this->Height, this->Width, pos, siz, rot, k <= 2 ? 2 : k != h);
					}
				}
			sky_renders->Draw(this->camera, ResourceManager::GetSkybox("cosmic"), this->Height, this->Width);
			tex.paintText("There is an entire cosmic in every baby's eyes.", 35.0f, 35.0f, 1.0f, glm::vec3(1.0, 1.0f, 1.0f));
		}
		else {
			tex.paintText("Thank you for playing our games.", 35.0f, 85.0f, 1.0f, glm::vec3(1.0, 1.0f, 1.0f));
			tex.paintText("What we want to deliver is that focusing can ", 35.0f, 55.0f, 1.0f, glm::vec3(1.0, 1.0f, 1.0f));
			tex.paintText("make you find sth interesting and get into other's heart ", 35.0f, 25.0f, 1.0f, glm::vec3(1.0, 1.0f, 1.0f));
		}
		break;
	}
	default: {
		break;
	}
	}
	this->Effects->EndRender();
	this->Effects->Render(glfwGetTime());
}