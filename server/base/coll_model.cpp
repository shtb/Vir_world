#include "coll_model.h"
#include "interfaces.h"

void coll_model::copy_to(coll_model& model)
{
	model.ver_num = this->ver_num;
	model.ver = new float[3 * ver_num + 3];
	model.group_num = this->group_num;
	model.group_name = new string[group_num];
	model.group = new coll_model_group[group_num];
	for(int i = 0; i < 3 * ver_num + 3; ++i)
	{
		model.ver[i] = this->ver[i];
	}
	for(int i = 0; i < group_num; ++i)
	{
		model.group_name[i] = this->group_name[i];
		model.group[i].triangle_num = this->group[i].triangle_num;
		model.group[i].min = this->group[i].min;
		model.group[i].max = this->group[i].max;
		model.group[i].triangle = new coll_model_triangle[this->group[i].triangle_num];
		for(int j = 0; j < group[i].triangle_num; ++j)
		{
			model.group[i].triangle[j] = this->group[i].triangle[j];
		}
	}
}

void coll_model::scale(double ratio_a, double ratio_b, double ratio_c)
{
	for(int i = 3; i < (3 * ver_num + 3); i += 3)
	{
		ver[i] *= ratio_a;
		ver[i + 1] *= ratio_b;
		ver[i + 2] *= ratio_c;
	}
}

void coll_model::translate(double ratio_a, double ratio_b, double ratio_c)
{
	for(int i = 3; i < (3 * ver_num + 3); i += 3)
	{
		ver[i] += ratio_a;
		ver[i + 1] += ratio_b;
		ver[i + 2] += ratio_c;
	}
}

void coll_model::rotate(double ratio_a, double ratio_b, double ratio_c)
{
	ratio_a/=57.2957795;
	ratio_b/=57.2957795;
	ratio_c/=57.2957795;
	for(int i = 3; i < (3 * ver_num + 3); i += 3)
	{
		float cosa=cos(ratio_a);
		float cosb=cos(ratio_b);
		float cosc=cos(ratio_c);
		float sina=sin(ratio_a);
		float sinb=sin(ratio_b);
		float sinc=sin(ratio_c);
		float x=ver[i];
		float y=ver[i+1];
		float z=ver[i+2];
		ver[i]=x*cosc-y*sinc;
		ver[i+1]=x*sinc+y*cosc;
		x=ver[i];
		y=ver[i+1];
		ver[i]=x*cosb+z*sinb;
		ver[i+2]=z*cosb-x*sinb;
		x=ver[i];
		z=ver[i+2];
		ver[i+1]=y*cosa-z*sina;
		ver[i+2]=y*sina+z*cosa;
	}
}

void coll_model::rotate_b(double ratio_b)
{
	ratio_b/=57.2957795;
	for(int i = 3; i < (3 * ver_num + 3); i += 3)
	{

		float cosb=cos(ratio_b);
		float sinb=sin(ratio_b);
		float x=ver[i];
		float z=ver[i+2];
		ver[i]=x*cosb+z*sinb;
		ver[i+2]=z*cosb-x*sinb;
	}
}

void coll_model::reset()
{
	for(int i = 3; i < (3 * ver_num + 3); i += 3)
	{
		ver[i]=const_ver[i];
		ver[i+1]=const_ver[i+1];
		ver[i+2]=const_ver[i+2];
	}
}

void coll_model::init(string fil)
{
	ifstream file(fil, ios::in | ios::binary);
	if(!file)
	{
		my_print.error("file open failed!--" + fil);
	}
	else
	{
		file.read((char*)&ver_num, 4);
		int nor_num;
		file.read((char*)&nor_num, 4);
		ver = new float[ver_num * 3 + 3];
		const_ver=new float[ver_num * 3 + 3];
		file.read((char*)ver, 12 * ver_num + 12);
		memcpy(const_ver, ver, ver_num * 3+3);
		file.seekg(12 * nor_num + 12,ios::cur);
		file.read((char*)&group_num, 4);
		group = new coll_model_group[group_num];
		group_name = new string[group_num];
		for(int i = 0; i < group_num; ++i)
		{
			int len = 0;
			file.read((char*)&len, 4);
			char* s = new char[len + 1];
			file.read(s, len);
			s[len] = '\0';
			group_name[i] = s;
			delete[] s;
			file.read((char*)&group[i].triangle_num, 4);
			file.read((char*)&group[i].min, 4);
			file.read((char*)&group[i].max, 4);
			group[i].triangle = new coll_model_triangle[group[i].triangle_num];
			file.seekg(sizeof(coll_model_material),ios::cur);
			file.read((char*)group[i].triangle, sizeof(coll_model_triangle) * group[i].triangle_num);
		}
	}
}

void coll_model::destroy()
{
	delete[] ver;
	delete[] group;
	delete[] group_name;
	for(int i = 0; i < group_num; ++i)
	{
		delete[] group[i].triangle;
	}
}
