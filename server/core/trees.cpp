#include "trees.h"
#include"../base/interfaces.h"

trees::trees()
{
        kind_id=enum_id_tree;
        max_live_num=300;
}


void trees::move()
{
        life*p=the_head.next;
        while(p!=NULL)
        {
                p->live_time++;
                if(p->live_time%p->fission_wait==(unsigned)p->fission_wait-1)
                {
                        p->life-=2.0;
                        float_virw_position pos;
                        pos.x=(rand()%18000)/100.0+10;
                        pos.z=(rand()%18000)/100.0+10;
                        pos.y=my_world.collision.map_height(pos.x,pos.z);
                        add(pos,0.9);
                        pos.x=(rand()%18000)/100.0+10;
                        pos.z=(rand()%18000)/100.0+10;
                        pos.y=my_world.collision.map_height(pos.x,pos.z);
                        add(pos,0.9);
                }
                p->dead=true;
                p->life+=0.0006;
                p=p->next;
        }
}

trees::~trees()
{
}
