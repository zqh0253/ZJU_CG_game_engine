#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "texture.h"
#include "shader.h"
#include "camera.h"

class voxel
{
public:
	voxel();
	~voxel();
	//����Բ��,����posΪ�ױ�Բ����ά���꣬r�뾶��h�߶�
	void draw_clinder(glm::vec3 pos, GLfloat r, GLfloat h, glm::vec3 color);
	//�������壬����posΪԲ�����꣬r�뾶
	void draw_ball(glm::vec3 pos, GLfloat, glm::vec3 colorr);
	//����Բ׶, ����posΪ�ױ�Բ����ά���꣬r�뾶��h�߶�
	void draw_cone(glm::vec3 pos, GLfloat r, GLfloat h, glm::vec3 color);
private:
	void draw_triangle(glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 color);
};


