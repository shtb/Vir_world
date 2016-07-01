#ifndef WORLD_SWITCH_H
#define WORLD_SWITCH_H

#include"../../pub/STDHF.h"
#include<string.h>

using namespace std;

enum enum_directions {enum_direction_east,enum_direction_west,enum_direction_south,enum_direction_north};

class world_switch
{
public:
        int this_world_order=12;
        const int max_world_num=81;
        int using_world_num=23;
        int world_map[11][11];
        void init();
        bool switch_world(int dir);
        void save();
        void get_xy_pos(int&x,int&y);
        world_switch();
        ~world_switch();
private:
        void load_this_world(int world_order);
        void save_this_world(int world_order);
        void refresh_texture_file_name();
};

#endif // WORLD_SWITCH_H
