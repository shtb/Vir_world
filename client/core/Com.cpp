#include "Com.h"
#include "../base/gra_structs.h"
#include "../base/interfaces.h"
using namespace std;

static unsigned int material_pos=0;
string material_value;
string commands[]= {"3d","accum",  "color", "client_color" , "close"  , "cube_color","cocube_mv","cocube_cp","cocube_cr","cocube_de", "cw", "laser","login", "debug" ,"fog","gear_trans","help","linux","mirror","post","refresh_by_net", "re_link_server", "grain","light ","pic_num", "sav_color","txt","voice","world_circle_time"};

int guess_command(string& ans)
{
	int num=0;
	for(int i=0; i<(int)(sizeof(commands)/sizeof(string)); ++i)
	{
		if(commands[i].find(my_print.typing)<100)
		{
			num++;
			if(num==1)
				ans=commands[i];
			else
				ans+="    "+commands[i];
		}
	}
	return num;
}

Com::Com()
{
	sem_init(&com_sem,0,0);
}

void Com::command_input_receiver(key_board_node key)
{
	if(key.key==9)
	{
		string ans;
		int gn=guess_command(ans);
		if(gn==1)
		{
			my_print.typing=ans;
		}
		else if(gn>1)
			my_print.message(ans);
	}
	if(using_method == sc_input)
	{
		if(input_method_sc_receiver(key))
		{
			key.key = 13;
			goto go_command;
		}
	}
	else
	{
go_command:
		if(key.key == 13)
		{
			string menu;
			menu = my_print.typing;
			my_print.history += my_print.typing_pre + my_print.typing + "\\n";
			my_print.typing = "";
			my_print.typing_pre = "\\cazz > \\cazz";
			if(menu=="3d")
			{
				my_gra.use3d=!my_gra.use3d;
			}
			else if(menu == "accum")
			{
				my_print.message("input a float para(<=0 to close):");
				my_print.con_input() >> my_gra.accum.para;
				if(my_gra.accum.para > 0 && my_gra.accum.para < 1)
				{
					my_gra.accum.init=true;
					my_gra.accum.on = true;
				}
				else if(my_gra.accum.para <= 0)
				{
					my_gra.accum.on = false;
				}
				else
				{
					my_print.warning("the input should be less than 1.0!");
					my_gra.accum.on = false;
				}
			}
			else if (menu == "color")
			{
				my_print.message("input the color name:");
				string file;
				my_print.con_input()>>file;
				ifstream color_file(my_file.pub_path+"color/" + file + ".color", ios::binary | ios::in);
				if (!color_file)
				{
					my_print.message(my_file.pub_path + "color/" + file + ".color file  not found!");
				}
				else
				{
					for (int i = 0; i < my_gra.MuLin_model_display.group_num; ++i)
					{
						color_file.read((char*)&(my_gra.MuLin_model_display.material[i]), sizeof(GL_model_material));
					}
					my_print.message("color changed!");
					my_gra.color_file_name=file;
				}
				color_file.close();
			}
			else if(menu == "client_color")
			{
				float co[4];
				my_print.message("input 4 float between 0 ~ 1:");
				my_print.con_input() >> co[0] >> co[1] >> co[2] >> co[3];
				stringstream ss;
				ss<<"client";
				ss.write((char*)co, sizeof(float) * 4);
				string ssstr=ss.str();
				my_net.send_to("comman",ssstr);
			}
			else if(menu == "close")
			{
				good_bye = true;
			}
			else if(menu=="cube_color")
			{
				my_print.message("input 4 float between 0 ~ 1 (<0 to rand):");
				float co[4];
				my_print.con_input() >> co[0] >> co[1] >> co[2] >> co[3];
				if(co[0]<0)
				{
					my_gra.rand_cube_color=true;
				}
				else
				{
					my_gra.rand_cube_color=false;
					my_gra.construct_cube_color[0]=co[0];
					my_gra.construct_cube_color[1]=co[1];
					my_gra.construct_cube_color[2]=co[2];
					my_gra.construct_cube_color[3]=co[3];
				}
			}
			else if(menu=="cocube_mv")
			{
				if(cub_com==0)
				{
					cub_com='m';
					cub_state=0;
					cub_ss.clear();
					cub_ss.str("");
					cub_ss<<'m';
					my_print.message("three position left:");
				}
				else
				{
					my_print.warning("cube command is already running!");
				}
			}
			else if(menu=="cocube_de")
			{
				if(cub_com==0)
				{
					cub_com='d';
					cub_state=0;
					cub_ss.clear();
					cub_ss.str("");
					cub_ss<<'d';
					my_print.message("two position left:");
				}
				else
				{
					my_print.warning("cube command is already running!");
				}
			}
			else if(menu=="cocube_cr")
			{
				if(cub_com==0)
				{
					cub_com='r';
					cub_state=0;
					cub_ss.clear();
					cub_ss.str("");
					cub_ss<<'r';
					my_print.message("input 4 float between 0 ~ 1 (<0 to rand):");
					float co[4];
					my_print.con_input() >> co[0] >> co[1] >> co[2] >> co[3];
					cub_ss.write((char*)co,sizeof(float)*4);
					my_print.message("two position left:");
				}
				else
				{
					my_print.warning("cube command is already running!");
				}
			}
			else if(menu=="cocube_cp")
			{
				if(cub_com==0)
				{
					cub_com='c';
					cub_state=0;
					cub_ss.clear();
					cub_ss.str("");
					cub_ss<<'c';
					my_print.message("three position left:");
				}
				else
				{
					my_print.warning("cube command is already running!");
				}
			}
			else if(menu == "laser")
			{
				my_print.message("input one unsigned int for interval time:");
				my_print.con_input() >> my_gra.shoot_speed;
			}
			else if(menu=="login")
			{
				string choo;
				my_print.safe_con_input() >> choo;
				my_net.send_to("login1",choo);
			}
			else if(menu == "cw")
			{
				string choo;
				my_print.message("input a string for wear choice"
						     "(with no plural):");
				my_print.con_input() >> choo;
				if(choo == "my")
					my_gra.MuLin_show[0] = my_gra.MuLin_show[3] = my_gra.MuLin_show[11] = my_gra.MuLin_show[12] =my_gra.MuLin_show[13] = false;
				else if(choo == "skirt")
					my_gra.MuLin_show[0] = !my_gra.MuLin_show[0];
				else if(choo == "shoe")
					my_gra.MuLin_show[13] = !my_gra.MuLin_show[13];
				else if(choo == "armlet")
					my_gra.MuLin_show[11] = !my_gra.MuLin_show[11];
				else if(choo == "necktie")
					my_gra.MuLin_show[12] = !my_gra.MuLin_show[12];
				else if(choo == "shawl")
					my_gra.MuLin_show[3] = !my_gra.MuLin_show[3];
				else
					my_gra.MuLin_show[0] = my_gra.MuLin_show[3] = my_gra.MuLin_show[11] = my_gra.MuLin_show[12] =
							my_gra.MuLin_show[13] = true;
				stringstream ss;
				ss<<"cw1234";
				ss.write((char*)my_gra.MuLin_show, sizeof(bool)*20);
				string ssstr=ss.str();
				my_net.send_to("comman",ssstr);
			}
			else if(menu == "fog")
			{
				my_gra.fog.render = !my_gra.fog.render;
			}
			else if(menu=="gear_trans")
			{
				my_gra.gear_transparent=!my_gra.gear_transparent;
			}
			else if(menu == "grain")
			{
				my_print.message("input a string for choice:");
				string name;
				my_print.con_input() >> name;
				int choice = -1;
				if(name == "snow")
					choice = enum_id_snow;
				else if(name == "hail")
					choice = enum_id_hail;
				else if(name == "gear")
					choice = enum_id_gear;
				else if(name == "petal")
					choice = enum_id_petal;
				else if(name == "maple")
					choice = enum_id_maple;
				if(choice == -1)
				{
					my_print.warning("wrong name for choice!");
				}
				else
				{
					stringstream ss;
					int nm;
					virw_position po;
					float co[4];
					po.x = my_gra.look_at->x;
					po.z = my_gra.look_at->z;
					po.y = my_gra.look_at->y;

					my_print.message("input an int for num(0 to close,<0 to origin):");
					my_print.con_input() >> nm;
					if(nm <= 0)
					{
						co[0] = -2;
						nm = 1000;
					}
					else
					{
						my_print.message("input three float for color(<0 to origion):");
						my_print.con_input() >> co[0] >> co[1] >> co[2] >> co[3];
					}
					ss<<"grain1";
					ss.write((char*)&nm, sizeof(int));
					ss.write((char*)&po, sizeof(virw_position));
					ss.write((char*)&choice, sizeof(int));
					ss.write((char*)co, sizeof(float) * 3);
					string ssstr=ss.str();
					my_net.send_to("comman",ssstr);
				}
			}
			else if(menu == "help")
			{
				string s;
				for(int i=0; i<(int)sizeof(commands)/8; ++i)
				{
					s+=commands[i]+"   ";
				}
				my_print.message(s);
			}
			else if(menu == "light")
			{
				double tem = -1;
				my_print.message("please input a double(0~2):");
				my_print.con_input() >> tem;
				if(tem >= 0 && tem <= 2)
				{
					my_gra.light = tem;
					my_print.message("succeed!");
				}
			}
			else if(menu=="linux")
			{
				string tem;
				my_print.message("please input linux command:");
				my_print.con_input() >> tem;
				//tem+="|gra";
				system(tem.c_str());
				//cin>>tem;
				//my_print.message(tem);
			}
			else if(menu=="mirror")
			{
				my_gra.mirror=!my_gra.mirror;
			}
			else if(menu=="refresh_by_net")
			{
				my_gra.refresh_by_net=!my_gra.refresh_by_net;
			}
			else if(menu == "pic_num")
			{
				double tem;
				my_print.message("please input the picture_num(1~150):");
				my_print.con_input() >> tem;
				if(tem > 0 && tem < 151)
				{
					my_gra.picture_num = tem;
					my_print.message("succeed!");
				}
				else
				{
					my_print.message("failed!");
				}
			}
			else if(menu=="post")
			{
				my_gra.post=!my_gra.post;
			}
			else if(menu=="re_link_server")
			{
				my_net.reload=true;
			}
			else if (menu == "sav_color")
			{
				my_print.message("input a string for color name:");
				my_print.con_input()>>my_gra.color_file_name;
				ofstream color(my_file.pub_path + "color/" + my_gra.color_file_name + ".color", ios::binary | ios::out);
				if (!color)
				{
					my_print.error(my_file.pub_path + "color/" + my_gra.color_file_name + ".color  create failed!");
				}
				else
				{
					for (int i = 0; i < my_gra.MuLin_model_display.group_num; ++i)
					{
						color.write((char*)&(my_gra.MuLin_model_display.material[i]), sizeof(GL_model_material));
					}
					my_print.message( "color file created!");
				}
				color.close();
				my_print.message("color saved!");
			}
			else if(menu=="txt")
			{
				string tem;
				my_print.message("please input the path and name:");
				my_print.con_input() >> tem;
				ifstream file(tem.c_str(),ios::in);
				if(file)
				{
					string show;
					my_print.message("input e to next,q to quit!");
					bool next=true;
					while(!good_bye)
					{
						if(next)
						{
							if(getline(file,show))
							{
								my_print.message(show);
								next=false;
							}
							else
							{
								my_print.message("the file is over!");
								break;
							}
						}
						key_board_node key;
						key=my_print.get_current_key();
						while(key.state!=key_down)
						{
							global_input_receiver(key);
							key=my_print.get_current_key();
						}
						if(key.key=='e')
						{
							next=true;
						}
						else if(key.key=='q')
						{
							my_print.message("txt view stop!");
							break;
						}
						else
						{
							if(!global_input_receiver(key))
								my_print.message("input e to next,q to quit!");
						}
					}
				}
				else
				{
					my_print.warning("file open failed!");
				}
			}
			else if(menu=="voice")
			{
				my_voice.voice_on=!my_voice.voice_on;
			}
			else if(menu=="world_circle_time")
			{
				unsigned int ci;
				my_print.message("input the num of circle in every second:");
				my_print.con_input() >> ci;
				if(ci==0)
				{
					my_print.message("fail:the num should >0:");
				}
				ci=1000000/ci;
				stringstream ss;
				ss.write((char*)&ci, sizeof(unsigned int));
				string ssstr=ss.str();
				my_net.send_to("cycle_",ssstr);
			}
			else
			{
				my_print.warning("command check failed!");
			}
			input_receiver = default_input;
			my_print.typing_pre="";
			if(input_lock)
			{
				input_receiver = command_input;
				my_print.typing_pre = "\\cazz-> \\cazz";
			}
		}
		else if(key.key >= 0x20 && key.key <= 0x7E)
		{
			my_print.typing += key.key;
		}
		else if(key.key == 8)
		{
			my_print.typing = my_print.typing.substr(0, my_print.typing.length() - 1);
		}
		else
		{
			global_input_receiver(key);
		}
	}
}

