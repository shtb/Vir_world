#pragma once

#include <iostream>
#include <string>
#include <fstream>

using namespace std;

enum file_index_enum
{
    enum_file_config,
    enum_file_gra_config,
    enum_file_world,
    enum_file_font,
    enum_file_model_muLin,
    enum_file_model_snow,
    enum_file_model_gear,
    enum_file_model_contrast,
    enum_file_model_hail,
    enum_file_model_maple,
    enum_file_model_petal,
    enum_file_model_magic,
    enum_file_model_tree,
    enum_file_model_grass_eat,
    enum_file_model_meat_eat,
    enum_file_model_fly_eat,
    enum_file_all_num
};

class file_index
{
public:
	file_index();
	void file_check();
	~file_index();

	const static int file_num = enum_file_all_num;
    string pub_path = "../../data/gra/";
	string files[file_num];
};
