#include "voxel.h"
#include <cmath>

//GLuint voxel::ballVAO, voxel::cylVAO, voxel::conVAO;

voxel::voxel()
{

}
voxel::~voxel()
{
}

glm::vec3 rotate1(glm::vec3 a, double d)
{
	d = d / 180 * acos(-1);
	glm::vec3 b;
	b.z = a.z;
	b.x = a.x*cos(d) - a.y*sin(d);
	b.y = a.x*sin(d) + a.y*cos(d);
	return b;
}

glm::vec3 rotate2(glm::vec3 a, double d)
{
	d = d / 180 * acos(-1);
	glm::vec3 b;
	b.y = a.y;
	b.x = a.x*cos(d) - a.z*sin(d);
	b.z = a.x*sin(d) + a.z*cos(d);
	return b;
}

void voxel::init(Shader shader)
{
	this->shader = shader;
	glm::vec3 v[4];
	int siz = 10;
	float ball[10 * 10 * 8 * 2 * 3 * 3 * 2];
	int num = 0, g = 0;
	v[0] = glm::vec3(1, 0, 0);
	v[2] = rotate2(v[0], 90 / siz);
	for (int i = 0; i < siz; i++) {
		glm::vec3 nxt = rotate2(v[0], 90.0 / siz);
		for (int j = 0; j < siz; j++) {
			v[1] = rotate1(v[0], 90.0 / siz);
			v[3] = rotate1(v[2], 90.0 / siz);
			for (int fx = -1; fx < 2; fx++)
				for (int fy = -1; fy < 2; fy++)
					for (int fz = -1; fz < 2; fz++) {
						if (fx*fy*fz == 0) continue;
						for (int k = 0; k < 2; k++) {
							glm::vec3 p;
							if (k == 0) {
								p = glm::cross(v[1] - v[0], v[2] - v[0]);
							}
							else {
								p = glm::cross(v[2] - v[3], v[1] - v[3]);
							}
							for (int l = k; l < k + 3; l++) {
								ball[num++] = fx * v[l].x;
								ball[num++] = fy * v[l].y;
								ball[num++] = fz * v[l].z;
								ball[num++] = fx * p.x;
								ball[num++] = fy * p.y;
								ball[num++] = fz * p.z;
							}

						}
					}
			v[0] = v[1];
			v[2] = v[3];
		}
		v[0] = nxt;
		v[2] = rotate2(v[0], 90 / siz);
	}

	unsigned int VBO;
	glGenVertexArrays(1, &ballVAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ball), ball, GL_STATIC_DRAW);

	glBindVertexArray(ballVAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	num = 0;
	float cyl[40 * 4 * 3 * 3 * 2];
	v[0] = glm::vec3(1, 0, 0);
	for (int i = 0; i < siz; i++) {
		v[1] = rotate1(v[0], 90 / siz);
		for (int fx = -1; fx < 2; fx++)
			for (int fy = -1; fy < 2; fy++) {
				int fz = 1;
				glm::vec3 p;
				if (fx*fy*fz == 0) continue;
				p = glm::cross(v[1], v[0]);
				cyl[num++] = fx * 0;
				cyl[num++] = fy * 0;
				cyl[num++] = fz * 0;
				cyl[num++] = fx * p.x;
				cyl[num++] = fy * p.y;
				cyl[num++] = fz * p.z;
				cyl[num++] = fx * v[0].x;
				cyl[num++] = fy * v[0].y;
				cyl[num++] = fz * v[0].z;
				cyl[num++] = fx * p.x;
				cyl[num++] = fy * p.y;
				cyl[num++] = fz * p.z;
				cyl[num++] = fx * v[1].x;
				cyl[num++] = fy * v[1].y;
				cyl[num++] = fz * v[1].z;
				cyl[num++] = fx * p.x;
				cyl[num++] = fy * p.y;
				cyl[num++] = fz * p.z;

				p = glm::cross(v[0], v[1]);
				cyl[num++] = fx * 0;
				cyl[num++] = fy * 0;
				cyl[num++] = fz * 1;
				cyl[num++] = fx * p.x;
				cyl[num++] = fy * p.y;
				cyl[num++] = fz * p.z;
				cyl[num++] = fx * v[0].x;
				cyl[num++] = fy * v[0].y;
				cyl[num++] = fz * 1;
				cyl[num++] = fx * p.x;
				cyl[num++] = fy * p.y;
				cyl[num++] = fz * p.z;
				cyl[num++] = fx * v[1].x;
				cyl[num++] = fy * v[1].y;
				cyl[num++] = fz * 1;
				cyl[num++] = fx * p.x;
				cyl[num++] = fy * p.y;
				cyl[num++] = fz * p.z;

				glm::vec3 tmp = v[0];
				tmp.z = 1;
				p = glm::cross(v[0] - tmp, v[1] - tmp);
				cyl[num++] = fx * v[0].x;
				cyl[num++] = fy * v[0].y;
				cyl[num++] = fz * 1;
				cyl[num++] = fx * p.x;
				cyl[num++] = fy * p.y;
				cyl[num++] = fz * p.z;
				cyl[num++] = fx * v[0].x;
				cyl[num++] = fy * v[0].y;
				cyl[num++] = fz * v[0].z;
				cyl[num++] = fx * p.x;
				cyl[num++] = fy * p.y;
				cyl[num++] = fz * p.z;
				cyl[num++] = fx * v[1].x;
				cyl[num++] = fy * v[1].y;
				cyl[num++] = fz * v[1].z;
				cyl[num++] = fx * p.x;
				cyl[num++] = fy * p.y;
				cyl[num++] = fz * p.z;

				cyl[num++] = fx * v[1].x;
				cyl[num++] = fy * v[1].y;
				cyl[num++] = fz * v[1].z;
				cyl[num++] = fx * p.x;
				cyl[num++] = fy * p.y;
				cyl[num++] = fz * p.z;
				cyl[num++] = fx * v[0].x;
				cyl[num++] = fy * v[0].y;
				cyl[num++] = fz * 1;
				cyl[num++] = fx * p.x;
				cyl[num++] = fy * p.y;
				cyl[num++] = fz * p.z;
				cyl[num++] = fx * v[1].x;
				cyl[num++] = fy * v[1].y;
				cyl[num++] = fz * 1;
				cyl[num++] = fx * p.x;
				cyl[num++] = fy * p.y;
				cyl[num++] = fz * p.z;
			}
		v[0] = v[1];
	}

	glGenVertexArrays(1, &cylVAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cyl), cyl, GL_STATIC_DRAW);

	glBindVertexArray(cylVAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));


	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	num = 0;
	float Con[40 * 2 * 3 * 3 * 2];
	v[0] = glm::vec3(1, 0, 0);
	for (int i = 0; i < siz; i++) {
		v[1] = rotate1(v[0], 90 / siz);
		for (int fx = -1; fx < 2; fx++)
			for (int fy = -1; fy < 2; fy++) {
				int fz = 1;
				glm::vec3 p;
				p = cross(v[1], v[0]);
				if (fx*fy*fz == 0) continue;
				Con[num++] = fx * 0;
				Con[num++] = fy * 0;
				Con[num++] = fz * 0;
				Con[num++] = fx * p.x;
				Con[num++] = fy * p.y;
				Con[num++] = fz * p.z;
				Con[num++] = fx * v[0].x;
				Con[num++] = fy * v[0].y;
				Con[num++] = fz * v[0].z;
				Con[num++] = fx * p.x;
				Con[num++] = fy * p.y;
				Con[num++] = fz * p.z;
				Con[num++] = fx * v[1].x;
				Con[num++] = fy * v[1].y;
				Con[num++] = fz * v[1].z;
				Con[num++] = fx * p.x;
				Con[num++] = fy * p.y;
				Con[num++] = fz * p.z;

				glm::vec3 tmp(0, 0, 1);
				p = cross(v[0] - tmp, v[1] - tmp);
				Con[num++] = fx * 0;
				Con[num++] = fy * 0;
				Con[num++] = fz * 1;
				Con[num++] = fx * p.x;
				Con[num++] = fy * p.y;
				Con[num++] = fz * p.z;
				Con[num++] = fx * v[0].x;
				Con[num++] = fy * v[0].y;
				Con[num++] = fz * v[0].z;
				Con[num++] = fx * p.x;
				Con[num++] = fy * p.y;
				Con[num++] = fz * p.z;
				Con[num++] = fx * v[1].x;
				Con[num++] = fy * v[1].y;
				Con[num++] = fz * v[1].z;
				Con[num++] = fx * p.x;
				Con[num++] = fy * p.y;
				Con[num++] = fz * p.z;

			}
		v[0] = v[1];
	}

	glGenVertexArrays(1, &conVAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Con), Con, GL_STATIC_DRAW);

	glBindVertexArray(conVAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));


	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	float cube[12 * 3 * 3 * 2 * 2];

	num = 0;
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			for (int k = 0; k < 2; k++) {
				if (i + j + k != 2) continue;
				cube[num++] = 0;
				cube[num++] = 0;
				cube[num++] = 0;
				cube[num++] = i - 1;
				cube[num++] = j - 1;
				cube[num++] = k - 1;
				cube[num++] = i;
				cube[num++] = j;
				cube[num++] = k;
				cube[num++] = i - 1;
				cube[num++] = j - 1;
				cube[num++] = k - 1;
				cube[num++] = i;
				cube[num++] = i ? 0 : j;
				cube[num++] = (i + j) ? 0 : k;
				cube[num++] = i - 1;
				cube[num++] = j - 1;
				cube[num++] = k - 1;

				cube[num++] = 0;
				cube[num++] = 0;
				cube[num++] = 0;
				cube[num++] = i - 1;
				cube[num++] = j - 1;
				cube[num++] = k - 1;
				cube[num++] = i;
				cube[num++] = j;
				cube[num++] = k;
				cube[num++] = i - 1;
				cube[num++] = j - 1;
				cube[num++] = k - 1;
				cube[num++] = (k + j) ? 0 : i;
				cube[num++] = k ? 0 : j;
				cube[num++] = k;
				cube[num++] = i - 1;
				cube[num++] = j - 1;
				cube[num++] = k - 1;
			}
		}
	}
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			for (int k = 0; k < 2; k++) {
				if (i + j + k != 1) continue;
				cube[num++] = 1;
				cube[num++] = 1;
				cube[num++] = 1;
				cube[num++] = i;
				cube[num++] = j;
				cube[num++] = k;
				cube[num++] = i;
				cube[num++] = j;
				cube[num++] = k;
				cube[num++] = i;
				cube[num++] = j;
				cube[num++] = k;
				cube[num++] = 1;
				cube[num++] = k ? 0 : 1;
				cube[num++] = k;
				cube[num++] = i;
				cube[num++] = j;
				cube[num++] = k;

				cube[num++] = 1;
				cube[num++] = 1;
				cube[num++] = 1;
				cube[num++] = i;
				cube[num++] = j;
				cube[num++] = k;
				cube[num++] = i;
				cube[num++] = j;
				cube[num++] = k;
				cube[num++] = i;
				cube[num++] = j;
				cube[num++] = k;
				cube[num++] = i;
				cube[num++] = i ? 0 : 1;
				cube[num++] = 1;
				cube[num++] = i;
				cube[num++] = j;
				cube[num++] = k;
			}
		}
	}
