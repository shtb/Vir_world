#include "lives.h"

lives::lives()
{
}

life* lives::die(life*p)
{
        if(p->attach)
        {
                p->life=0.2;
                return p->next;
        }
        p->pre->next=p->next;
        if(p->next!=NULL)
        {
                p->next->pre=p->pre;
        }
        life*de=p;
        p=p->next;
        delete de;
        --live_num;
        return p;
}

void lives::clean_and_map()
{
        for(int i=0; i<20; ++i)
        {
                for(int j=0; j<20; ++j)
                {
                        live_map[i][j]=NULL;
                }
        }
        life*p=the_head.next;
        while(p!=NULL)
        {
                if(p->life<0)
                {
                        p=die(p);
                        continue;
                }
                else
                {
                        unsigned int a=p->position.x/10;
                        unsigned int b=p->position.z/10;
                        if(a>=20||b>=20)
                        {
                                p=die(p);
                                continue;
                        }
                        live_map[a][b]=p;
                }
                p=p->next;
        }
}

void lives::add(float_virw_position pos,float lif)
{
        if(live_num<max_live_num)
        {
                live_num++;
                life*p;
                p=new life;
                p->position=pos;
                p->life=lif;
                p->ener=lif;
                if(kind_id!=enum_id_tree)
                {
                        p->fission_wait=2000+(rand()%1000);
                }
                else
                {
                        p->fission_wait=3000+(rand()%2000);
                }
                p->next=the_head.next;
                p->pre=&the_head;
                if(the_head.next!=NULL)
                        the_head.next->pre=p;
                the_head.next=p;
        }
}

lives::~lives()
{
}
