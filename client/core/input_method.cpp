#include "input_method.h"
#include "../base/interfaces.h"
#include <algorithm>
using namespace std;

string allsm="bpmfdtnlgkhjqxrzcsyw";
string get_smpy(string pinyin)
{
	string smpy;
	int len=pinyin.length();
	bool longym=false;
	for(int i=0; i<len; ++i)
	{
		if(allsm.find(pinyin[i])!=string::npos)
		{
			longym=false;
			//get sm
			smpy+=pinyin[i];
			char sm=pinyin[i];
			if(i<len-1)
			{
				if(pinyin[i+1]=='h')
					i++;
			}
			//get ym
			if(i<len-1)
			{
				i++;
				char thc;
				thc=pinyin[i];
				if(thc=='a')
				{
					if(i<len-1)
					{
						i++;
						thc=pinyin[i];
						if(thc=='i'||thc=='o')
						{}
						else if(thc=='n')
						{
							longym=true;
							if(i<len-1)
							{
								i++;
								thc=pinyin[i];
								if(thc!='g')
									i--;
							}
						}
						else
						{
							i--;
						}
					}
				}
				else if(thc=='o')
				{
					if(i<len-1)
					{
						i++;
						thc=pinyin[i];
						if(thc=='u')
						{}
						else if(thc=='n')
						{
							longym=true;
							if(i<len-1)
							{
								i++;
								thc=pinyin[i];
								if(thc!='g')
									i-=2;
							}
						}
						else
						{
							i--;
						}
					}
				}
				else if(thc=='e')
				{
					if(i<len-1)
					{
						i++;
						thc=pinyin[i];
						if(thc=='i')
						{}
						else if(thc=='r')
							longym=true;
						else if(thc=='n')
						{
							longym=true;
							if(i<len-1)
							{
								i++;
								thc=pinyin[i];
								if(thc!='g')
									i--;
							}
						}
						else
						{
							i--;
						}
					}
				}
				else if(thc=='i')
				{
					if(i<len-1)
					{
						i++;
						thc=pinyin[i];
						if(thc=='u'||thc=='e')
						{}
						else if(thc=='n')
						{
							longym=true;
							if(i<len-1)
							{
								i++;
								thc=pinyin[i];
								if(thc!='g')
									i--;
							}
						}
						else if(thc=='o')
						{
							if(i<len-2)
							{
								if(pinyin[i+1]=='n'&&pinyin[i+2]=='g')
									i+=2;
								else
									i--;
							}
						}
						else if(thc=='a')
						{
							if(i<len-1)
							{
								i++;
								thc=pinyin[i];
								if(thc=='n')
								{
									longym=true;
									if(i<len-1)
									{
										i++;
										thc=pinyin[i];
										if(thc!='g')
											i--;
									}
								}
								else if(thc!='o')
								{
									i--;
								}
							}
						}
						else
						{
							i--;
						}
					}
				}
				else if(thc=='u')
				{
					if(i<len-1)
					{
						i++;
						thc=pinyin[i];
						if(thc=='i'||thc=='o'||thc=='e')
						{}
						else if(thc=='n')
							longym=true;
						else if(thc=='a')
						{
							if(i<len-1)
							{
								i++;
								thc=pinyin[i];
								if(thc=='n')
								{
									longym=true;
									if(i<len-1)
									{
										i++;
										thc=pinyin[i];
										if(thc!='g')
											i--;
									}

								}
								else if(thc!='i')
								{
									i--;
								}
							}
						}
						else
						{
							i--;
						}
					}
				}
				else if(thc=='v')
				{
					if(i<len-1)
					{
						i++;
						thc=pinyin[i];
						if(thc=='e')
						{}
						else if(thc=='n')
							longym=true;
						else
						{
							i--;
						}
					}
				}
				else
				{
					if(sm=='r')
					{
						smpy=smpy.substr(0,smpy.length()-1);
					}
				}
			}
			else
			{
				smpy=smpy.substr(0,smpy.length()-1);
			}
		}
		else
		{
			if(longym)
			{
				longym=false;
				i-=2;
			}
			else
				smpy+=pinyin[i];
		}

	}
	if(smpy.length()<2)
	{
		smpy="";
	}
	return smpy;
}

input_method::input_method()
{
	// ctor
}

bool Comp(const input_node& a, const input_node& b)
{
	return *(a.used) > *(b.used);
}

