#pragma once

#include <ft2build.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <map>
#include "shader.h"

#include FT_FREETYPE_H  

struct Character {
	GLuint     TextureID;  // 字形纹理的ID
	glm::ivec2 Size;       // 字形大小
	glm::ivec2 Bearing;    // 从基准线到字形左部/顶部的偏移值
	GLuint     Advance;    // 原点距下一个字形原点的距离
};


class text {
public:
	text();
	~text();
	void init(Shader);
	void paintText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);
private:
	GLuint textVAO, textVBO;
	Shader shader;
};
