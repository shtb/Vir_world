#ifndef INPUT_M
#define INPUT_M

#include "../../pub/STDHF.h"
#include <vector>
#include <map>

using std::string;
using std::map;
using std::multimap;
using std::vector;

struct chara
{
	  int used = 0;
	  string hanzi;
	  char tone = 0;
};

struct word
{
	  int used = 0;
	  string word;
};

struct input_node
{
	  string word;
	  int* used;
};

class input_method
{
public:
	  int look_line = 0;

	  int part_i = 1;
	  bool part_choose = false;
	  bool creating_word = false;
	  string cr_word;
	  string cr_pinyin;

	  int changed_time = 0;
	  int save_limit = 40;
	  string typing;
	  multimap<string, chara> ch_map;
	  multimap<string, word> wo_map;
	  multimap<string, word> smwo_map;
	  string show;
	  vector<input_node> choices;
	  input_method();
	  void init();
	  void create_word();
	  void save();
	  string get_answer(unsigned int choice);
	  void parse();
	  virtual ~input_method();

protected:
private:
};

#endif // INPUT_M