void input_method::init()
{
	{
		int max_used=0;
		int zero_used=0;

		ifstream inm(my_file.pub_path+"com/input_method_sc_character.im", ios::in | ios::binary);
		if(!inm)
			my_print.error("input_method_sc init failed!");
		chara temch;
		while(inm.read((char*)&temch.used, sizeof(int)))
		{

			if(temch.used==0)
				zero_used++;
			else
				if(temch.used>max_used)
					max_used=temch.used;
			inm.read((char*)&temch.tone, sizeof(char));
			string pinyin;
			size_t len;
			inm.read((char*)&len, sizeof(size_t));
			char* s = new char[len + 1];
			inm.read(s, len);
			s[len] = '\0';
			temch.hanzi = s;
			delete[] s;
			inm.read((char*)&len, sizeof(size_t));
			s = new char[len + 1];
			inm.read(s, len);
			s[len] = '\0';
			pinyin = s;
			delete[] s;
			ch_map.insert(pair<string, chara>(pinyin, temch));
		}
		inm.close();
		cout<<"max_used:"<<max_used<<"\tnever:"<<zero_used<<endl;
	}

	{
		int max_used=0;
		int zero_used=0;

		ifstream inm( my_file.pub_path+"com/input_method_sc_word.im", ios::in | ios::binary);
		if(!inm)
			my_print.error("input_method_sc init failed!");
		word temch;
		while(inm.read((char*)&temch.used, sizeof(int)))
		{
			if(temch.used==0)
				zero_used++;
			else
				if(temch.used>max_used)
					max_used=temch.used;
			string pinyin;
			size_t len;
			inm.read((char*)&len, sizeof(size_t));
			char* s = new char[len + 1];
			inm.read(s, len);
			s[len] = '\0';
			temch.word = s;
			delete[] s;
			inm.read((char*)&len, sizeof(size_t));
			s = new char[len + 1];
			inm.read(s, len);
			s[len] = '\0';
			pinyin = s;
			delete[] s;
			string smpy=get_smpy(pinyin);
			wo_map.insert(pair<string, word>(pinyin, temch));
			if(smpy!="")
				smwo_map.insert(pair<string, word>(smpy, temch));
		}
		inm.close();
		cout<<"max_used:"<<max_used<<"\tnever:"<<zero_used<<endl;
	}
	//addition
	{
		ifstream inm( my_file.pub_path+"com/add_word.txt", ios::in | ios::binary);
		if(inm)
		{
			word temch;
			string pinyin;
			while(inm>>pinyin)
			{
				inm>>temch.word;
				wo_map.insert(pair<string, word>(pinyin, temch));
				string smpy=get_smpy(pinyin);
				if(smpy!="")
					smwo_map.insert(pair<string, word>(smpy, temch));
			}
		}
		inm.close();
	}
	stringstream sss;
	sss<<"character:"<<ch_map.size()<<"\twords:"<<wo_map.size();
	my_print.message("input_method_sc init finished!");
	my_print.message(sss.str());
}

void input_method::save()
{
	{
		ofstream inm(my_file.pub_path+"com/input_method_sc_character.im", ios::out);
		multimap<string, chara>::iterator p_map;
		pair<multimap<string, chara>::iterator, multimap<string, chara>::iterator> ret;
		for(p_map = ch_map.begin(); p_map != ch_map.end();)
		{
			ret = ch_map.equal_range(p_map->first);
			for(p_map = ret.first; p_map != ret.second; ++p_map)
			{
				inm.write((char*)&(p_map->second.used), sizeof(int));
				inm.write((char*)&(p_map->second.tone), sizeof(char));

				size_t st = p_map->second.hanzi.length();
				inm.write((char*)&st, sizeof(size_t));
				inm.write(p_map->second.hanzi.c_str(), st);

				st = p_map->first.length();
				inm.write((char*)&st, sizeof(size_t));
				inm.write(p_map->first.c_str(), st);
			}
		}
		inm.close();
	}

	{
		ofstream inm(my_file.pub_path+"com/input_method_sc_word.im", ios::out);
		multimap<string, word>::iterator p_map;
		pair<multimap<string, word>::iterator, multimap<string, word>::iterator> ret;
		for(p_map = wo_map.begin(); p_map != wo_map.end();)
		{
			ret = wo_map.equal_range(p_map->first);
			for(p_map = ret.first; p_map != ret.second; ++p_map)
			{
				inm.write((char*)&(p_map->second.used), sizeof(int));

				size_t st = p_map->second.word.length();
				inm.write((char*)&st, sizeof(size_t));
				inm.write(p_map->second.word.c_str(), st);

				st = p_map->first.length();
				inm.write((char*)&st, sizeof(size_t));
				inm.write(p_map->first.c_str(), st);
			}
		}
		inm.close();
	}
	my_print.message("input_method_saved!");
}

void input_method::create_word()
{
	if(creating_word)
	{
		creating_word = false;
		word tem;
		tem.word = cr_word;
		tem.used = 10;
		if(cr_word.length()>2&&cr_pinyin.length()>2)
		{
			wo_map.insert(pair<string, word>(cr_pinyin, tem));
			string smpy=get_smpy(cr_pinyin);
			if(smpy!="")
				smwo_map.insert(pair<string, word>(smpy, tem));
		}
		cr_pinyin = "";
		cr_word = "";
	}
	else
	{
		creating_word = true;
		cr_pinyin = "";
		cr_word = "";
	}
}

