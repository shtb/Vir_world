#pragma once

#include<thread>
#include"../../pub/spinlock.h"
#include"../../pub/STDHF.h"
#include"../base/world_structs.h"

using namespace std;


struct grain
{
        int model=0;
        float color[4];
        float_virw_position position;
        float_virw_velocity velocity;
        double mass;
        double wind_resistance;
        grain*next=NULL;
        grain*pre=NULL;
        int life;
        bool attach=false;
};

class virw_grain
{
public:
        spinlock lock;
        grain grains;
        int live_num=0;
        int max_num=10000;

        void add(int num,virw_position pos,int model,float color[]);
};
