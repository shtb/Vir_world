#include "grass_eats.h"
#include"../base/interfaces.h"

grass_eats::grass_eats()
{
      kind_id=enum_id_grass_eat;
}


void grass_eats::move()
{
      life*p=the_head.next;
      while(p!=NULL)
      {
            p->last_position=p->position;
            p->live_time++;
            p->hunting=NULL;
            if(p->live_time%p->fission_wait==(unsigned)p->fission_wait-1)
            {
                  p->life-=2.0;
                  p->ener-=2.0;
                  add(p->position,0.7);
                  add(p->position,0.7);
                  add(p->position,0.6);
            }
            if(p->ener<0)
            {
                  p->dead=true;
            }
            if(!p->dead)
            {
                  p->sleep=false;
                  p->life+=0.001;
                  p->ener-=0.001;
                  //think
                  if(p->ener>10*p->life)
                  {
                        p->hungry=false;
                  }

                  if(p->attacked)
                  {
                        p->want_pos.x=(rand()%20000)/100.0;
                        if(p->want_pos.x<150&&p->want_pos.x>100)
                              p->want_pos.x=195;
                        else if(p->want_pos.x>50&&p->want_pos.x<=100)
                              p->want_pos.x=5;
                        p->want_pos.z=(rand()%20000)/100.0;
                        if(p->want_pos.z<150&&p->want_pos.z>100)
                              p->want_pos.z=195;
                        else if(p->want_pos.z>50&&p->want_pos.z<=100)
                              p->want_pos.z=5;
                        p->attacked--;
                  }
                  else if(p->ener<(3*p->life)||p->hungry)
                  {
                        p->hungry=true;
                        unsigned int a=p->position.x/10;
                        unsigned int b=p->position.z/10;
                        life*disl=my_world.w_trees.live_map[a][b];
                        if(disl==0)
                        {
                              if(p->tem_time<200)
                              {
                                    ++(p->tem_time);
                              }
                              else
                              {
                                    p->want_pos.x=(rand()%20000)/100.0;
                                    if(p->want_pos.x<150&&p->want_pos.x>100)
                                          p->want_pos.x=195;
                                    else if(p->want_pos.x>50&&p->want_pos.x<=100)
                                          p->want_pos.x=5;
                                    p->want_pos.z=(rand()%20000)/100.0;
                                    if(p->want_pos.z<150&&p->want_pos.z>100)
                                          p->want_pos.z=195;
                                    else if(p->want_pos.z>50&&p->want_pos.z<=100)
                                          p->want_pos.z=5;
                                    p->tem_time=0;
                              }
                        }
                        else
                        {
                              p->tem_time=201;
                              p->hunting=disl;
                              p->want_pos=disl->position;
                        }
                  }
                  else
                  {
                        p->sleep=true;
                  }
                  /////////////////////////////////////////////move
                  if(!p->sleep)
                  {
                        float_virw_velocity dv;
                        dv.x=(p->want_pos.x-p->position.x);
                        dv.z=(p->want_pos.z-p->position.z);
                        if(abs(dv.x)<0.05||abs(dv.z)<0.05)
                        {
                              p->velocity=p->mem_velocity;
                              p->attacked=1;
                        }
                        else
                        {
                              if(dv.z<0)
                              {
                                    p->want_pos.b=180+atan(dv.x/dv.z)*57.3;
                              }
                              else
                              {
                                    p->want_pos.b=atan(dv.x/dv.z)*57.3;
                                    if(p->want_pos.b<0)
                                          p->want_pos.b=360+p->want_pos.b;
                              }
                              float ang_b=p->want_pos.b-p->position.b;
                              if(ang_b>180)
                              {
                                    ang_b=ang_b-360;
                              }
                              float d_angle=abs(ang_b);
                              float speed=(180-d_angle)/4550.0;
                              if(dv.x>1)
                                    dv.x=1;
                              if(dv.z>1)
                                    dv.z=1;
                              if(dv.x<-1)
                                    dv.x=-1;
                              if(dv.z<-1)
                                    dv.z=-1;
                              p->velocity.x+=dv.x*speed;
                              p->velocity.z+=dv.z*speed;
                              p->velocity.b+=0.04*ang_b;
                              p->mem_velocity=p->velocity;
                        }
                  }
            }
            else
            {
                  p->life-=0.005;
            }
            p->position.x+=p->velocity.x;
            p->position.y+=p->velocity.y;
            p->position.z+=p->velocity.z;
            p->position.a+=p->velocity.a;
            p->position.b+=p->velocity.b;
            p=p->next;
      }
}

grass_eats::~grass_eats()
{
}
