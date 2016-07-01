#include "character_controller.h"
#include <fstream>
#include <string.h>
#include "../base/interfaces.h"
using namespace std;

int unicode_to_utf8(unsigned int unic, unsigned char *pOutput)
{
	if ( unic <= 0x0000007F )
	{
		*pOutput     = (unic & 0x7F);
		return 1;
	}
	else if ( unic >= 0x00000080 && unic <= 0x000007FF )
	{
		*(pOutput+1) = (unic & 0x3F) | 0x80;
		*pOutput     = ((unic >> 6) & 0x1F) | 0xC0;
		return 2;
	}
	else if ( unic >= 0x00000800 && unic <= 0x0000FFFF )
	{
		*(pOutput+2) = (unic & 0x3F) | 0x80;
		*(pOutput+1) = ((unic >>  6) & 0x3F) | 0x80;
		*pOutput     = ((unic >> 12) & 0x0F) | 0xE0;
		return 3;
	}
	else if ( unic >= 0x00010000 && unic <= 0x001FFFFF )
	{
		*(pOutput+3) = (unic & 0x3F) | 0x80;
		*(pOutput+2) = ((unic >>  6) & 0x3F) | 0x80;
		*(pOutput+1) = ((unic >> 12) & 0x3F) | 0x80;
		*pOutput     = ((unic >> 18) & 0x07) | 0xF0;
		return 4;
	}
	else if ( unic >= 0x00200000 && unic <= 0x03FFFFFF )
	{
		*(pOutput+4) = (unic & 0x3F) | 0x80;
		*(pOutput+3) = ((unic >>  6) & 0x3F) | 0x80;
		*(pOutput+2) = ((unic >> 12) & 0x3F) | 0x80;
		*(pOutput+1) = ((unic >> 18) & 0x3F) | 0x80;
		*pOutput     = ((unic >> 24) & 0x03) | 0xF8;
		return 5;
	}
	else if ( unic >= 0x04000000 && unic <= 0x7FFFFFFF )
	{
		*(pOutput+5) = (unic & 0x3F) | 0x80;
		*(pOutput+4) = ((unic >>  6) & 0x3F) | 0x80;
		*(pOutput+3) = ((unic >> 12) & 0x3F) | 0x80;
		*(pOutput+2) = ((unic >> 18) & 0x3F) | 0x80;
		*(pOutput+1) = ((unic >> 24) & 0x3F) | 0x80;
		*pOutput     = ((unic >> 30) & 0x01) | 0xFC;
		return 6;
	}
	return 0;
}


character_controller::character_controller()
{
	panels = new panel;
	picture = new GLubyte[3000 * 1030];
}

character_controller::~character_controller()
{
	// dtor
}

void character_controller::init(string file_name)
{
	ifstream file(file_name, ios::in | ios::binary);
	if(!file)
	{
		my_print.error(file_name + "load failed!");
	}
	else
	{
		int c_num = 0;
		character tem;
		while(file.read((char*)&tem.code, sizeof(int)))
		{
			++c_num;
			file.read((char*)&tem.wide, sizeof(int));
			tem.pixels = new GLubyte[24 * tem.wide];
			file.read((char*)tem.pixels, 24 * tem.wide);
			character_map[tem.code] = tem;
		}
		stringstream ss;
		ss << file_name << " loaded, " << c_num << " characters!";
		my_print.message(ss.str());
	}
}