lb:;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);

	glBindVertexArray(cubeVAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));


	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}

void voxel::drawball(Camera * camera, GLfloat H, GLfloat W, glm::vec3 position, glm::vec3 size, glm::vec3 color, glm::vec3 rotate, GLfloat a)
{
	static int cnt = 0;
	cnt+=6;
	cnt %= 360;
	this->shader.Use();
	glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom), W / H, 0.1f, 100.0f);
	glm::mat4 view = camera->GetViewMatrix();
	this->shader.SetMatrix4("projection", projection);
	this->shader.SetMatrix4("view", view);

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, position);  // First translate (transformations are: scale happens first, then rotation and then finall translation happens; reversed order)

	model = glm::scale(model, size); // Last scale

	model = glm::rotate(model, glm::radians(a), glm::normalize(rotate)); // Then rotate
	this->shader.SetMatrix4("model", model);
	this->shader.SetVector3f("lightColor", 1.0f, 1.0f, 1.0f);
	this->shader.SetVector3f("lightPos", -2.5*sin(glm::radians((float)cnt)), 0.0, -1.5*cos(glm::radians((float)cnt)));
	this->shader.SetVector3f("viewPos", camera->Position);
	this->shader.SetVector3f("color", color);
	//this->shader.SetInteger("material.diffuse", 0);


	glBindVertexArray(this->ballVAO);
	glDrawArrays(GL_TRIANGLES, 0, 10 * 10 * 8 * 2 * 3);
	glBindVertexArray(0);
}

