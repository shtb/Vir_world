#ifndef GRA_STRCTS_H_INCLUDED
#define GRA_STRCTS_H_INCLUDED

#include "../../pub/STDHF.h"
#include"../../pub/world_pub_structs.h"

struct grain
{
        int model_id;
        float_virw_position pos;
        float color[4]= {0,0,0,0.7};
};

struct gra_fog
{
        bool render = true;
        float color[3];
        float density = 0.1;
};

struct gra_accum
{
        bool init = true;
        bool on = false;
        float para = 0.1;
};

class GL_task_part
{
public:
        float* contrast;
        int part_order;
        void init(int con_num, int order)
        {
                contrast = new float[con_num + 1];
                part_order = order;
        }
};

class GL_task
{
public:
        bool play_on = 0;
        double state = 0;

        int task_part_num = 0;
        GL_task_part* task_part;
        bool go_back = 0;
        double limit = 1;
        double step = 0;
        int real = 0;
        bool direction = 0;
        bool always_on = 0;
        bool limit_rand = 0;
        bool auto_run = 1;
        int part_postion = 0;
        void init(int num, bool back, int re, bool always, bool rand, double ste)
        {
                auto_run = true;
                go_back = back;
                task_part_num = num;
                task_part = new GL_task_part[num];
                real = re;
                always_on = always;
                limit_rand = rand;
                step = ste;
        }
        void init(int num)
        {
                auto_run = false;
                real = 0;
                task_part_num = num;
                task_part = new GL_task_part[num];
        }
};

#endif // GRA_STRCTS_H_INCLUDED
