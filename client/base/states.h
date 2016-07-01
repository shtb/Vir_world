#ifndef STATES_H
#define STATES_H

#include "../../pub/STDHF.h"
#include "../../pub/spinlock.h"
#include"gra_structs.h"

using namespace std;

class states
{
public:
        string state_show;
        gra_client myself;
        unsigned int world_run_count=0;
        int time_big,time_small;

        void refresh_show();

        states();
        ~states();

};

#endif // STATES_H
