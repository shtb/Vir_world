#ifndef CHARACTER_C
#define CHARACTER_C

#include "../../pub/STDHF.h"
#include <map>
#include <GL/glut.h>

using std::string;
using std::map;

struct character
{
	int code;
	int wide;
	GLubyte* pixels;
};

struct panel
{
	string sentence;
	GLuint texture;
	int wide, height;
	panel* next = NULL;
};

class character_controller
{
public:
	GLubyte* picture;
	bool dark_ground = true;
	map<int, character> character_map;
	int panel_num = 0;
	unsigned char show_gradation = 250;
	const int line_distance=30;
	const int paper_distance_vertical=4;
	const int paper_distance_horizon=6;
	const int panel_buffer = 16;
	panel* panels = NULL;
	void init(string file);
	void cutstring(string& sen, int wide);
	panel get_panel(string sen, int wide);
	void clean_panels();
	void change_ground();
	character_controller();
	~character_controller();

protected:
private:
};

#endif // CHARACTER_C
