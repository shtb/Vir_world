#pragma once

#include <string>
#include <fstream>
#include <sstream>

using namespace std;

struct coll_model_material
{
public:
	float shininess;
	float ambient[4];
	float diffuse[4];
	float specular[4];
	float emission[4];
};

struct coll_model_triangle
{
	int ver_pos[3];
	int ver_nor[3];
};

struct coll_model_group
{
	int min, max;
	int triangle_num;
	coll_model_triangle* triangle=NULL;
};

class coll_model
{
public:
	float* ver=NULL;
	float* const_ver=NULL;
	int ver_num;
	int group_num = 0;
	string* group_name=NULL;
	coll_model_group* group=NULL;

	void reset();
	void copy_to(coll_model& model);
	void save_ori(string fil);
	void scale(double ratio_a, double ratio_b, double ratio_c);
	void translate(double ratio_a, double ratio_b, double ratio_c);
	void rotate(double ratio_a, double ratio_b, double ratio_c);
    void rotate_b(double ratio_b);
	void init(string fil);
	void destroy();
};
