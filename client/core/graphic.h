#ifndef GRAP
#define GRAP

#include "../../pub/STDHF.h"
#include <GL/glut.h>
#include"unistd.h"
#include<X11/X.h>
#include<X11/Xlib.h>
#include<X11/Xutil.h>
#include "character_controller.h"
#include "../../pub/thread_control.h"
#include "../base/GL_model.h"
#include "../base/gra_structs.h"
#include"../base/paint_board.h"
#include"../../pub/spinlock.h"
#include<set>

class graphic : public thread_control
{
public:
      virw_position cub_tem1,cub_tem2;

      double elevator_height=30;
      bool eye_pic_ch=false;
      char*eye_picture;
      string color_file_name;
      unsigned char shoot_kind=enum_shoot_destroy;
      unsigned int shoot_speed=0;
      double construct_cube_color[4]={0.3,0.3,1.0,1.0};
      bool rand_cube_color=true;
      bool mirror=false;
      bool use3d=false;
      bool p3d_red=true;
      paint_board big_paint;
      //////////////////////////////////
      bool refresh_by_net=false;
      bool full_screen = true;
      Display *dpy;
      Window root_window;
      int world_changed=-1;//-1:ok;0:change_pre;1-5:changed;

      bool mouse_button_right=false;
      bool mouse_button_left=false;
      bool post=true;
      set<gra_client*>gra_clients;
      spinlock gra_clients_lock;
      set<shoot*>shoots;
      spinlock shoots_lock;
      static const int grain_MAX_NUM = 10000;
      int live_grain_num = 0;
      grain grains[grain_MAX_NUM];

      int live_fly_eat_num = 0;
      float_virw_position fly_eats[100];
      float fly_eats_big[100];
      bool fly_eats_dead[100];

      int live_grass_eat_num = 0;
      float_virw_position grass_eats[100];
      float grass_eats_big[100];
      bool grass_eats_dead[100];

      int live_meat_eat_num = 0;
      float_virw_position meat_eats[100];
      float meat_eats_big[100];
      bool meat_eats_dead[100];

      int live_tree_num = 0;
      float_virw_position trees[300];
      float trees_big[300];

      int live_cube_num = 0;
      long cubes[20480];
      float cubes_color[20480*4];

      double picture_num = 60;
      int argc;
      char** argv;
      double speed = 0.01;
      bool gear_transparent=true;
      GL_model World_gear_model[15];
      GL_model models[20];
      GL_model MuLin_model_const, MuLin_model_display;
      gra_fog fog;
      gra_accum accum;
      GLuint MuLin_eye_texture,World_texture_front, World_texture_behind, World_texture_left, World_texture_right, World_texture_above,
      World_texture_below;
      double Cr, Cg, Cb, Ca;
      character_controller my_font;

      virw_position mulin_pos;
      virw_position mulin_look;
      virw_position mulin_distortion;
      virw_position my_pos;
      virw_position eye_focus;
      virw_position* look_at;

      static const int MuLin_part_num = 14;
      string Part_name[MuLin_part_num];
      static const int MuLin_task_num = 16;
      GL_task MuLin_task[MuLin_task_num];
      string Task_name[MuLin_task_num];
      bool MuLin_show[20] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
      double head_eye_angle[2];

      double light = 0.8;
      double map_height[41 * 41];
      bool net_flush;

      void mixture(int task_or, int task_part_or);
      void init_task();

      int t_run();
      int t_load();
      int t_save();
      double get_map_height(double, double);

      void key(unsigned char key, int, int);
      void keyup(unsigned char key, int, int);
      void skey(int key, int, int);
      void skeyup(int key, int, int);
      void keys();

      void display_world();
      void load_texture();
      void display_light();
      void display_char();
      void display_paint();
      void display();
      void load_models();
      void idle();
      graphic();
      virtual ~graphic();

protected:
private:
};

#endif // GRAP
