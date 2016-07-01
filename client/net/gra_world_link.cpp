#include "gra_world_link.h"
#include"../base/interfaces.h"
#include<sys/time.h>

gra_world_link::gra_world_link()
{
	work_mode=Inte_client;
}

void gra_world_link::send_to(string head,string& my_message)
{
	send_lock.lock();
	if(t_save_sy)
	{
		return;
	}
	if(head.length()!=6)
	{
		my_print.error("send message head length is wrong!");
		return;
	}
	stringstream mess_head;
	mess_head.write(head.c_str(),6);
	int mess_len=my_message.length();
	mess_head.write((char*)&mess_len,sizeof(int));
	string send_message=mess_head.str()+my_message;
	net_send(server_socket, send_message.data(), send_message.length());
	send_lock.unlock();
}

void gra_world_link::send_to(string head,char* my_message, int length)
{
	send_lock.lock();
	if(t_save_sy)
	{
		return;
	}
	if(head.length()!=6)
	{
		my_print.error("send message head length is wrong!");
		return;
	}
	stringstream mess_head;
	mess_head.write(head.c_str(),6);
	mess_head.write((char*)&length,sizeof(int));
	net_send(server_socket, mess_head.str().data(), 10);
	if(length>0)
		net_send(server_socket, my_message,length);
	send_lock.unlock();
}