void Com::talk_input_receiver(key_board_node key)
{
	if(using_method == sc_input)
	{
		if(input_method_sc_receiver(key))
		{
			key.key = 13;
			goto go_talk;
		}
	}
	else
	{
go_talk:
		if(key.key == 13)
		{
			if(my_print.typing != "")
			{
				string sen = my_print.typing;
				if(my_states.myself.login_state!=enum_login_shtb)
				{
					my_print.warning("login:");
					string choo;
					my_print.safe_con_input() >> choo;
					my_net.send_to("login1",choo);
				}
				my_net.send_to("st_tlk",sen);
				my_print.add_talk(sen, "shtb");
			}
			my_print.history += my_print.typing_pre + my_print.typing + "\n";
			my_print.typing = my_print.typing_pre = "";
			input_receiver = default_input;
			if(input_lock)
			{
				input_receiver = talk_input;
				my_print.typing_pre = "\\cnnz*> \\cnnz";
			}
		}
		else if(key.key >= 0x20 && key.key <= 0x7E)
		{
			my_print.typing += key.key;
		}
		else if(key.key == 8)
		{
			my_print.typing = my_print.typing.substr(0, my_print.typing.length() - 1);
		}
		else
		{
			global_input_receiver(key);
		}
	}
}

bool Com::input_method_sc_receiver(key_board_node key)
{
	if(key.key == 13)//enter
	{
		input_sc.look_line = 0;
		if(input_sc.typing == "")
			return true;
		my_print.typing += input_sc.typing;
		input_sc.typing = "";
		input_sc.parse();
	}
	else if(key.key >= '1' && key.key <= '9')
	{
		my_print.typing += input_sc.get_answer(key.key - 0x31);
	}
	else if(key.key == 45)//-
	{
		if(input_sc.look_line > 0)
		{
			input_sc.look_line -= 1;
			input_sc.parse();
		}
	}
	else if(key.key == 61)//=
	{
		unsigned ti = input_sc.look_line * 9 + 9;
		if(input_sc.choices.size() > ti)
		{
			input_sc.look_line += 1;
			input_sc.parse();
		}
	}
	else if(key.key == 32)//space
	{
		my_print.typing += input_sc.get_answer(0);
	}
	else if(key.key == 8)//backspace
	{
		if(input_sc.typing != "")
			input_sc.typing = input_sc.typing.substr(0, input_sc.typing.length() - 1);
		else
			my_print.typing = my_print.typing.substr(0, my_print.typing.length() - 1);
		input_sc.look_line = 0;
		input_sc.parse();
	}
	else if(key.key == 1008)
	{
		if(using_method == sc_input)
		{
			input_sc.create_word();
			input_sc.parse();
		}
	}
	else if(key.key >= 32 && key.key <= 126)//this should be last
	{
		input_sc.look_line = 0;
		input_sc.typing += key.key;
		input_sc.parse();
	}
	else
	{
		global_input_receiver(key);
	}
	return false;
}

