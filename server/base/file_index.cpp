#include"file_index.h"
#include"../base/interfaces.h"

file_index::file_index()
{
	files[enum_file_config]="config.conf";
	files[enum_file_world]="../gra/texture/";
	for (int i = 0; i < file_num; ++i)
	{
		files[i] = pub_path + files[i];
	}
}

void file_index::file_check()
{
	ifstream tem_file;
	bool unabridged = 1;

	for (int i = 0; i < file_num; ++i)
	{
		tem_file.open(files[i],ios::binary);
		if (!tem_file)
		{
			unabridged = 0;
			my_print.error("<< " + files[i] + " >> not found!");
		}
		tem_file.close();
	}
	my_print.message("file_check finished!");
	if(!unabridged)
	{
		getchar();
	}
}

file_index::~file_index()
{
}
