#include "mc_grass_cube.h"
#include "resource_manager.h"

std::vector<bare_sprite_renderer*>mc_grass_cube::mc_renders;
std::vector<GLchar*> mc_grass_cube::mc_tex_color;

GLfloat mc_grass_cube::farside[] = {
	// positions          // normals           // texture coords
	//far
	0.0f,  0.0f, 0.0f,  -1.0f,  0.0f, 0.0f,  1.0f / 16,  1.0f / 16,
	0.0f,  1.0f, 0.0f,  -1.0f,  0.0f, 0.0f,  1.0f / 16,  0.0f,
	1.0f,  0.0f, 0.0f,  -1.0f,  0.0f, 0.0f,  1.0f / 8,   1.0f / 16,
	1.0f,  0.0f, 0.0f,  -1.0f,  0.0f, 0.0f,  1.0f / 8,   1.0f / 16,
	0.0f,  1.0f, 0.0f,  -1.0f,  0.0f, 0.0f,  1.0f / 16,  0.0f,
	1.0f,  1.0f, 0.0f,  -1.0f,  0.0f, 0.0f,  1.0f / 8,   0.0f,
};
GLfloat mc_grass_cube::nearside[] = {
	//near
	0.0f, 0.0f, 1.0f,  0.0f,  0.0f, -1.0f,  1.0f / 16,  1.0f / 16,
	0.0f, 1.0f, 1.0f,  0.0f,  0.0f, -1.0f,  1.0f / 16,  0.0f,
	1.0f,  0.0f, 1.0f,  0.0f,  0.0f, -1.0f,  1.0f / 8,  1.0f / 16,
	1.0f,  0.0f, 1.0f,  0.0f,  0.0f, -1.0f,  1.0f / 8,  1.0f / 16,
	0.0f,  1.0f, 1.0f,  0.0f,  0.0f, -1.0f,  1.0f / 16,  0.0f,
	1.0f, 1.0f, 1.0f,  0.0f,  0.0f, -1.0f,  1.0f / 8, 0.0f,
};
GLfloat mc_grass_cube::leftside[] = {
	//left
	0.0f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f / 16,  1.0f / 16,
	0.0f,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f,  1.0f / 8,  1.0f / 16,
	0.0f, 1.0f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f / 16,  0.0f,
	0.0f, 1.0f, 0.0f, -1.0f,  0.0f,  0.0f,  1.0f / 16,  0.0f,
	0.0f, 0.0f,  1.0f, -1.0f,  0.0f,  0.0f,  1.0f / 8,  1.0f / 16,
	0.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f,  1.0f / 8,  0.0f,
};
GLfloat mc_grass_cube::rightside[] = {
	//right
	1.0f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f / 16,  1.0f / 16,
	1.0f,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f,  1.0f / 8,  1.0f / 16,
	1.0f, 1.0f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f / 16,  0.0f,
	1.0f, 1.0f, 0.0f, -1.0f,  0.0f,  0.0f,  1.0f / 16,  0.0f,
	1.0f, 0.0f,  1.0f, -1.0f,  0.0f,  0.0f,  1.0f / 8,  1.0f / 16,
	1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f,  1.0f / 8,  0.0f,
};
GLfloat mc_grass_cube::downside[] = {
	//down
	0.0f,  0.0f, 0.0f,  0.0f,  1.0f,  0.0f,  2.0f / 16,  0.0f,
	1.0f,  0.0f, 0.0f,  0.0f,  1.0f,  0.0f,  2.0f / 16,  1.0f / 16,
	0.0f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,  3.0f / 16,  0.0f,
	0.0f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,  3.0f / 16,  0.0f,
	1.0f,  0.0f,  0.0f,  0.0f,  1.0f,  0.0f,  2.0f / 16,  1.0f / 16,
	1.0f,  0.0f, 1.0f,  0.0f,  1.0f,  0.0f,  3.0f / 16,  1.0f / 16
};
GLfloat mc_grass_cube::upside[] = {
	//up
	0.0f,  1.0f, 0.0f,  0.0f,  1.0f,  0.0f,  0.0f/16,  0.0f,
	1.0f,  1.0f, 0.0f,  0.0f,  1.0f,  0.0f,  0.0f/16,  1.0f/16,
	0.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  1.0f/16,  0.0f,
	0.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  1.0f/16,  0.0f,
	1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f/16,  1.0f/16,
	1.0f,  1.0f, 1.0f,  0.0f,  1.0f,  0.0f,  1.0f/16,  1.0f/16
};

mc_grass_cube::mc_grass_cube()
{

}

mc_grass_cube::~mc_grass_cube()
{

}

void mc_grass_cube::Draw(Camera * camera, GLfloat H, GLfloat W, glm::vec3 position, glm::vec3 size, GLfloat rotate)
{
	std::vector<bare_sprite_renderer*>::iterator bsi;
	std::vector<GLchar*>::iterator bci;
	for (bsi = mc_renders.begin(),
		bci = mc_tex_color.begin();
		bsi != mc_renders.cend();
		bsi++, bci++) {
		(*bsi)->Draw(camera, H, W, ResourceManager::GetTexture(*bci), position, size, rotate, glm::vec3(0.0f, 1.0f, 0.0f));
	}
}

void mc_grass_cube::initRenderData()
{
	
	mc_renders.push_back(new bare_sprite_renderer(ResourceManager::GetShader("baresprite"), nearside, 6 * 8));
	mc_tex_color.push_back((char*)"mc2");
	mc_renders.push_back(new bare_sprite_renderer(ResourceManager::GetShader("baresprite"), leftside, 6 * 8));
	mc_tex_color.push_back((char*)"mc2");
	mc_renders.push_back(new bare_sprite_renderer(ResourceManager::GetShader("baresprite"), rightside, 6 * 8));
	mc_tex_color.push_back((char*)"mc2");
	mc_renders.push_back(new bare_sprite_renderer(ResourceManager::GetShader("baresprite"), upside, 6 * 8));
	mc_tex_color.push_back((char*)"mc2");
	mc_renders.push_back(new bare_sprite_renderer(ResourceManager::GetShader("baresprite"), farside, 6 * 8));
	mc_tex_color.push_back((char*)"mc2");
	mc_renders.push_back(new bare_sprite_renderer(ResourceManager::GetShader("baresprite"), downside, 6 * 8));
	mc_tex_color.push_back((char*)"mc2");
}
