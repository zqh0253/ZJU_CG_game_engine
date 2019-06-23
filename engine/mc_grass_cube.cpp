#include "mc_grass_cube.h"
#include "resource_manager.h"

std::vector<bare_sprite_renderer*>mc_grass_cube::mc_renders[4];
std::vector<GLchar*> mc_grass_cube::mc_tex_color[4];

GLfloat farside[] = {
	// positions          // normals           // texture coords
	//far
	0.0f,  0.0f, 0.0f,  -1.0f,  0.0f, 0.0f,  1.0f / 16,  1.0f / 16,
	0.0f,  1.0f, 0.0f,  -1.0f,  0.0f, 0.0f,  1.0f / 16,  0.0f,
	1.0f,  0.0f, 0.0f,  -1.0f,  0.0f, 0.0f,  1.0f / 8,   1.0f / 16,
	1.0f,  0.0f, 0.0f,  -1.0f,  0.0f, 0.0f,  1.0f / 8,   1.0f / 16,
	0.0f,  1.0f, 0.0f,  -1.0f,  0.0f, 0.0f,  1.0f / 16,  0.0f,
	1.0f,  1.0f, 0.0f,  -1.0f,  0.0f, 0.0f,  1.0f / 8,   0.0f,
};
GLfloat nearside[] = {
	//near
	0.0f, 0.0f, 1.0f,  0.0f,  0.0f, -1.0f,  1.0f / 16,  1.0f / 16,
	1.0f,  0.0f, 1.0f,  0.0f,  0.0f, -1.0f,  1.0f / 8,  1.0f / 16,
	0.0f, 1.0f, 1.0f,  0.0f,  0.0f, -1.0f,  1.0f / 16,  0.0f,
	1.0f,  0.0f, 1.0f,  0.0f,  0.0f, -1.0f,  1.0f / 8,  1.0f / 16,
	1.0f, 1.0f, 1.0f,  0.0f,  0.0f, -1.0f,  1.0f / 8, 0.0f,
	0.0f,  1.0f, 1.0f,  0.0f,  0.0f, -1.0f,  1.0f / 16,  0.0f,
};
GLfloat leftside[] = {
	//left
	0.0f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f / 16,  1.0f / 16,
	0.0f,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f,  1.0f / 8,  1.0f / 16,
	0.0f, 1.0f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f / 16,  0.0f,
	0.0f, 1.0f, 0.0f, -1.0f,  0.0f,  0.0f,  1.0f / 16,  0.0f,
	0.0f, 0.0f,  1.0f, -1.0f,  0.0f,  0.0f,  1.0f / 8,  1.0f / 16,
	0.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f,  1.0f / 8,  0.0f,
};
GLfloat rightside[] = {
	//right
	1.0f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f / 16,  1.0f / 16,
	1.0f, 1.0f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f / 16,  0.0f,
	1.0f,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f,  1.0f / 8,  1.0f / 16,
	1.0f, 1.0f, 0.0f, -1.0f,  0.0f,  0.0f,  1.0f / 16,  0.0f,
	1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f,  1.0f / 8,  0.0f,
	1.0f, 0.0f,  1.0f, -1.0f,  0.0f,  0.0f,  1.0f / 8,  1.0f / 16,
};
GLfloat downside[] = {
	//down
	0.0f,  0.0f, 0.0f,  0.0f,  1.0f,  0.0f,  2.0f / 16,  0.0f,
	1.0f,  0.0f, 0.0f,  0.0f,  1.0f,  0.0f,  2.0f / 16,  1.0f / 16,
	0.0f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,  3.0f / 16,  0.0f,
	0.0f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,  3.0f / 16,  0.0f,
	1.0f,  0.0f,  0.0f,  0.0f,  1.0f,  0.0f,  2.0f / 16,  1.0f / 16,
	1.0f,  0.0f, 1.0f,  0.0f,  1.0f,  0.0f,  3.0f / 16,  1.0f / 16
};
GLfloat upside[] = {
	//up
	0.0f,  1.0f, 0.0f,  0.0f,  1.0f,  0.0f,  0.0f / 16,  0.0f,
	0.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  1.0f / 16,  0.0f,
	1.0f,  1.0f, 0.0f,  0.0f,  1.0f,  0.0f,  0.0f / 16,  1.0f / 16,
	0.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  1.0f / 16,  0.0f,
	1.0f,  1.0f, 1.0f,  0.0f,  1.0f,  0.0f,  1.0f / 16,  1.0f / 16,
	1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f / 16,  1.0f / 16,
};