void voxel::drawcyl(Camera * camera, GLfloat H, GLfloat W, glm::vec3 position, glm::vec3 size, glm::vec3 color, glm::vec3 rotate, GLfloat a)
{
	this->shader.Use();
	glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom), W / H, 0.1f, 100.0f);
	glm::mat4 view = camera->GetViewMatrix();
	this->shader.SetMatrix4("projection", projection);
	this->shader.SetMatrix4("view", view);

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, position);  // First translate (transformations are: scale happens first, then rotation and then finall translation happens; reversed order)

	model = glm::scale(model, size); // Last scale

	model = glm::rotate(model, glm::radians(a), glm::normalize(rotate)); // Then rotate

	this->shader.SetMatrix4("model", model);
	this->shader.SetVector3f("lightColor", 1.0f, 1.0f, 1.0f);
	this->shader.SetVector3f("lightPos", -2.5, 0.0, -1.5);
	this->shader.SetVector3f("color", color);
	//this->shader.SetInteger("material.diffuse", 0);


	glBindVertexArray(this->cylVAO);
	glDrawArrays(GL_TRIANGLES, 0, 40 * 4 * 3);
	glBindVertexArray(0);
}

void voxel::drawcon(Camera * camera, GLfloat H, GLfloat W, glm::vec3 position, glm::vec3 size, glm::vec3 color, glm::vec3 rotate, GLfloat a)
{
	this->shader.Use();
	glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom), W / H, 0.1f, 100.0f);
	glm::mat4 view = camera->GetViewMatrix();
	this->shader.SetMatrix4("projection", projection);
	this->shader.SetMatrix4("view", view);

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, position);  // First translate (transformations are: scale happens first, then rotation and then finall translation happens; reversed order)

	model = glm::scale(model, size); // Last scale

	model = glm::rotate(model, glm::radians(a), glm::normalize(rotate)); // Then rotate

	this->shader.SetMatrix4("model", model);
	this->shader.SetVector3f("lightColor", 1.0f, 1.0f, 1.0f);
	this->shader.SetVector3f("lightPos", -2.5, 0.0, -1.5);
	this->shader.SetVector3f("color", color);
	//this->shader.SetInteger("material.diffuse", 0);


	glBindVertexArray(this->conVAO);
	glDrawArrays(GL_TRIANGLES, 0, 40 * 2 * 3);
	glBindVertexArray(0);
}

