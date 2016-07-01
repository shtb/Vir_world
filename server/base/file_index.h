#pragma once

#include<iostream>
#include<string>
#include<fstream>

using namespace std;

enum file_index_enum { enum_file_config,enum_file_world,enum_file_all_num};

class file_index
{
public:
	file_index();
	void file_check();
	~file_index();

	const static int file_num = enum_file_all_num;
    string pub_path = "../../data/world/";
	string files[file_num];
};
