#ifndef WORLD_H
#define WORLD_H

#include"../../pub/STDHF.h"
#include"../../pub/thread_control.h"
#include"../base/GL_task.h"
#include"../../pub/time_control.h"
#include"../base/world_structs.h"
#include"virw_grains.h"
#include"virw_weather.h"
#include"collision_check.h"
#include"fly_eats.h"
#include"grass_eats.h"
#include"meat_eats.h"
#include"trees.h"
#include"cubes.h"
#include"world_switch.h"


class world:public thread_control
{
public:
      bool mulin_ground=false;
      int cubes_changed=1;
      unsigned int cycle_time=33333;
      unsigned int run_count=0;
      set<shoot*>shoots;
      world_switch my_world_switch;
      time_control virw_time_control;
      distortion mulin_distortion;
      virw_grain grains;
      fly_eats w_fly_eats;
      grass_eats w_grass_eats;
      trees w_trees;
      cubes w_cubes;      
      spinlock w_cubes_lock;
      meat_eats w_meat_eats;
      virw_weather weather;
      collision_check collision;

      double elevator_height=30.0;
      int elevator_state=0;//0:above,1:downing,2:below,3:uping

      double map_height[41 * 41];
      double Cr,Cg,Cb,Ca;
      float fog_density;
      double gravity_accleration=9.8;

      virw_position mulin_pos;
      virw_velocity mulin_vel;
      double mulin_energy=1.0;
      double mulin_health=1.0;

      bool mulin_attach=false;
      bool attach_check=false;

      static const int MuLin_part_num = 14;
      string Part_name[MuLin_part_num];
      static const int MuLin_task_num = 16;
      GL_task MuLin_task[MuLin_task_num];
      bool MuLin_show[20] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
      string Task_name[MuLin_task_num];

      void net_send();
      void rand_color();

      void physic();
      void physic_gravity_wind();
      void physic_collision();
      void physic_move();

      int t_load();
      int t_loop();

      void map_height_init(std::string file_name);

      world();
      virtual ~world();
protected:
private:
};

#endif // WORLD_H