void voxel::drawcube(Camera * camera, GLfloat H, GLfloat W, glm::vec3 position, glm::vec3 size, glm::vec3 color, glm::vec3 rotate, GLfloat a)
{
	this->shader.Use();
	glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom), W / H, 0.1f, 100.0f);
	glm::mat4 view = camera->GetViewMatrix();
	this->shader.SetMatrix4("projection", projection);
	this->shader.SetMatrix4("view", view);

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, position);  // First translate (transformations are: scale happens first, then rotation and then finall translation happens; reversed order)

	model = glm::scale(model, size); // Last scale

	model = glm::rotate(model, glm::radians(a), glm::normalize(rotate)); // Then rotate

	this->shader.SetMatrix4("model", model);
	this->shader.SetVector3f("lightColor", 1.0f, 1.0f, 1.0f);
	this->shader.SetVector3f("lightPos", -2.5, 0.0, -1.5);
	this->shader.SetVector3f("color", color);
	//this->shader.SetInteger("material.diffuse", 0);


	glBindVertexArray(this->cubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 12 * 3);
	glBindVertexArray(0);
}

void voxel::drawtree1(Camera * camera, GLfloat H, GLfloat W, glm::vec3 position, glm::vec3 size, glm::vec3 color)
{
	color = glm::vec3(0., 0.4, 0.);
	drawcon(camera, H, W, position + glm::vec3(size.x * 0, size.y * 1, size.z * 0), glm::vec3(size.x, size.y * 2, size.z), color, glm::vec3(1, 0, 0), -90);
	color = glm::vec3(0.63, 0.32, 0.18);
	drawcyl(camera, H, W, position + glm::vec3(size.x * 0, size.y * 0, size.z * 0), glm::vec3(size.x*0.2, size.y * 1, size.z*0.2), color, glm::vec3(1, 0, 0), -90);
}