GLfloat farside2[] = {
	// positions          // normals           // texture coords
	//far
	0.0f,  0.0f, 0.0f,  -1.0f,  0.0f, 0.0f,  2.0f / 16,  1.0f / 16,
	0.0f,  1.0f, 0.0f,  -1.0f,  0.0f, 0.0f,  2.0f / 16,  0.0f,
	1.0f,  0.0f, 0.0f,  -1.0f,  0.0f, 0.0f,  3.0f / 16,  1.0f / 16,
	1.0f,  0.0f, 0.0f,  -1.0f,  0.0f, 0.0f,  3.0f / 16,  1.0f / 16,
	0.0f,  1.0f, 0.0f,  -1.0f,  0.0f, 0.0f,  2.0f / 16,  0.0f,
	1.0f,  1.0f, 0.0f,  -1.0f,  0.0f, 0.0f,  3.0f / 16,  0.0f,
};
GLfloat nearside2[] = {
	//near
	0.0f, 0.0f, 1.0f,  0.0f,  0.0f, -1.0f,  2.0f / 16,  1.0f / 16,
	1.0f,  0.0f, 1.0f,  0.0f,  0.0f, -1.0f,  3.0f / 16,  1.0f / 16,
	0.0f, 1.0f, 1.0f,  0.0f,  0.0f, -1.0f,  2.0f / 16,  0.0f,
	1.0f,  0.0f, 1.0f,  0.0f,  0.0f, -1.0f,  3.0f / 16,  1.0f / 16,
	1.0f, 1.0f, 1.0f,  0.0f,  0.0f, -1.0f,  3.0f / 16, 0.0f,
	0.0f,  1.0f, 1.0f,  0.0f,  0.0f, -1.0f,  2.0f / 16,  0.0f,
};
GLfloat leftside2[] = {
	//left
	0.0f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,  2.0f / 16,  1.0f / 16,
	0.0f,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f,  3.0f / 16,  1.0f / 16,
	0.0f, 1.0f,  0.0f, -1.0f,  0.0f,  0.0f,  2.0f / 16,  0.0f,
	0.0f, 1.0f, 0.0f, -1.0f,  0.0f,  0.0f,  2.0f / 16,  0.0f,
	0.0f, 0.0f,  1.0f, -1.0f,  0.0f,  0.0f,  3.0f / 16,  1.0f / 16,
	0.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f,  3.0f / 16,  0.0f,
};
GLfloat rightside2[] = {
	//right
	1.0f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,  2.0f / 16,  1.0f / 16,
	1.0f, 1.0f,  0.0f, -1.0f,  0.0f,  0.0f,  2.0f / 16,  0.0f,
	1.0f,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f,  3.0f / 16,  1.0f / 16,
	1.0f, 1.0f, 0.0f, -1.0f,  0.0f,  0.0f,  2.0f / 16,  0.0f,
	1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f,  3.0f / 16,  0.0f,
	1.0f, 0.0f,  1.0f, -1.0f,  0.0f,  0.0f,  3.0f / 16,  1.0f / 16,
};


