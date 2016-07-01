#ifndef LIVES_H
#define LIVES_H

#include<thread>
#include"../../pub/spinlock.h"
#include"../../pub/STDHF.h"
#include"world_structs.h"

using namespace std;

struct life
{
        float space_ver[153];
        float_virw_position last_position;
        float_virw_position position;
        float_virw_position want_pos;
        float_virw_velocity velocity;
        float_virw_velocity mem_velocity;
        life*next=NULL;
        life*pre=NULL;
        life* hunting=0;
        int fission_wait;
        bool hungry=false;
        unsigned int live_time=0;
        float ener=0;
        float life;
        short tem_time=0;
        bool ground=false;
        bool dead=false;
        int attacked=1;
        bool attach=false;
        bool sleep=false;
};

class lives
{
public:
        life* live_map[20][20];
        life the_head;
        int live_num=0;
        int max_live_num=100;
        int kind_id=-1;
        life* die(life*de);
        void clean_and_map();
        void add(float_virw_position pos,float lif);
        lives();
        ~lives();

};

#endif // LIVES_H