void character_controller::cutstring(string& sen, int wide)
{
	string color;
	int line_byte = wide * 3;
	while(line_byte % 4 != 0)
		++line_byte;
	int pos_x = paper_distance_horizon;
	map<int, character>::iterator ite;
	int font_wide;
	int cha_len;
	size_t ssize=sen.length();
	for(unsigned int i = 0; i <ssize ; ++i)
	{
		int ch = sen[i];
		/////////////////////////
		if(ch < 0)
		{
			int l = -1;
			char c = sen[i];
			while(c < 0)
			{
				c <<= 1;
				l++;
			}
			cha_len=l;
			int uni = 0;
			int tem = 0;
			switch(l)
			{
				case(1):
					tem = sen[i] & 0x1f;
					uni = tem << 6;
					break;
				case(2):
					tem = sen[i] & 0x0f;
					uni = tem << 12;
					break;
				case(3):
					tem = sen[i] & 0x07;
					uni = tem << 18;
					break;
				case(4):
					tem = sen[i] & 0x03;
					uni = tem << 24;
					break;
				case(5):
					tem = sen[i] & 0x01;
					uni = tem << 30;
					break;
				default:
					break;
			}
			while(l--)
			{
				i++;
				int tem = sen[i] & 0x3f;
				uni |= (tem << (l * 6));
			}
			ch = uni;
		}
		///////////////////////
		switch(ch)
		{
			case('\n'):
			{
				if(i + 2 < sen.length())
				{
					sen=color+sen.substr(i+1,string::npos);
					return;
				}
				continue;
			}
				break;
			case('\t'):
			{
				if(pos_x + 48 < wide)
				{
					pos_x +=48 ;
				}
				else
				{
					pos_x=wide;
				}
				continue;
			}
				break;
			case('\\'):
			{
				bool pause_trans = false;
				ch = sen[i + 1];
				switch(ch)
				{
					case('c'):
					{
						if(sen.length() > i + 3)
						{
							color=sen.substr(i,5);
						}
						i += 4;
					}
						break;
					case('n'):
						if(i + 2 < sen.length())
						{
							sen=color+sen.substr(i+2,string::npos);
							return;
						}
						i += 1;
						break;
					case('\\'):
						ch = '\\';
						i += 1;
						pause_trans = true;
						break;
					default:
						ch = '\\';
						pause_trans = true;
						break;
				}
				if(!pause_trans)
					continue;
			}
				break;
			default:
				break;
		}
		ite = character_map.find(ch);
		if(ite != character_map.end())
		{
			font_wide = ite->second.wide;
		}
		else
		{
			ite = character_map.find(-1);
			font_wide = ite->second.wide;
		}
		if(pos_x + font_wide + paper_distance_horizon > wide)
		{
			sen=color+sen.substr(i-cha_len,string::npos);
			return;
		}
		pos_x += font_wide;
	}
}

