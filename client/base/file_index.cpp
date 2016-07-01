#include "file_index.h"
#include "interfaces.h"

file_index::file_index()
{
	files[enum_file_config]="config.conf";
    files[enum_file_gra_config]="gra_config";
	files[enum_file_model_contrast] = "model/contrast.con";
	files[enum_file_model_gear] = "model/gear.ori";
	files[enum_file_model_magic] = "model/magic.ori";
	files[enum_file_model_maple] = "model/maple.ori";
	files[enum_file_model_muLin] = "model/mulin.ori";
	files[enum_file_model_petal] = "model/petal.ori";
	files[enum_file_model_hail] = "model/hail.ori";
	files[enum_file_model_snow] = "model/snow.ori";
	files[enum_file_model_grass_eat] = "model/grass_eat.ori";
	files[enum_file_model_tree] = "model/tree.ori";
	files[enum_file_model_meat_eat] = "model/meat_eat.ori";
	files[enum_file_model_fly_eat] = "model/fly_eat.ori";
	files[enum_file_world] = "texture/11";
	files[enum_file_font] = "font/shtb.font";
	for(int i = 0; i < file_num; ++i)
	{
		files[i] = pub_path + files[i];
	}
}

void file_index::file_check()
{
	ifstream tem_file;
	bool unabridged = 1;

	for(int i = 0; i < file_num; ++i)
	{
		tem_file.open(files[i], ios::binary);
		if(!tem_file)
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
