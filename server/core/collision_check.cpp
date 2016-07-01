#include"../../pub/STDHF.h"
#include"../base/interfaces.h"
#include "collision_check.h"

using namespace std;


collision_check::collision_check()
{
      //ctor
}

long collision_check::add(life*p,virtual_ID id)
{
      unsigned long model=(unsigned long)id<<48;
      long this_space;
      coll_model*mod;
      switch(id)
      {
      case(enum_id_fly_eat):
      {
            mod=&fly_eat_model;
            break;
      }
      case(enum_id_grass_eat):
      {
            mod=&grass_eat_model;
            break;
      }
      case(enum_id_meat_eat):
      {
            mod=&meat_eat_model;
            break;
      }
      case(enum_id_tree):
      {
            mod=&tree_model;
            break;
      }
      default:
            mod=NULL;
            break;
      }
      if((my_world.run_count+3)%6==0)
      {
            mod->reset();
            double big=pow(p->life,0.33333);
            mod->scale(big,big,big);
            if(p->position.b>360)
                  p->position.b-=360;
            if(p->position.c>360)
                  p->position.c-=360;
            if(p->position.b<0)
                  p->position.b+=360;
            if(p->position.c<0)
                  p->position.c+=360;

            mod->rotate(p->position.a,p->position.b,p->position.c);
            for(int i = 3; i < 153; i += 3)
            {
                  p->space_ver[i]=mod->ver[i];
                  p->space_ver[i+1]=mod->ver[i+1];
                  p->space_ver[i+2]=mod->ver[i+2];
            }
      }
      else
      {
            for(int i = 3; i < 153; i += 3)
            {
                  mod->ver[i]=p->space_ver[i];
                  mod->ver[i+1]=p->space_ver[i+1];
                  mod->ver[i+2]=p->space_ver[i+2];
            }
      }
      mod->translate(p->position.x,p->position.y,p->position.z);

      ///////////////////////////////////////////////////
      if(p->position.x >= 199|| p->position.x <= 1 || p->position.z >= 199|| p->position.z <= 1 || p->position.y >= 99)
      {
            p->attacked=true;
            p->velocity.x=0;
            p->velocity.y=0;
            p->velocity.z=0;
      }
      float het=map_height(p->position.x, p->position.z);
      if(p->position.y <= het)
      {
            if(p->velocity.y<0)
                  p->velocity.y=0.05+-0.8*p->velocity.y;
            if(p->velocity.y<0.05)
                  p->velocity.y=0.05;
            p->ground=true;
            if(id==enum_id_fly_eat)
            {
                  p->attacked=true;
            }
      }
      else
      {
            p->ground=false;
      }
      /////////////////////////////////////////////////
      bool eat=false;
      map<long,long>::iterator ite;
      for(int i = 3; i < 153; i += 3)
      {
            this_space=(((long)(mod->ver[i]*0.5))<<40)+(((long)(mod->ver[i+1]))<<20)+(long)(mod->ver[i+2]*0.5);
            if(coll2m.count(this_space))
            {
                  p->velocity.y+=0.1*(30-p->position.y);
            }
            else
            {
                  this_space=(((long)(mod->ver[i]*2))<<40)+(((long)(mod->ver[i+1]*4))<<20)+(long)(mod->ver[i+2]*2);
                  if(coll50cm.count(this_space))
                  {
                        p->velocity.y+=0.1*(30-p->position.y);
                  }
                  else
                  {
                        this_space=(((long)(mod->ver[i]*10))<<40)+(((long)(mod->ver[i+1]*10))<<20)+(long)(mod->ver[i+2]*10);
                        if(coll10cm.count(this_space))
                        {
                              ite=coll10cm.find(this_space);
                              unsigned long col=ite->second;
                              int kind=col>>48;
                              switch(kind)
                              {
                              case(enum_id_tree):
                              case(enum_id_fly_eat):
                              case(enum_id_meat_eat):
                              case(enum_id_grass_eat):
                              {
                                    unsigned long point=col&0xffffffffffff;
                                    if((life*)point==p->hunting)
                                    {
                                          eat=true;
                                    }
                              }
                                    break;
                              default:
                              {
                                    p->velocity.x*=-1.1;
                                    if(p->velocity.y<0)
                                          p->velocity.y*=-1.8;
                                    else
                                          p->velocity.y*=-0.9;
                                    p->velocity.z*=-1.1;
                              }
                                    break;
                              }
                        }
                        else
                        {
                              coll10cm.insert(make_pair(this_space,(model+(unsigned long)p)));
                        }
                  }
            }
      }
      float_virw_velocity dv;
      dv.x=(p->want_pos.x-p->position.x);
      dv.y=(p->want_pos.y-p->position.y);
      dv.z=(p->want_pos.z-p->position.z);
      float distance=dv.x*dv.x+dv.y*dv.y+dv.z*dv.z;
      if(distance<1&&p->hunting!=0)
      {
            eat=true;
      }
      if(eat)
      {
            if(p->hunting->dead)
            {
                  p->ener+=0.02;
                  p->hunting->life-=0.03;
            }
            else
            {
                  p->hunting->ener-=0.05;
                  p->hunting->attacked=100;
            }
      }
      return 0;
}