panel character_controller::get_panel(string sen, int wide)
{
	panel* p = panels->next;
	while(p != NULL)
	{
		if(wide == p->wide && sen == p->sentence)
		{
			panel* r = panels;
			while(r->next != p)
				r = r->next;
			r->next = p->next;
			p->next = panels->next;
			panels->next = p;
			return *p;
		}
		p = p->next;
	}
	static string last_long;
	if(sen.length() > 4000)
	{
		if(last_long == sen)
		{
			sen = sen.substr(0, 4000);
		}
		else
		{
			last_long = sen;
			my_print.warning("the sentence is too long:" + sen.substr(0, 20));
			sen = sen.substr(0, 4000);
		}
	}

	panel_num++;
	p = new panel;
	p->next = panels->next;
	panels->next = p;
	p->sentence = sen;
	p->wide = wide;

	if(sen[sen.length()-1]==' ')
	{
		sen+='|';
	}
	int line_byte = wide * 3;
	while(line_byte % 4 != 0)
		++line_byte;
	if(dark_ground)
	{
		memset(picture, 0, 3000 * 1030);
	}
	else
	{
		memset(picture, 255, 3000 * 1030);
	}
	int pos_x = paper_distance_horizon, pos_y = paper_distance_vertical;
	map<int, character>::iterator ite;
	GLubyte* pixels;
	int font_wide;
	int height;
	char color[3];
	if(dark_ground)
	{
		color[0] = color[1] = color[2] = 25;
	}
	else
	{
		color[0] = color[1] = color[2] = 0;
	}
	size_t ssize=sen.length();
	for(unsigned int i = 0; i <ssize ; ++i)
	{
		int ch = sen[i];
		/////////////////////////
		if(ch < 0)
		{
			int l = -1;
			char c = sen[i];
			while(c < 0)
			{
				c <<= 1;
				l++;
			}
			int uni = 0;
			int tem = 0;
			switch(l)
			{
				case(1):
					tem = sen[i] & 0x1f;
					uni = tem << 6;
					break;
				case(2):
					tem = sen[i] & 0x0f;
					uni = tem << 12;
					break;
				case(3):
					tem = sen[i] & 0x07;
					uni = tem << 18;
					break;
				case(4):
					tem = sen[i] & 0x03;
					uni = tem << 24;
					break;
				case(5):
					tem = sen[i] & 0x01;
					uni = tem << 30;
					break;
				default:
					break;
			}
			while(l--)
			{
				i++;
				int tem = sen[i] & 0x3f;
				uni |= (tem << (l * 6));
			}
			ch = uni;
		}
		///////////////////////
		switch(ch)
		{
			case('\n'):
			{
				pos_x =paper_distance_horizon;
				if(pos_y < 970 && i + 2 < sen.length())
					pos_y +=line_distance;
				continue;
			}
				break;
			case('\r'):
			{
				continue;
			}
				break;
			case('\t'):
			{
				if(pos_x + 48 < wide)
				{
					pos_x +=48 ;
				}
				else
				{
					pos_x=wide;
				}
				continue;
			}
				break;
			case('\\'):
			{
				bool pause_trans = false;
				ch = sen[i + 1];
				switch(ch)
				{
					case('c'):
					{
						if(sen.length() > i + 3)
						{
							color[0] = sen[i + 2] - 'a';
							color[1] = sen[i + 3] - 'a';
							color[2] = sen[i + 4] - 'a';
							if(dark_ground && color[0] < 2 && color[1] < 2 && color[2] < 2)
								color[0] = color[1] = color[2] = 12;
							else if(!dark_ground && color[0] > 24 && color[1] > 24 && color[2] > 24)
								color[0] = color[1] = color[2] = 12;
						}
						i += 4;
					}
						break;
					case('n'):
						pos_x =paper_distance_horizon;
						if(pos_y < 970 && i + 2 < sen.length())
							pos_y +=line_distance;
						i += 1;
						break;
					case('\\'):
						ch = '\\';
						i += 1;
						pause_trans = true;
						break;
					default:
						ch = '\\';
						pause_trans = true;
						break;
				}
				if(!pause_trans)
					continue;
			}
				break;
			default:
				break;
		}
		ite = character_map.find(ch);
		if(ite != character_map.end())
		{
			pixels = ite->second.pixels;
			font_wide = ite->second.wide;
		}
		else
		{
			ite = character_map.find(-1);
			pixels = ite->second.pixels;
			font_wide = ite->second.wide;
		}
		if(pos_x + font_wide + paper_distance_horizon > wide)
		{
			pos_x = paper_distance_horizon;
			if(pos_y < 970)
				pos_y +=line_distance;
		}
		for(int k = 0; k < 24; ++k)
		{
			for(int l = 0; l < font_wide; ++l)
			{
				int mkl = pixels[k * font_wide + l];
				if(mkl > show_gradation)
				{
					picture[(pos_y + k) * line_byte + 3 * (pos_x + l)] = mkl * color[2] / 25.0;
					picture[(pos_y + k) * line_byte + 3 * (pos_x + l) + 1] = mkl * color[1] / 25.0;
					picture[(pos_y + k) * line_byte + 3 * (pos_x + l) + 2] = mkl * color[0] / 25.0;
				}
			}
		}
		pos_x += font_wide;
	}
	height = pos_y + 24+paper_distance_vertical;
	GLuint texture_ID;
	glGenTextures(1, &texture_ID);
	if(texture_ID == 0)
	{
		cerr << "texture number not get!!!" << endl;
		return *p;
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, texture_ID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, wide, height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, picture);
	}
	p->texture = texture_ID;
	p->height = height;
	if(panel_num > panel_buffer)
		clean_panels();
	return *p;
}

void character_controller::change_ground()
{
	dark_ground = !dark_ground;
	while(panel_num > 0)
		clean_panels();
}

void character_controller::clean_panels()
{
	panel* p = panels;
	panel* r = p;
	while(p->next != NULL)
	{
		r = p;
		p = p->next;
	}
	glDeleteTextures(1, &p->texture);
	--panel_num;
	r->next = NULL;
	delete p;
}