GLfloat farside3[] = {
	// positions          // normals           // texture coords
	//far
	0.0f,  0.0f, 0.0f,  -1.0f,  0.0f, 0.0f,  7.0f / 16,  1.0f / 16,
	0.0f,  1.0f, 0.0f,  -1.0f,  0.0f, 0.0f,  7.0f / 16,  0.0f,
	1.0f,  0.0f, 0.0f,  -1.0f,  0.0f, 0.0f,  8.0f / 16,   1.0f / 16,
	1.0f,  0.0f, 0.0f,  -1.0f,  0.0f, 0.0f,  8.0f / 16,   1.0f / 16,
	0.0f,  1.0f, 0.0f,  -1.0f,  0.0f, 0.0f,  7.0f / 16,  0.0f,
	1.0f,  1.0f, 0.0f,  -1.0f,  0.0f, 0.0f,  8.0f / 16,   0.0f,
};
GLfloat nearside3[] = {
	//near
	0.0f, 0.0f, 1.0f,  0.0f,  0.0f, -1.0f,  7.0f / 16,  1.0f / 16,
	1.0f,  0.0f, 1.0f,  0.0f,  0.0f, -1.0f,  8.0f / 16,  1.0f / 16,
	0.0f, 1.0f, 1.0f,  0.0f,  0.0f, -1.0f,  7.0f / 16,  0.0f,
	1.0f,  0.0f, 1.0f,  0.0f,  0.0f, -1.0f,  8.0f / 16,  1.0f / 16,
	1.0f, 1.0f, 1.0f,  0.0f,  0.0f, -1.0f,  8.0f / 16, 0.0f,
	0.0f,  1.0f, 1.0f,  0.0f,  0.0f, -1.0f,  7.0f / 16,  0.0f,
};
GLfloat leftside3[] = {
	//left
	0.0f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,  7.0f / 16,  1.0f / 16,
	0.0f,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f,  8.0f / 16,  1.0f / 16,
	0.0f, 1.0f,  0.0f, -1.0f,  0.0f,  0.0f,  7.0f / 16,  0.0f,
	0.0f, 1.0f, 0.0f, -1.0f,  0.0f,  0.0f,  7.0f / 16,  0.0f,
	0.0f, 0.0f,  1.0f, -1.0f,  0.0f,  0.0f,  8.0f / 16,  1.0f / 16,
	0.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f,  8.0f / 16,  0.0f,
};
GLfloat rightside3[] = {
	//right
	1.0f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,  7.0f / 16,  1.0f / 16,
	1.0f, 1.0f,  0.0f, -1.0f,  0.0f,  0.0f,  7.0f / 16,  0.0f,
	1.0f,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f,  8.0f / 16,  1.0f / 16,
	1.0f, 1.0f, 0.0f, -1.0f,  0.0f,  0.0f,  7.0f / 16,  0.0f,
	1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f,  8.0f / 16,  0.0f,
	1.0f, 0.0f,  1.0f, -1.0f,  0.0f,  0.0f,  8.0f / 16,  1.0f / 16,
};
GLfloat downside3[] = {
	//down
	0.0f,  0.0f, 0.0f,  0.0f,  1.0f,  0.0f,  7.0f / 16,  0.0f,
	1.0f,  0.0f, 0.0f,  0.0f,  1.0f,  0.0f,  7.0f / 16,  1.0f / 16,
	0.0f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,  8.0f / 16,  0.0f,
	0.0f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,  8.0f / 16,  0.0f,
	1.0f,  0.0f,  0.0f,  0.0f,  1.0f,  0.0f,  7.0f / 16,  1.0f / 16,
	1.0f,  0.0f, 1.0f,  0.0f,  1.0f,  0.0f,  8.0f / 16,  1.0f / 16
};
GLfloat upside3[] = {
	//up
	0.0f,  1.0f, 0.0f,  0.0f,  1.0f,  0.0f,  7.0f / 16,  0.0f,
	0.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  8.0f / 16,  0.0f,
	1.0f,  1.0f, 0.0f,  0.0f,  1.0f,  0.0f,  7.0f / 16,  1.0f / 16,
	0.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  8.0f / 16,  0.0f,
	1.0f,  1.0f, 1.0f,  0.0f,  1.0f,  0.0f,  8.0f / 16,  1.0f / 16,
	1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  7.0f / 16,  1.0f / 16,
};
mc_grass_cube::mc_grass_cube()
{

}

mc_grass_cube::~mc_grass_cube()
{

}