bool Com::global_input_receiver(key_board_node key)
{
	bool state;
	if(key.state == key_down)
	{
		state = true;
	}
	else
	{
		state = false;
	}

	switch(key.key)
	{
		case(1001):
			keys_on[kF1] = state;
			break;
		case(1002):
			keys_on[kF2] = state;
			break;
		case(1004):
			if(state)
			{
				my_gra.shoot_kind--;
				if(my_gra.shoot_kind>=enum_shoot_max)
					my_gra.shoot_kind=enum_shoot_max-1;
			}
			break;
		case(1005):
			if(state)
			{
				my_gra.shoot_kind++;
				if(my_gra.shoot_kind==enum_shoot_max)
					my_gra.shoot_kind=enum_shoot_cube;
			}
			break;
		case(1006):
			break;
		case(1009):
			if(state)
			{
				if(using_method == sc_input)
				{
					using_method = 0;
					input_sc.show = "";
				}
				else
				{
					using_method = sc_input;
					input_sc.parse();
				}
			}
			break;
		case(1100): // left
			keys_on[kleft] = state;
			break;
		case(1101): // up
			keys_on[kup] = state;
			break;
		case(1102): // right
			keys_on[kright] = state;
			break;
		case(1103): // down
			keys_on[kdown] = state;
			break;
		case(1104): // pgup
			keys_on[kpgup] = state;
			break;
		case(1105): // pgdown
			keys_on[kpgdown] = state;
			break;
		case(1106): // home
			keys_on[khome] = state;
			if(state)
			{
				my_gra.look_at = &my_gra.mulin_look;
			}
			break;
		case(1107): // end
			keys_on[kend] = state;
			if(state)
			{
				double dx = my_gra.look_at->x - my_gra.my_pos.x;
				double dy = my_gra.look_at->y - my_gra.my_pos.y;
				double dz = my_gra.look_at->z - my_gra.my_pos.z;
				double l = sqrt(dx * dx + dy * dy + dz * dz);
				my_gra.eye_focus.x = dx / l + my_gra.my_pos.x;
				my_gra.eye_focus.y = dy / l + my_gra.my_pos.y;
				my_gra.eye_focus.z = dz / l + my_gra.my_pos.z;
			}
			break;
		case(1112):
			keys_on[klshift] = state;
			break;
		case(1113):
			keys_on[krshift] = state;
			break;
		case(1114):
			keys_on[klctrl] = state;
			break;
		case(1115):
			keys_on[krctrl] = state;
			break;
		case(1116):
			keys_on[klalt] = state;
			if(state && my_gra.look_at == &my_gra.mulin_look)
			{
				my_gra.look_at = &my_gra.eye_focus;
				my_gra.eye_focus.x = my_gra.mulin_look.x;
				my_gra.eye_focus.y = my_gra.mulin_look.y;
				my_gra.eye_focus.z = my_gra.mulin_look.z;
			}
			break;
		case(1117):
			keys_on[kralt] = state;
			break;
		default:
			return false;
			break;
	}
	return true;
}