collision_reflect collision_check::map_reflect(double x,double y)
{
      unsigned int a=y/5;
      unsigned int b=x/5;
      if(a>=40||b>=40)
      {
            return reflect[0];
      }
      else
      {
            if(x+y<5*(a+b)+5)
            {
                  return reflect[a*40+b];
            }
            else
            {
                  return reflect[a*40+b+1600];
            }
      }
}

void collision_check::mixture(int task_or, int task_part_or)
{
      double state;
      int time = 0;

      state = my_world.MuLin_task[task_or].state; // k-快,m-慢

      if(my_world.MuLin_task[task_or].real == 1) // m-k-m-k-m
      {
            state = (sin(3.1415926535897 * (state - 0.5)) + 1) / 2;
      }
      else if(my_world.MuLin_task[task_or].real == 2)   // k-m-k
      {
            state = sin(3.1415926535897 / 2 * state);
      }
      else if(my_world.MuLin_task[task_or].real == 3)   // m-k-m
      {
            state = sin(3.1415926535897 / 2 * (state - 1.0)) + 1;
      }
      else if(my_world.MuLin_task[task_or].real == 4)   // m-k|-m-k-m
      {
            if(my_world.MuLin_task[task_or].direction)
                  state = sin(3.1415926535897 / 2 * state);
            else
                  state = (sin(3.1415926535897 * (state - 0.5)) + 1) / 2;
      }
      state *= my_world.MuLin_task[task_or].limit;

      coll_model_group* group_d = &(MuLin_model_display.group[my_world.MuLin_task[task_or].task_part[task_part_or].part_order]);
      int min, max;
      min = group_d->min;
      max = group_d->max;
      for(int i = min; i < max; ++i)
      {
            MuLin_model_display.ver[i] += state * my_world.MuLin_task[task_or].task_part[task_part_or].contrast[time];
            ++time;
      }
}