void mc_grass_cube::Draw(Camera * camera, GLfloat H, GLfloat W, glm::vec3 position, glm::vec3 size, GLfloat rotate, GLint type)
{
	std::vector<bare_sprite_renderer*>::iterator bsi;
	std::vector<GLchar*>::iterator bci;
	for (bsi = mc_renders[type].begin(),
		bci = mc_tex_color[type].begin();
		bsi != mc_renders[type].cend();
		bsi++, bci++) {
		(*bsi)->Draw(camera, H, W, ResourceManager::GetTexture(*bci), position, size, rotate, glm::vec3(0.0f, 1.0f, 0.0f));
	}
}

void mc_grass_cube::initRenderData()
{

	mc_renders[0].push_back(new bare_sprite_renderer(ResourceManager::GetShader("baresprite"), nearside, 6 * 8));
	mc_tex_color[0].push_back((char*)"mc2");
	mc_renders[0].push_back(new bare_sprite_renderer(ResourceManager::GetShader("baresprite"), leftside, 6 * 8));
	mc_tex_color[0].push_back((char*)"mc2");
	mc_renders[0].push_back(new bare_sprite_renderer(ResourceManager::GetShader("baresprite"), rightside, 6 * 8));
	mc_tex_color[0].push_back((char*)"mc2");
	mc_renders[0].push_back(new bare_sprite_renderer(ResourceManager::GetShader("baresprite"), upside, 6 * 8));
	mc_tex_color[0].push_back((char*)"mc2");
	mc_renders[0].push_back(new bare_sprite_renderer(ResourceManager::GetShader("baresprite"), farside, 6 * 8));
	mc_tex_color[0].push_back((char*)"mc2");
	mc_renders[0].push_back(new bare_sprite_renderer(ResourceManager::GetShader("baresprite"), downside, 6 * 8));
	mc_tex_color[0].push_back((char*)"mc2");

	mc_renders[1].push_back(new bare_sprite_renderer(ResourceManager::GetShader("baresprite"), nearside2, 6 * 8));
	mc_tex_color[1].push_back((char*)"mc2");
	mc_renders[1].push_back(new bare_sprite_renderer(ResourceManager::GetShader("baresprite"), leftside2, 6 * 8));
	mc_tex_color[1].push_back((char*)"mc2");
	mc_renders[1].push_back(new bare_sprite_renderer(ResourceManager::GetShader("baresprite"), rightside2, 6 * 8));
	mc_tex_color[1].push_back((char*)"mc2");
	mc_renders[1].push_back(new bare_sprite_renderer(ResourceManager::GetShader("baresprite"), upside, 6 * 8));
	mc_tex_color[1].push_back((char*)"mc2");
	mc_renders[1].push_back(new bare_sprite_renderer(ResourceManager::GetShader("baresprite"), farside2, 6 * 8));
	mc_tex_color[1].push_back((char*)"mc2");
	mc_renders[1].push_back(new bare_sprite_renderer(ResourceManager::GetShader("baresprite"), downside, 6 * 8));
	mc_tex_color[1].push_back((char*)"mc2");

	mc_renders[2].push_back(new bare_sprite_renderer(ResourceManager::GetShader("baresprite"), nearside3, 6 * 8));
	mc_tex_color[2].push_back((char*)"mc2");
	mc_renders[2].push_back(new bare_sprite_renderer(ResourceManager::GetShader("baresprite"), leftside3, 6 * 8));
	mc_tex_color[2].push_back((char*)"mc2");
	mc_renders[2].push_back(new bare_sprite_renderer(ResourceManager::GetShader("baresprite"), rightside3, 6 * 8));
	mc_tex_color[2].push_back((char*)"mc2");
	mc_renders[2].push_back(new bare_sprite_renderer(ResourceManager::GetShader("baresprite"), upside3, 6 * 8));
	mc_tex_color[2].push_back((char*)"mc2");
	mc_renders[2].push_back(new bare_sprite_renderer(ResourceManager::GetShader("baresprite"), farside3, 6 * 8));
	mc_tex_color[2].push_back((char*)"mc2");
	mc_renders[2].push_back(new bare_sprite_renderer(ResourceManager::GetShader("baresprite"), downside3, 6 * 8));
	mc_tex_color[2].push_back((char*)"mc2");
}