void Com::default_input_receiver(key_board_node key)
{
	switch(key.key)
	{
		case('f'):
		case('F'):
			input_receiver = command_input;
			my_print.typing_pre = "\\cazz-> \\cazz";
			break;
		case('j'):
		case('J'):
			input_receiver = talk_input;
			my_print.typing_pre = "\\cnnz*> \\cnnz";
			break;
		case(1003):
			input_receiver = material_change_input;
			my_print.title="          "+my_gra.Part_name[material_pos]+" : "+material_value+"\n    R G B -   color   + R G B\n    1 2  3 - specular + 7 8  9\n    Q W E - diffuse  + U I O\n    A S D - ambient  + J K L\n    Z X C - emission + M < >\n        F  -  shiness + H \n        B : diffuse=ambient";
			break;
		case('['):
			my_print.history = "";
			break;
		case(']'):
			my_print.system = "";
			break;
		case('\\'):
		{
			if(my_print.talked!="")
				my_print.talked = "";
			else
				my_print.talking="";
		}
			break;
		default:
			global_input_receiver(key);
			break;
	}
}

void Com::material_change_receiver(key_board_node key)
{
	stringstream ss;
	switch (key.key)
	{
		case(13):
			my_print.title="";
			input_receiver=default_input;
			return;
			break;
		case('b') :
		case('B') :
			for (int j = 0; j < 3; ++j)
			{
				my_gra.MuLin_model_display.material[material_pos].diffuse[j] =
						my_gra.MuLin_model_display.material[material_pos].ambient[j];
			}
			break;
		case(61) :
			material_pos++;
			material_pos %= 14;
			break;
		case(45) :
			material_pos--;
			material_pos %= 14;
			break;
		case('1') :
			my_gra.MuLin_model_display.material[material_pos].specular[0] -= 0.01;
			ss << my_gra.MuLin_model_display.material[material_pos].specular[0];
			ss >> material_value;
			break;
		case('2') :
			my_gra.MuLin_model_display.material[material_pos].specular[1] -= 0.01;
			ss << my_gra.MuLin_model_display.material[material_pos].specular[1];
			ss >> material_value;
			break;
		case('3') :
			my_gra.MuLin_model_display.material[material_pos].specular[2] -= 0.01;
			ss << my_gra.MuLin_model_display.material[material_pos].specular[2];
			ss >> material_value;
			break;
		case('q') :
		case('Q') :
			my_gra.MuLin_model_display.material[material_pos].diffuse[0] -= 0.01;
			ss << my_gra.MuLin_model_display.material[material_pos].diffuse[0];
			ss >> material_value;
			break;
		case('w') :
		case('W') :
			my_gra.MuLin_model_display.material[material_pos].diffuse[1] -= 0.01;
			ss << my_gra.MuLin_model_display.material[material_pos].diffuse[1];
			ss >> material_value;
			break;
		case('e') :
		case('E') :
			my_gra.MuLin_model_display.material[material_pos].diffuse[2] -= 0.01;
			ss << my_gra.MuLin_model_display.material[material_pos].diffuse[2];
			ss >> material_value;
			break;
		case('a') :
		case('A') :
			my_gra.MuLin_model_display.material[material_pos].ambient[0] -= 0.01;
			ss << my_gra.MuLin_model_display.material[material_pos].ambient[0];
			ss >> material_value;
			break;
		case('s') :
		case('S') :
			my_gra.MuLin_model_display.material[material_pos].ambient[1] -= 0.01;
			ss << my_gra.MuLin_model_display.material[material_pos].ambient[1];
			ss >> material_value;
			break;
		case('D') :
		case('d') :
			my_gra.MuLin_model_display.material[material_pos].ambient[2] -= 0.01;
			ss << my_gra.MuLin_model_display.material[material_pos].ambient[2];
			ss >> material_value;
			break;
		case('f') :
		case('F') :
			if (my_gra.MuLin_model_display.material[material_pos].shininess < 10 ||
			    my_gra.MuLin_model_display.material[material_pos].shininess>118)
			{
				my_gra.MuLin_model_display.material[material_pos].shininess -= 0.5;
			}
			else
			{
				my_gra.MuLin_model_display.material[material_pos].shininess -= 5;
			}
			ss << my_gra.MuLin_model_display.material[material_pos].shininess;
			ss >> material_value;
			break;
		case('7') :
			my_gra.MuLin_model_display.material[material_pos].specular[0] += 0.01;
			ss << my_gra.MuLin_model_display.material[material_pos].specular[0];
			ss >> material_value;
			break;
		case('8') :
			my_gra.MuLin_model_display.material[material_pos].specular[1] += 0.01;
			ss << my_gra.MuLin_model_display.material[material_pos].specular[1];
			ss >> material_value;
			break;
		case('9') :
			my_gra.MuLin_model_display.material[material_pos].specular[2] += 0.01;
			ss << my_gra.MuLin_model_display.material[material_pos].specular[2];
			ss >> material_value;
			break;
		case('u') :
		case('U') :
			my_gra.MuLin_model_display.material[material_pos].diffuse[0] += 0.01;
			ss << my_gra.MuLin_model_display.material[material_pos].diffuse[0];
			ss >> material_value;
			break;
		case('i') :
		case('I') :
			my_gra.MuLin_model_display.material[material_pos].diffuse[1] += 0.01;
			ss << my_gra.MuLin_model_display.material[material_pos].diffuse[1];
			ss >> material_value;
			break;
		case('o') :
		case('O') :
			my_gra.MuLin_model_display.material[material_pos].diffuse[2] += 0.01;
			ss << my_gra.MuLin_model_display.material[material_pos].diffuse[2];
			ss >> material_value;
			break;
		case('j') :
		case('J') :
			my_gra.MuLin_model_display.material[material_pos].ambient[0] += 0.01;
			ss << my_gra.MuLin_model_display.material[material_pos].ambient[0];
			ss >> material_value;
			break;
		case('k') :
		case('K') :
			my_gra.MuLin_model_display.material[material_pos].ambient[1] += 0.01;
			ss << my_gra.MuLin_model_display.material[material_pos].ambient[1];
			ss >> material_value;
			break;
		case('l') :
		case('L') :
			my_gra.MuLin_model_display.material[material_pos].ambient[2] += 0.01;
			ss << my_gra.MuLin_model_display.material[material_pos].ambient[2];
			ss >> material_value;
			break;
		case('h') :
		case('H') :
			if (my_gra.MuLin_model_display.material[material_pos].shininess < 10 ||
			    my_gra.MuLin_model_display.material[material_pos].shininess>118)
			{
				my_gra.MuLin_model_display.material[material_pos].shininess += 0.5;
			}
			else
			{
				my_gra.MuLin_model_display.material[material_pos].shininess += 5;
			}
			ss << my_gra.MuLin_model_display.material[material_pos].shininess;
			ss >> material_value;
			break;
		case('m') :
		case('M') :
			my_gra.MuLin_model_display.material[material_pos].emission[0] += 0.01;
			ss << my_gra.MuLin_model_display.material[material_pos].emission[0];
			ss >> material_value;
			break;
		case(44) :
		case(60) :
			my_gra.MuLin_model_display.material[material_pos].emission[1] += 0.01;
			ss << my_gra.MuLin_model_display.material[material_pos].emission[1];
			ss >> material_value;
			break;
		case(46) :
		case(62) :
			my_gra.MuLin_model_display.material[material_pos].emission[2] += 0.01;
			ss << my_gra.MuLin_model_display.material[material_pos].emission[2];
			ss >> material_value;
			break;
		case('z') :
		case('Z') :
			my_gra.MuLin_model_display.material[material_pos].emission[0] -= 0.01;
			ss << my_gra.MuLin_model_display.material[material_pos].emission[0];
			ss >> material_value;
			break;
		case('x') :
		case('X') :
			my_gra.MuLin_model_display.material[material_pos].emission[1] -= 0.01;
			ss << my_gra.MuLin_model_display.material[material_pos].emission[1];
			ss >> material_value;
			break;
		case('c') :
		case('C') :
			my_gra.MuLin_model_display.material[material_pos].emission[2] -= 0.01;
			ss << my_gra.MuLin_model_display.material[material_pos].emission[2];
			ss >> material_value;
			break;
		default:
			global_input_receiver(key);
			break;
	}
	my_gra.MuLin_model_display.material[head] = my_gra.MuLin_model_display.material[chest];
	my_gra.MuLin_model_display.material[leg] = my_gra.MuLin_model_display.material[chest];
	my_print.title="          "+my_gra.Part_name[material_pos]+" : "+material_value+"\n    R G B -   color   + R G B\n    1 2  3 - specular + 7 8  9\n    Q W E - diffuse  + U I O\n    A S D - ambient  + J K L\n    Z X C - emission + M < >\n        F  -  shiness + H \n        B : diffuse=ambient";
}

int Com::t_load()
{
	input_sc.init();
	return 0;
}

int Com::t_loop()
{
	sem_wait(&com_sem);
	while(!key_board.empty())
	{
		key_board_node the_key;
		the_key = key_board.front();
		key_board.pop();
		if(the_key.state == key_down)
		{
			switch(input_receiver)
			{
				case(default_input):
					default_input_receiver(the_key);
					break;
				case(material_change_input):
					material_change_receiver(the_key);
					break;
				case(command_input):
					command_input_receiver(the_key);
					break;
				case(talk_input):
					talk_input_receiver(the_key);
					break;
				default:
					cout << "unknown key receiver!" << endl;
					input_receiver = default_input;
					break;
			}
		}
		else
			global_input_receiver(the_key);
	}
	return 0;
}

int Com::t_save()
{
	input_sc.save();
	return 0;
}

Com::~Com()
{
}