void voxel::drawtree2(Camera * camera, GLfloat H, GLfloat W, glm::vec3 position, glm::vec3 size, glm::vec3 color)
{
	//shugan
	color = glm::vec3(0.63, 0.32, 0.18);
	drawcube(camera, H, W, position + glm::vec3(size.x * 0, size.y * 0, size.z * 0), size, color, glm::vec3(1, 0, 0), 0);
	drawcube(camera, H, W, position + glm::vec3(size.x * 0, size.y * 1, size.z * 0), size, color, glm::vec3(1, 0, 0), 0);
	drawcube(camera, H, W, position + glm::vec3(size.x * 0, size.y * 2, size.z * 0), size, color, glm::vec3(1, 0, 0), 0);
	drawcube(camera, H, W, position + glm::vec3(size.x * 0, size.y * 3, size.z * 0), size, color, glm::vec3(1, 0, 0), 0);
	drawcube(camera, H, W, position + glm::vec3(size.x * 0, size.y * 4, size.z * 0), size, color, glm::vec3(1, 0, 0), 0);
	//zhicha
	drawcube(camera, H, W, position + glm::vec3(size.x * 1, size.y*2.5, size.z * 0), GLfloat(0.5)*size, color, glm::vec3(1, 0, 0), 0);
	drawcube(camera, H, W, position + glm::vec3(size.x*-0.5, size.y*1.7, size.z*0.5), GLfloat(0.5)*size, color, glm::vec3(1, 0, 0), 0);
	//yezi
	color = glm::vec3(0.13, 0.55, 0.13);
	drawcube(camera, H, W, position + glm::vec3(size.x * 0, size.y * 4, size.z * 0), size, color, glm::vec3(1, 0, 0), 0);
	drawcube(camera, H, W, position + glm::vec3(size.x * 0, size.y * 4, size.z * 1), size, color, glm::vec3(1, 0, 0), 0);
	drawcube(camera, H, W, position + glm::vec3(size.x * 0, size.y * 4, size.z*-1), size, color, glm::vec3(1, 0, 0), 0);
	drawcube(camera, H, W, position + glm::vec3(size.x * 1, size.y * 4, size.z * 0), size, color, glm::vec3(1, 0, 0), 0);
	drawcube(camera, H, W, position + glm::vec3(size.x*-1, size.y * 4, size.z * 0), size, color, glm::vec3(1, 0, 0), 0);
	drawcube(camera, H, W, position + glm::vec3(size.x * 1, size.y * 4, size.z * 1), size, color, glm::vec3(1, 0, 0), 0);
	drawcube(camera, H, W, position + glm::vec3(size.x*-1, size.y * 4, size.z*-1), size, color, glm::vec3(1, 0, 0), 0);
	drawcube(camera, H, W, position + glm::vec3(size.x * 1, size.y * 4, size.z*-1), size, color, glm::vec3(1, 0, 0), 0);
	drawcube(camera, H, W, position + glm::vec3(size.x*-1, size.y * 4, size.z * 1), size, color, glm::vec3(1, 0, 0), 0);
	drawcube(camera, H, W, position + glm::vec3(size.x * 0, size.y * 4, size.z * 2), size, color, glm::vec3(1, 0, 0), 0);
	drawcube(camera, H, W, position + glm::vec3(size.x * 0, size.y * 4, size.z*-2), size, color, glm::vec3(1, 0, 0), 0);
	drawcube(camera, H, W, position + glm::vec3(size.x * 2, size.y * 4, size.z * 0), size, color, glm::vec3(1, 0, 0), 0);
	drawcube(camera, H, W, position + glm::vec3(size.x*-2, size.y * 4, size.z * 0), size, color, glm::vec3(1, 0, 0), 0);
	drawcube(camera, H, W, position + glm::vec3(size.x*-2, size.y * 4, size.z * 1), size, color, glm::vec3(1, 0, 0), 0);
	drawcube(camera, H, W, position + glm::vec3(size.x*-2, size.y * 4, size.z*-1), size, color, glm::vec3(1, 0, 0), 0);
	drawcube(camera, H, W, position + glm::vec3(size.x*-1, size.y * 4, size.z * 2), size, color, glm::vec3(1, 0, 0), 0);
	drawcube(camera, H, W, position + glm::vec3(size.x * 1, size.y * 4, size.z*-2), size, color, glm::vec3(1, 0, 0), 0);

	drawcube(camera, H, W, position + glm::vec3(size.x * 0, size.y * 5, size.z * 0), size, color, glm::vec3(1, 0, 0), 0);
	drawcube(camera, H, W, position + glm::vec3(size.x * 0, size.y * 5, size.z * 1), size, color, glm::vec3(1, 0, 0), 0);
	drawcube(camera, H, W, position + glm::vec3(size.x * 0, size.y * 5, size.z*-1), size, color, glm::vec3(1, 0, 0), 0);
	drawcube(camera, H, W, position + glm::vec3(size.x * 1, size.y * 5, size.z * 0), size, color, glm::vec3(1, 0, 0), 0);
	drawcube(camera, H, W, position + glm::vec3(size.x*-1, size.y * 5, size.z * 0), size, color, glm::vec3(1, 0, 0), 0);
	drawcube(camera, H, W, position + glm::vec3(size.x * 1, size.y * 5, size.z * 1), size, color, glm::vec3(1, 0, 0), 0);
	drawcube(camera, H, W, position + glm::vec3(size.x*-1, size.y * 5, size.z*-1), size, color, glm::vec3(1, 0, 0), 0);
	drawcube(camera, H, W, position + glm::vec3(size.x * 1, size.y * 5, size.z*-1), size, color, glm::vec3(1, 0, 0), 0);
	drawcube(camera, H, W, position + glm::vec3(size.x*-1, size.y * 5, size.z * 1), size, color, glm::vec3(1, 0, 0), 0);
	drawcube(camera, H, W, position + glm::vec3(size.x * 0, size.y * 5, size.z * 2), size, color, glm::vec3(1, 0, 0), 0);
	drawcube(camera, H, W, position + glm::vec3(size.x * 0, size.y * 5, size.z*-2), size, color, glm::vec3(1, 0, 0), 0);
	drawcube(camera, H, W, position + glm::vec3(size.x * 2, size.y * 5, size.z * 0), size, color, glm::vec3(1, 0, 0), 0);
	drawcube(camera, H, W, position + glm::vec3(size.x*-2, size.y * 5, size.z * 0), size, color, glm::vec3(1, 0, 0), 0);
	drawcube(camera, H, W, position + glm::vec3(size.x*-2, size.y * 5, size.z * 2), size, color, glm::vec3(1, 0, 0), 0);
	drawcube(camera, H, W, position + glm::vec3(size.x*-2, size.y * 5, size.z*-2), size, color, glm::vec3(1, 0, 0), 0);
	drawcube(camera, H, W, position + glm::vec3(size.x * 2, size.y * 5, size.z * 2), size, color, glm::vec3(1, 0, 0), 0);
	drawcube(camera, H, W, position + glm::vec3(size.x * 2, size.y * 5, size.z*-2), size, color, glm::vec3(1, 0, 0), 0);
	drawcube(camera, H, W, position + glm::vec3(size.x*-1, size.y * 5, size.z * 2), size, color, glm::vec3(1, 0, 0), 0);
	drawcube(camera, H, W, position + glm::vec3(size.x*-1, size.y * 5, size.z*-2), size, color, glm::vec3(1, 0, 0), 0);
	drawcube(camera, H, W, position + glm::vec3(size.x * 1, size.y * 5, size.z * 2), size, color, glm::vec3(1, 0, 0), 0);
	drawcube(camera, H, W, position + glm::vec3(size.x * 1, size.y * 5, size.z*-2), size, color, glm::vec3(1, 0, 0), 0);
	drawcube(camera, H, W, position + glm::vec3(size.x*-2, size.y * 5, size.z * 1), size, color, glm::vec3(1, 0, 0), 0);
	drawcube(camera, H, W, position + glm::vec3(size.x*-2, size.y * 5, size.z*-1), size, color, glm::vec3(1, 0, 0), 0);
	drawcube(camera, H, W, position + glm::vec3(size.x * 2, size.y * 5, size.z * 1), size, color, glm::vec3(1, 0, 0), 0);
	drawcube(camera, H, W, position + glm::vec3(size.x * 2, size.y * 5, size.z*-1), size, color, glm::vec3(1, 0, 0), 0);

	drawcube(camera, H, W, position + glm::vec3(size.x * 0, size.y * 6, size.z * 0), size, color, glm::vec3(1, 0, 0), 0);
	drawcube(camera, H, W, position + glm::vec3(size.x * 0, size.y * 6, size.z * 1), size, color, glm::vec3(1, 0, 0), 0);
	drawcube(camera, H, W, position + glm::vec3(size.x * 0, size.y * 6, size.z*-1), size, color, glm::vec3(1, 0, 0), 0);
	drawcube(camera, H, W, position + glm::vec3(size.x * 1, size.y * 6, size.z * 0), size, color, glm::vec3(1, 0, 0), 0);
	drawcube(camera, H, W, position + glm::vec3(size.x*-1, size.y * 6, size.z * 0), size, color, glm::vec3(1, 0, 0), 0);
	drawcube(camera, H, W, position + glm::vec3(size.x*-1, size.y * 6, size.z*-1), size, color, glm::vec3(1, 0, 0), 0);
	drawcube(camera, H, W, position + glm::vec3(size.x*-1, size.y * 6, size.z * 1), size, color, glm::vec3(1, 0, 0), 0);
	drawcube(camera, H, W, position + glm::vec3(size.x * 1, size.y * 6, size.z * 1), size, color, glm::vec3(1, 0, 0), 0);
	drawcube(camera, H, W, position + glm::vec3(size.x * 0, size.y * 6, size.z*-2), size, color, glm::vec3(1, 0, 0), 0);
	drawcube(camera, H, W, position + glm::vec3(size.x*-2, size.y * 6, size.z * 0), size, color, glm::vec3(1, 0, 0), 0);

	drawcube(camera, H, W, position + glm::vec3(size.x * 0, size.y * 7, size.z * 0), size, color, glm::vec3(1, 0, 0), 0);
	drawcube(camera, H, W, position + glm::vec3(size.x * 0, size.y * 7, size.z * 1), size, color, glm::vec3(1, 0, 0), 0);
	drawcube(camera, H, W, position + glm::vec3(size.x * 0, size.y * 7, size.z*-1), size, color, glm::vec3(1, 0, 0), 0);
	drawcube(camera, H, W, position + glm::vec3(size.x * 1, size.y * 7, size.z * 0), size, color, glm::vec3(1, 0, 0), 0);
	drawcube(camera, H, W, position + glm::vec3(size.x*-1, size.y * 7, size.z * 0), size, color, glm::vec3(1, 0, 0), 0);

}

void voxel::drawtree3(Camera * camera, GLfloat H, GLfloat W, glm::vec3 position, glm::vec3 size, glm::vec3 color)
{
	color = glm::vec3(1, 0, 0);
	drawball(camera, H, W, position + glm::vec3(size.x * 0, size.y*2.5, 0), GLfloat(0.5)*size, color, glm::vec3(1, 0, 0), 0);
	color = glm::vec3(0, 0, 0);
	drawcyl(camera, H, W, position + glm::vec3(size.x * 0, size.y * 0, size.z * 0), glm::vec3(size.x*0.005, size.y * 2, size.z*0.005), color, glm::vec3(1, 0, 0), -90);
}