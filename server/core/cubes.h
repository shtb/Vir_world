#ifndef CUBES_H
#define CUBES_H

#include"../../pub/spinlock.h"
#include"../../pub/STDHF.h"
#include"../base/lives.h"
#include"../base/world_structs.h"

struct cube
{
        float color[4];
        long position;
        cube*next=NULL;
        cube*pre=NULL;
};

class cubes
{
public:
      cubes();
      cube the_head;
      int live_num=0;
      int max_live_num=20480;
      cube* destroy(cube*de);
      void add(virw_position pos,float color[4]);
      void add(long pos,float color[4]);
};

#endif // CUBES_H
