#include "console.h"
#include<sys/time.h>
#include <iostream>
#include <sstream>
#include <string>
#include "interfaces.h"

using namespace std;

console::console()
{
}

void console::cut_string(string& sen)
{
	for(unsigned int i = 0; i < sen.length()-2; ++i)
	{
		if(sen[i] == '\\' && sen[i + 1] == 'n')
		{
			sen = sen.substr(i + 2);
			return;
		}
		else if(sen[i]=='\n')
		{
			sen=sen.substr(i+1);
			return;
		}
	}
	sen = "";
}

void console::warning(string sentence)
{
	lock.slock();
	if(started)
	{
		while(system_p.length()>500)
		{
			cut_string(system_p);
		}

		if(sentence==system_last)
		{
			same_count++;
		}
		else
		{
			if(same_count!=1)
			{
				stringstream ss;
				ss<<"  ::";
				ss<<same_count<<"\n";
				if(system_p[system_p.length()-1]=='\n')
					system_p=system_p.substr(0,system_p.length()-1);
				system_p+=ss.str();
			}
			same_count=1;
			system_last=sentence;
			system_p += "\\czha>WARNING: " + sentence + "\n";
		}
		if(same_count==1)
		{
			system=system_p;
		}
		else
		{
			stringstream ss;
			ss<<"  ::";
			ss<<same_count<<"\n";
			if(system_p[system_p.length()-1]=='\n')
				system_p=system_p.substr(0,system_p.length()-1);
			system=system_p+ss.str();
		}
	}
	else
		cerr << "\033[35m >WARNING: " + sentence + "\033[0m" << endl;
	lock.unlock();
}

void console::error(string sentence)
{
	lock.slock();
	if(started)
	{
		while(system_p.length()>500)
		{
			cut_string(system_p);
		}

		if(sentence==system_last)
		{
			same_count++;
		}
		else
		{
			if(same_count!=1)
			{
				stringstream ss;
				ss<<"  ::";
				ss<<same_count<<"\n";
				if(system_p[system_p.length()-1]=='\n')
					system_p=system_p.substr(0,system_p.length()-1);
				system_p+=ss.str();
			}
			same_count=1;
			system_last=sentence;
			system_p += "\\czaa>WARNING: " + sentence + "\n";
		}

		if(same_count==1)
		{
			system=system_p;
		}
		else
		{
			stringstream ss;
			ss<<"  ::";
			ss<<same_count<<"\n";
			if(system_p[system_p.length()-1]=='\n')
				system_p=system_p.substr(0,system_p.length()-1);
			system=system_p+ss.str();
		}
	}
	else
		cerr << "\033[31m >ERROR: " + sentence + "\033[0m" << endl;
	lock.unlock();
}

void console::message(string sentence)
{
	lock.slock();
	if(started)
	{
		history += "\\cfzf >" + sentence + "\\n";
	}
	else
		cout << "\033[34m >" + sentence + "\033[0m" << endl;
	lock.unlock();
}

void console::add_talk(string sentence, string people)
{
	if(talking != "")
	{
		talked += talking + "\\n";
	}
	if(people == "shtb")
	{
		talking = "\\cnnz" + sentence;
	}
	else if(people == "mulin")
	{
		talking = "\\czpa" + sentence;
	}
	else
		talking = sentence;
}

key_board_node console::get_current_key()
{
	key_board_node the_key;
	while(my_com.key_board.empty())
	{
		usleep(1000);
	}
	the_key = my_com.key_board.front();
	my_com.key_board.pop();
	return the_key;
}

stringstream& console::con_input()
{
	ss_input.clear();
	ss_input.str("");
	if(started)
	{
		bool got = false;
		while(!got)
		{
			usleep(1000);
			while(!my_com.key_board.empty())
			{
				key_board_node the_key;
				the_key = my_com.key_board.front();
				my_com.key_board.pop();
				if(the_key.state == key_down)
				{
					if(the_key.key == 13)
					{
						history += typing_pre + typing + "\\n";
						ss_input.str(typing);
						typing = "";
						got = true;
					}
					else if(the_key.key >= 0x20 && the_key.key <= 0x7E)
					{
						typing += the_key.key;
					}
					else if(the_key.key == 8)
					{
						typing = typing.substr(0, typing.length() - 1);
					}
				}
			}
		}
	}
	else
	{
		string s;
		cin >> s;
		ss_input << s;
	}
	return ss_input;
}

stringstream& console::safe_con_input()
{
	string safe_typing;
	ss_input.clear();
	ss_input.str("");
	if(started)
	{
		bool got = false;
		while(!got)
		{
			usleep(1000);
			while(!my_com.key_board.empty())
			{
				key_board_node the_key;
				the_key = my_com.key_board.front();
				my_com.key_board.pop();
				if(the_key.state == key_down)
				{
					if(the_key.key == 13)
					{
						ss_input.str(safe_typing);
						got = true;
					}
					else if(the_key.key >= 0x20 && the_key.key <= 0x7E)
					{
						safe_typing += the_key.key;
					}
					else if(the_key.key == 8)
					{
						safe_typing = safe_typing.substr(0, safe_typing.length() - 1);
					}
				}
			}
		}
	}
	else
	{
		string s;
		cin >> s;
		ss_input << s;
	}
	return ss_input;
}

console::~console()
{
}
