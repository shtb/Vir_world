#pragma once
#include "../../pub/STDHF.h"
#include "../../pub/spinlock.h"
#include"../core/Com.h"

using std::string;

class console
{
public:
	  spinlock lock;
	  bool started = false;
	  string typing_pre, typing;
	  string history;

	  string system;
	  string system_last;
	  int same_count=1;
	  string system_p;

	  string big_screen;

	  string title;
	  string mutable_message;

	  string talked;
	  string talking;

	  std::stringstream ss_input;

	  console();
	  void add_talk(string s, string people);
	  void cut_string(string& sen);
	  void message(string);
	  key_board_node get_current_key();
	  std::stringstream& con_input();
	  std::stringstream& safe_con_input();
	  void warning(string s);
	  void error(string s);
	  ~console();
};