void gra_world_link::parse(string &choose, stringstream& ss,int)
{
	if(choose == "task_c")
	{
		for(int i = 0; i < my_gra.MuLin_task_num; ++i)
		{
			ss.read((char*)&my_gra.MuLin_task[i].play_on, sizeof(bool));
			ss.read((char*)&my_gra.MuLin_task[i].state, sizeof(double));
		}
		for(int i=0; i<my_gra.MuLin_part_num; ++i)
		{
			ss.read((char*)&my_gra.MuLin_show[i],sizeof(bool));
		}
		ss.read((char*)&my_gra.mulin_pos,sizeof(virw_position));
		ss.read((char*)&my_gra.mulin_distortion, sizeof(virw_position));
		ss.read((char*)(&my_gra.Cr), sizeof(double));
		ss.read((char*)(&my_gra.Cg), sizeof(double));
		ss.read((char*)(&my_gra.Cb), sizeof(double));
		ss.read((char*)(&my_gra.Ca), sizeof(double));
		ss.read((char*)(&my_gra.fog.density), sizeof(float));
		ss.read((char*)(&my_states.time_big), sizeof(int));
		ss.read((char*)(&my_states.time_small), sizeof(int));
		ss.read((char*)(&my_states.world_run_count), sizeof(unsigned int));
		ss.read((char*)(&my_gra.head_eye_angle), sizeof(double)*2);
		ss.read((char*)(&my_gra.elevator_height), sizeof(double));
		int conet=0;
		while(my_gra.net_flush!=false&&my_gra.refresh_by_net&&conet<30)
		{
			conet++;
			usleep(1000);
		}
		my_gra.net_flush = true;
	}
	else if(choose=="bmp123")
	{
		ss.read(my_gra.eye_picture,480*270*3);
		my_gra.eye_pic_ch=true;
		my_gra.big_paint.put_rect(0,330,480,270,(unsigned char*)my_gra.eye_picture);
	}
	else if(choose=="chk_gt")
	{
		my_gra.eye_focus=my_gra.mulin_pos;
		my_gra.eye_focus.y-=my_gra.mulin_distortion.y;
		my_net.send_to("chk_rg",NULL,0);
	}
	else if(choose == "grains")
	{
		int num;
		ss.read((char*)&num, 4);
		for(int i = 0; i < num; ++i)
		{
			ss.read((char*)&(my_gra.grains[i].model_id), sizeof(int));
			ss.read((char*)&(my_gra.grains[i].pos), sizeof(float_virw_position));
			ss.read((char*)&(my_gra.grains[i].color), sizeof(float) * 3);
		}
		my_gra.live_grain_num = num;
	}
	else if(choose == "fly_ea")
	{
		int num;
		ss.read((char*)&num, 4);
		for(int i = 0; i < num; ++i)
		{
			ss.read((char*)&(my_gra.fly_eats[i]), sizeof(float_virw_position));
			ss.read((char*)&(my_gra.fly_eats_big[i]), sizeof(float));
			ss.read((char*)&(my_gra.fly_eats_dead[i]), sizeof(bool));
		}
		my_gra.live_fly_eat_num = num;
	}
	else if(choose == "trees_")
	{
		int num;
		ss.read((char*)&num, 4);
		for(int i = 0; i < num; ++i)
		{
			ss.read((char*)&(my_gra.trees[i]), sizeof(float_virw_position));
			ss.read((char*)&(my_gra.trees_big[i]), sizeof(float));
		}
		my_gra.live_tree_num = num;
	}
	else if(choose == "constr")
	{
		int num;
		ss.read((char*)&num, 4);
		for(int i = 0; i < num; ++i)
		{
			ss.read((char*)&(my_gra.cubes[i]), sizeof(long));
			ss.read((char*)&(my_gra.cubes_color[i*4]), sizeof(float)*4);
		}
		my_gra.live_cube_num = num;
	}
	else if(choose == "meat_e")
	{
		int num;
		ss.read((char*)&num, 4);
		for(int i = 0; i < num; ++i)
		{
			ss.read((char*)&(my_gra.meat_eats[i]), sizeof(float_virw_position));
			ss.read((char*)&(my_gra.meat_eats_big[i]), sizeof(float));
			ss.read((char*)&(my_gra.meat_eats_dead[i]), sizeof(bool));
		}
		my_gra.live_meat_eat_num = num;
	}
	else if(choose == "grass_")
	{
		int num;
		ss.read((char*)&num, 4);
		for(int i = 0; i < num; ++i)
		{
			ss.read((char*)&(my_gra.grass_eats[i]), sizeof(float_virw_position));
			ss.read((char*)&(my_gra.grass_eats_big[i]), sizeof(float));
			ss.read((char*)&(my_gra.grass_eats_dead[i]), sizeof(bool));
		}
		my_gra.live_grass_eat_num = num;
	}
	else if(choose == "shoots")
	{
		my_gra.shoots_lock.lock();
		set<shoot*>::iterator iter;
		for (iter = my_gra.shoots.begin(); iter != my_gra.shoots.end(); iter++)
		{
			delete *iter;
		}
		my_gra.shoots.clear();
		int num;
		ss.read((char*)&num,4);
		for(int i=0; i<num; ++i)
		{
			shoot*p=new shoot;
			ss.read((char*)p,sizeof(shoot));
			my_gra.shoots.insert(p);
		}
		my_gra.shoots_lock.unlock();
	}
	else if(choose=="ml_tlk")
	{
		my_print.add_talk(ss.str(),"mulin");
	}
	else if(choose=="pre_da")
	{
		ss.read((char*)&my_states.myself,sizeof(gra_client));
	}
	else if(choose=="advert")
	{
		int kind;
		ss.read((char*)&kind,4);
		int len;
		ss.read((char*)&len,4);
		char*tem=new char[len+1];
		ss.read(tem,len);
		tem[len]='\0';
		switch(kind)
		{
			case(1):
			{
				my_print.message(tem);
			}
				break;
			case(2):
			{
				my_print.warning(tem);
			}
				break;
			case(3):
			{
				my_print.error(tem);
			}
				break;
			default:
			{
				string p="unknown advertise:";
				my_print.message(p+tem);
			}
				break;
		}
	}
	else if(choose == "gra_cl")
	{
		my_gra.gra_clients_lock.lock();
		set<gra_client*>::iterator iter;
		for (iter = my_gra.gra_clients.begin(); iter != my_gra.gra_clients.end(); iter++)
		{
			delete *iter;
		}
		my_gra.gra_clients.clear();
		int num;
		ss.read((char*)&num,4);
		for(int i=0; i<num; ++i)
		{
			gra_client*p=new gra_client;
			ss.read((char*)p,sizeof(gra_client));
			my_gra.gra_clients.insert(p);
		}
		my_gra.gra_clients_lock.unlock();
	}
	else if(choose == "map_he")
	{
		int len=0;
		ss.read((char*)&len,sizeof(int));
		char *tem=new char[len+1];
		ss.read(tem,len);
		tem[len]='\0';
		my_file.files[enum_file_world]=tem;
		delete[] tem;
		for(int i = 0; i < 41; ++i)
		{
			for(int j = 0; j < 41; ++j)
			{
				ss.read((char*)(&(my_gra.map_height[i * 41 + j])), sizeof(double));
			}
		}
		my_gra.world_changed=0;
	}
	else
	{
		my_print.warning("unknown message:" + choose);
		cout << "unknown message:" << endl;
		cout << "choose:" << choose << endl;
		cout << "length:" << ss.str().length() << endl;
	}
}

gra_world_link::~gra_world_link()
{
}