string input_method::get_answer(unsigned int choice)
{
	if(choice + look_line * 9 < choices.size())
	{
		++changed_time;
		++(*choices[choice + look_line * 9].used);
		if(changed_time >= save_limit)
		{
			save();
			changed_time = 0;
		}
		string the_s = choices[choice + look_line * 9].word;
		if(creating_word)
		{
			cr_word += the_s;
			cr_pinyin += typing.substr(0, part_i);
		}
		choices.clear();
		if(part_choose)
		{
			if(part_i<(int)typing.length())
				typing = typing.substr(part_i, typing.length() - part_i);
		}
		else
		{
			typing = "";
		}
		look_line = 0;
		parse();
		return the_s;
	}
	else
	{
		return " ";
	}
}

void input_method::parse()
{
	stringstream ss;

	multimap<string, chara>::iterator c_map;
	pair<multimap<string, chara>::iterator, multimap<string, chara>::iterator> cret;

	multimap<string, word>::iterator w_map;
	pair<multimap<string, word>::iterator, multimap<string, word>::iterator> wret;

	part_i = typing.length();

	choices.clear();
	int num = ch_map.count(typing);
	num += wo_map.count(typing);
	num += smwo_map.count(typing);
	if(num == 0)
	{
		show = typing + "\\n";
		if(typing == "")
		{
			show = ">input_method_sc<";
		}
		else
		{
			part_choose = true;
			part_i = typing.length();
			while(part_i > 0)
			{
				string tem_typing = typing.substr(0, part_i);
				if(ch_map.count(tem_typing) + wo_map.count(tem_typing) > 0)
				{
					cret = ch_map.equal_range(tem_typing);
					for(c_map = cret.first; c_map != cret.second; ++c_map)
					{
						input_node tem_node;
						tem_node.word = c_map->second.hanzi;
						tem_node.used = &(c_map->second.used);
						bool had = false;
						for(unsigned int i = 0; i < choices.size(); ++i)
						{
							if(choices[i].word == tem_node.word)
							{
								had = true;
								break;
							}
						}
						if(!had)
							choices.push_back(tem_node);
					}

					wret = wo_map.equal_range(tem_typing);
					for(w_map = wret.first; w_map != wret.second; ++w_map)
					{
						input_node tem_node;
						tem_node.word = w_map->second.word;
						tem_node.used = &(w_map->second.used);
						bool had = false;
						for(unsigned int i = 0; i < choices.size(); ++i)
						{
							if(choices[i].word == tem_node.word)
							{
								had = true;
								break;
							}
						}
						if(!had)
							choices.push_back(tem_node);
					}

					sort(choices.begin(), choices.end(), Comp);
					unsigned int ord = 1;
					while(ord <= 9 && ord + look_line * 9 <= choices.size())
					{
						ss.clear();
						ss.str("");
						ss << ord;
						show += " " + ss.str() + "." + choices[ord - 1 + look_line * 9].word;
						++ord;
					}
					break;
				}
				part_i--;
			}
		}
	}
	else
	{
		part_choose = false;
		show = typing + "\\n";

		cret = ch_map.equal_range(typing);
		for(c_map = cret.first; c_map != cret.second; ++c_map)
		{
			input_node tem_node;
			tem_node.word = c_map->second.hanzi;
			tem_node.used = &(c_map->second.used);
			bool had = false;
			for(unsigned int i = 0; i < choices.size(); ++i)
			{
				if(choices[i].word == tem_node.word)
				{
					had = true;
					break;
				}
			}
			if(!had)
				choices.push_back(tem_node);
		}

		wret = wo_map.equal_range(typing);
		for(w_map = wret.first; w_map != wret.second; ++w_map)
		{
			input_node tem_node;
			tem_node.word = w_map->second.word;
			tem_node.used = &(w_map->second.used);
			bool had = false;
			for(unsigned int i = 0; i < choices.size(); ++i)
			{
				if(choices[i].word == tem_node.word)
				{
					had = true;
					break;
				}
			}
			if(!had)
				choices.push_back(tem_node);
		}

		wret = smwo_map.equal_range(typing);
		for(w_map = wret.first; w_map != wret.second; ++w_map)
		{
			input_node tem_node;
			tem_node.word = w_map->second.word;
			tem_node.used = &(w_map->second.used);
			bool had = false;
			for(unsigned int i = 0; i < choices.size(); ++i)
			{
				if(choices[i].word == tem_node.word)
				{
					had = true;
					break;
				}
			}
			if(!had)
				choices.push_back(tem_node);
		}

		sort(choices.begin(), choices.end(), Comp);
		unsigned int ord = 1;
		while(ord <= 9 && ord + look_line * 9 <= choices.size())
		{
			ss.clear();
			ss.str("");
			ss << ord;
			show += " " + ss.str() + "." + choices[ord - 1 + look_line * 9].word;
			++ord;
		}
	}
	if(creating_word)
		show += "\\n\\ciiz<<" + cr_pinyin + "><" + cr_word + ">>";
}

input_method::~input_method()
{
	// dtor
}
