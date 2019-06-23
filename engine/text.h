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
	GLuint     TextureID;  // ���������ID
	glm::ivec2 Size;       // ���δ�С
	glm::ivec2 Bearing;    // �ӻ�׼�ߵ�������/������ƫ��ֵ
	GLuint     Advance;    // ԭ�����һ������ԭ��ľ���
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
