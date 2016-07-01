#pragma once

#include <GL/glut.h>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

struct GL_model_material
{
public:
	float shininess;
	float ambient[4];
	float diffuse[4];
	float specular[4];
	float emission[4];
};

struct GL_model_triangle
{
	int ver_pos[3];
	int ver_nor[3];
};

struct GL_model_group
{
	int min, max;
	int triangle_num;
	GL_model_triangle* triangle=NULL;
};

class GL_model
{
public:
	bool finished = false;
	bool can_draw = false;
	float* ver=NULL;
	float* nor=NULL;
	int ver_num;
	int nor_num;
	GLuint list_pos;
	int group_num = 0;
	string* group_name=NULL;
	GL_model_material* material=NULL;
	GL_model_group* group=NULL;

	void copy_to(GL_model& model);
	void save_ori(string fil);
	void scale(double ratio_a, double ratio_b, double ratio_c);
	void translate(double ratio_a, double ratio_b, double ratio_c);
	void rotate(double ratio_a, double ratio_b, double ratio_c);
	void init(string fil);
	void copy_color(GL_model mo);
	void load_obj(string fil);
	void destroy();
	void draw(bool show[], double eye_angle[], GLuint dr);
	void draw();
	void creat_list();
	void list_draw();
	void less_draw();
	void no_color_draw();
};
