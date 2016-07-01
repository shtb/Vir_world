#include "world_switch.h"
#include"../base/interfaces.h"

world_switch::world_switch()
{
}

void world_switch::init()
{
	memset(world_map,0,11*11*4);
	int x=5,y=5;
	int dir=4;
	world_map[x][y]=1;
	for(int i=2; i<=using_world_num; ++i)
	{
		switch(dir)
		{
		case(1):
		{
			if(world_map[x+1][y]==0)
			{
				x++;
				dir=2;
			}
			else
			{
				y++;
			}
			world_map[x][y]=i;
		}
		break;
		case(2):
		{
			if(world_map[x][y-1]==0)
			{
				y--;
				dir=3;
			}
			else
			{
				x++;
			}
			world_map[x][y]=i;
		}
		break;
		case(3):
		{
			if(world_map[x-1][y]==0)
			{
				x--;
				dir=4;
			}
			else
			{
				y--;
			}
			world_map[x][y]=i;
		}
		break;
		case(4):
		{
			if(world_map[x][y+1]==0)
			{
				y++;
				dir=1;
			}
			else
			{
				x--;
			}
			world_map[x][y]=i;
		}
		break;
		default:
			break;
		}
	}
	refresh_texture_file_name();
}

void world_switch::get_xy_pos(int&x,int&y)
{
	for(int i=1; i<10; ++i)
	{
		for(int j=1; j<10; ++j)
		{
			if(world_map[i][j]==this_world_order)
			{
				x=i;
				y=j;
				return;
			}
		}
	}
	refresh_texture_file_name();
	my_print.error("it is a wrong world_order!:"+my_file.files[enum_file_world]);
}

void world_switch::save()
{
	save_this_world(this_world_order);
	my_print.message("world_saved");
}

bool world_switch::switch_world(int dir)
{
	int pos_x,pos_y;
	get_xy_pos(pos_x,pos_y);
	switch(dir)
	{
	case(enum_direction_east):
	{
		if(world_map[pos_x+1][pos_y]==0)
		{
			return false;
		}
		else
		{
			save_this_world(this_world_order);
			this_world_order=world_map[pos_x+1][pos_y];
			load_this_world(this_world_order);
			refresh_texture_file_name();
			return true;
		}
	}
	break;
	case(enum_direction_west):
	{
		if(world_map[pos_x-1][pos_y]==0)
		{
			return false;
		}
		else
		{
			save_this_world(this_world_order);
			this_world_order=world_map[pos_x-1][pos_y];
			load_this_world(this_world_order);
			refresh_texture_file_name();
			return true;
		}
	}
	break;
	case(enum_direction_south):
	{
		if(world_map[pos_x][pos_y-1]==0)
		{
			return false;
		}
		else
		{
			save_this_world(this_world_order);
			this_world_order=world_map[pos_x][pos_y-1];
			load_this_world(this_world_order);
			refresh_texture_file_name();
			return true;
		}
	}
	break;
	case(enum_direction_north):
	{
		if(world_map[pos_x][pos_y+1]==0)
		{
			return false;
		}
		else
		{
			save_this_world(this_world_order);
			this_world_order=world_map[pos_x][pos_y+1];
			load_this_world(this_world_order);
			refresh_texture_file_name();
			return true;
		}
	}
	break;
	default:
		break;
	}
	return false;
}

void world_switch::refresh_texture_file_name()
{
	stringstream ss;
	ss<<"../gra/texture/";
	ss<<this_world_order;
	my_file.files[enum_file_world]=my_file.pub_path+ss.str();
}

void world_switch::load_this_world(int world_order)
{
    world_order++;
    world_order--;
}

void world_switch::save_this_world(int world_order)
{
world_order++;
world_order--;
}

world_switch::~world_switch()
{
}
