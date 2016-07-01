#include "cubes.h"
#include"../base/interfaces.h"

cubes::cubes()
{

}

cube* cubes::destroy(cube*p)
{
      p->pre->next=p->next;
      if(p->next!=NULL)
      {
            p->next->pre=p->pre;
      }
      cube*de=p;
      p=p->next;
      delete de;
      --live_num;
      my_world.cubes_changed=2;
      return p;
}


void cubes::add(virw_position pos,float color[4])
{
      if(live_num<max_live_num)
      {
            live_num++;
            cube*p;
            p=new cube;
            p->position=(((long)(pos.x*10))<<40)+(((long)(pos.y*10))<<20)+(long)(pos.z*10);
            p->color[0]=color[0];
            p->color[1]=color[1];
            p->color[2]=color[2];
            p->color[3]=color[3];
            p->next=the_head.next;
            p->pre=&the_head;
            if(the_head.next!=NULL)
                  the_head.next->pre=p;
            the_head.next=p;
            my_world.cubes_changed=2;
      }
}

void cubes::add(long pos,float color[4])
{
      if(live_num<max_live_num)
      {
            live_num++;
            cube*p;
            p=new cube;
            p->position=pos;
            p->color[0]=color[0];
            p->color[1]=color[1];
            p->color[2]=color[2];
            p->color[3]=color[3];
            p->next=the_head.next;
            p->pre=&the_head;
            if(the_head.next!=NULL)
                  the_head.next->pre=p;
            the_head.next=p;
            my_world.cubes_changed=2;
      }
}