void collision_check::add_mulin()
{
      static int buffer_time=0;
      static size_t all_ver_size = 12 * MuLin_model_display.ver_num + 12;
      static set<long>buffer;
      long this_spaceaa;
      if(buffer_time%6==0)
      {
            buffer.clear();
            memcpy(MuLin_model_display.ver, MuLin_model_const.ver, all_ver_size);
            for(int i = 0; i < my_world.MuLin_task_num; ++i)
            {
                  if(my_world.MuLin_task[i].play_on)
                  {
                        for(int j = 0; j < my_world.MuLin_task[i].task_part_num; ++j)
                              if(my_world.MuLin_show[my_world.MuLin_task[i].task_part[j].part_order])
                              {
                                    mixture(i, j);
                              }
                  }
            }
            MuLin_model_display.rotate_b(my_world.mulin_pos.b+my_world.mulin_distortion.excursion.b);
            for(int i = 3; i < (3 * MuLin_model_display.ver_num + 3); i += 3)
            {
                  this_spaceaa=(((long)(MuLin_model_display.ver[i]*10))<<40)+(((long)(MuLin_model_display.ver[i+1]*10))<<20)+(long)(MuLin_model_display.ver[i+2]*10);
                  buffer.insert(this_spaceaa);
            }
      }

      if(my_world.mulin_pos.x >= 199)
      {
            my_world.mulin_pos.x = 199;
            my_world.mulin_vel.x=-0.1;
      }
      if(my_world.mulin_pos.x <= -199)
      {
            my_world.mulin_pos.x = -199;
            my_world.mulin_vel.x=0.1;
      }
      if(my_world.mulin_pos.z >= 199)
      {
            my_world.mulin_pos.z =199;
            my_world.mulin_vel.z=-0.1;
      }
      if(my_world.mulin_pos.z <= -199)
      {
            my_world.mulin_pos.z = -199;
            my_world.mulin_vel.z=0.1;
      }
      if(my_world.mulin_pos.y >= 99)
      {
            my_world.mulin_pos.y = 99;
            my_world.mulin_vel.y=-0.1;
      }
      double het=map_height(my_world.mulin_pos.x, my_world.mulin_pos.z)+0.06;
      bool refed=false;
      if(my_world.mulin_pos.y <= het)
      {
            collision_reflect refl=map_reflect(my_world.mulin_pos.x,my_world.mulin_pos.z);
            double vg=my_world.mulin_vel.x*refl.x+my_world.mulin_vel.y*refl.y+my_world.mulin_vel.z*refl.z;
            if(vg<0)
            {
                  my_world.mulin_vel.x-=1.8*vg*refl.x;
                  my_world.mulin_vel.y-=1.8*vg*refl.y;
                  my_world.mulin_vel.z-=1.8*vg*refl.z;
            }
            refed=true;
            my_world.mulin_vel.y+=(het-my_world.mulin_pos.y);
            my_world.mulin_ground=true;
      }

      long trans=(((long)((my_world.mulin_pos.x+my_world.mulin_distortion.excursion.x)*10))<<40)+(((long)((my_world.mulin_pos.y+my_world.mulin_distortion.excursion.y)*10))<<20)+(long)((my_world.mulin_pos.z+my_world.mulin_distortion.excursion.z)*10);
      set<long>::iterator muite,iend;
      iend=buffer.end();
      map<long,long>::iterator iter;
      for(muite=buffer.begin(); muite!=iend; ++muite)
      {
            long this_spacett=*muite+trans;
            double px=((this_spacett&0xfffff0000000000ul)>>40)/10.0;
            double py=((this_spacett&0x00000fffff00000ul)>>20)/10.0;
            double pz=(this_spacett&0x0000000000ffffful)/10.0;
            long this_space=(((long)(px*0.5))<<40)+(((long)(py))<<20)+(long)(pz*0.5);
            if(coll2m.count(this_space))
            {
                  if(!refed)
                  {
                        double opp=30-my_world.mulin_pos.y;
                        if(opp>0)
                              my_world.mulin_vel.y+=opp;
                        refed=true;
                  }
                  my_world.mulin_ground=true;
            }
            else
            {
                  this_space=(((long)(px*2))<<40)+(((long)(py*4))<<20)+(long)(pz*2);
                  if(coll50cm.count(this_space))
                  {
                        iter=coll50cm.find(this_space);
                        unsigned long col=iter->second;
                        int kind=col>>48;
                        switch(kind)
                        {
                        case(enum_id_plat):
                        {
                              my_world.mulin_ground=true;
                              if(!refed)
                              {
                                    double opp=30-my_world.mulin_pos.y;
                                    if(opp>0)
                                          my_world.mulin_vel.y+=opp;
                                    refed=true;
                              }
                        }
                              break;
                        case(enum_id_elevator):
                        {
                              my_world.mulin_ground=true;
                              if(my_world.elevator_state==0)
                                    my_world.elevator_state=1;
                              else if(my_world.elevator_state==2)
                                    my_world.elevator_state=3;
                              if(!refed)
                              {
                                    my_world.mulin_vel.y+=(my_world.elevator_height-my_world.mulin_pos.y);
                                    refed=true;
                              }
                        }
                              break;
                        default:
                        {
                              if(!refed)
                              {
                                    my_world.mulin_vel.x*=-1.1;
                                    if(my_world.mulin_vel.y<0)
                                          my_world.mulin_vel.y*=-1.8;
                                    else
                                          my_world.mulin_vel.y*=-0.9;
                                    my_world.mulin_vel.z*=-1.1;
                                    refed=true;
                              }
                        }
                              break;
                        }
                  }
                  else
                  {
                        if(coll10cm.count(this_spacett))
                        {
                              iter=coll10cm.find(this_spacett);
                              int kind=iter->second;
                              switch(kind)
                              {
                              case(enum_id_book):
                              {
                                    //unsigned long point=col&0xffffffffffff;
                              }
                                    break;
                              case(enum_id_cubes):
                              {
                                    if(!refed)
                                    {
                                          my_world.mulin_vel.x*=-1.1;
                                          if(my_world.mulin_vel.y<0)
                                                my_world.mulin_vel.y*=-1.8;
                                          else
                                                my_world.mulin_vel.y*=-0.9;
                                          my_world.mulin_vel.z*=-1.1;
                                          refed=true;
                                    }
                                    my_world.mulin_ground=true;
                              }
                                    break;
                              default:
                                    break;
                              }
                        }
                        else
                        {
                              coll10cm.insert(make_pair(this_spacett,long(enum_id_mulin)<<48));
                        }
                  }
            }
      }
      buffer_time++;
}

void collision_check::load_models()
{
      model_loaded=true;
      my_print.message("加载模型" );
      MuLin_model_display.init(my_file.pub_path+"../gra/model/mulin.ori");
      MuLin_model_display.copy_to(MuLin_model_const);
      fly_eat_model.init(my_file.pub_path+"../gra/model/fly_eat.ori");
      grass_eat_model.init(my_file.pub_path+"../gra/model/grass_eat.ori");
      meat_eat_model.init(my_file.pub_path+"../gra/model/meat_eat.ori");
      tree_model.init(my_file.pub_path+"../gra/model/tree.ori");

      my_print.message("加载动画数据");
      ifstream con(my_file.pub_path+"../gra/model/contrast.con", ios::in | ios::binary);
      string task_s, part_s;
      int con_num = 0;
      char ta[21] = {};
      char pa[21] = {};
      int len = 0;
      int part_order = -1, task_order = -1;

      while(con.read((char*)&len, 4))
      {
            con.read(ta, 20);
            ta[len] = '\0';
            task_s = ta;
            con.read((char*)&len, 4);
            con.read(pa, 20);
            pa[len] = '\0';
            part_s = pa;
            con.read((char*)&con_num, 4);
            // my_print.message("加载动画数据:" + task_s + " " + part_s);
            for(int i = 0; i < my_world.MuLin_task_num; ++i)
            {
                  if(task_s == my_world.Task_name[i])
                  {
                        task_order = i;
                        break;
                  }
            }
            if(task_order == -1)
            {
                  my_print.error("find unkown task name——" + task_s + " " + part_s);
            }
            for(int i = 0; i < my_world.MuLin_part_num; ++i)
            {
                  if(part_s == my_world.Part_name[i])
                  {
                        part_order = i;
                        break;
                  }
            }
            if(part_order == -1)
            {
                  my_print.error("find unkown part name——" + task_s + " " + part_s);
            }
            my_world.MuLin_task[task_order].task_part[my_world.MuLin_task[task_order].part_postion].init(con_num, part_order);
            con.read((char*)my_world.MuLin_task[task_order].task_part[my_world.MuLin_task[task_order].part_postion].contrast, 4 * con_num);
            my_world.MuLin_task[task_order].part_postion++;
            task_order = -1;
            part_order = -1;
      }
}

void collision_check::init()
{
      double a,b,c,l;
      double x1,y1,z1;
      double x2,y2,z2;
      double x3,y3,z3;
      for(int i=0; i<40; ++i)
      {
            for(int j=0; j<40; ++j)
            {
                  x1=i*5;
                  y1=j*5;
                  z1=my_world.map_height[i*41+j];
                  x2=i*5+5;
                  y2=j*5;
                  z2=my_world.map_height[i*41+j+1];
                  x3=i*5;
                  y3=j*5+5;
                  z3=my_world.map_height[i*41+j+41];

                  a=y1*z2-y1*z3-y2*z1+y2*z3+y3*z1-y3*z2;
                  b=-x1*z2+x1*z3+x2*z1-x2*z3-x3*z1+x3*z2;
                  c=x1*y2-x1*y3-x2*y1+x2*y3+x3*y1-x3*y2;

                  l=sqrt(pow(a,2)+pow(b,2)+pow(c,2));
                  reflect[i*40+j].x=a/l;
                  reflect[i*40+j].y=c/l;
                  reflect[i*40+j].z=b/l;

                  x1=i*5+5;
                  y1=j*5+5;
                  z1=my_world.map_height[i*41+j+42];

                  a=y1*z2-y1*z3-y2*z1+y2*z3+y3*z1-y3*z2;
                  b=-x1*z2+x1*z3+x2*z1-x2*z3-x3*z1+x3*z2;
                  c=x1*y2-x1*y3-x2*y1+x2*y3+x3*y1-x3*y2;

                  l=sqrt(pow(a,2)+pow(b,2)+pow(c,2));
                  reflect[i*40+j+1600].x=-a/l;
                  reflect[i*40+j+1600].y=-c/l;
                  reflect[i*40+j+1600].z=-b/l;

            }
      }
      if(!model_loaded)
      {
            load_models();
      }
}

double collision_check::map_height(double x,double y)
{
      unsigned int a=y/5;
      unsigned int b=x/5;
      if(a>=40||b>=40)
      {
            return 10;
      }
      else
      {
            if(x+y<5*(a+b)+5)
            {
                  double i=my_world.map_height[a*41+b];
                  double j=my_world.map_height[a*41+b+1];
                  double k=my_world.map_height[a*41+b+41];
                  return i+(j-i)*(x/5-b)+(k-i)*(y/5-a);
            }
            else
            {
                  double i=my_world.map_height[a*41+b+42];
                  double j=my_world.map_height[a*41+b+1];
                  double k=my_world.map_height[a*41+b+41];
                  return i+(k-i)*(1-(x/5-b))+(j-i)*(1-(y/5-a));
            }
      }
}

collision_check::~collision_check()
{
      //dtor
}
