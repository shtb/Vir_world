#include "world.h"
#include"../base/interfaces.h"
#include<iomanip>

using namespace std;

unsigned int mm[20][20]= {};
unsigned int mmb=0;

world::world()
{
    string tem1[] = { "wind",     "wink",        "smile",        "wind2", "sad",       "right_leg",
			    "left_leg", "right_hand1", "left_hand1",   "fight", "left_head", "right_head",
			    "head_up",  "head_down",   "battle_state", "squat"
			  };
    for(int i = 0; i < MuLin_task_num; ++i)
	  Task_name[i] = tem1[i];
    string tem2[] = { "skirt", "ear",   "eyebrow", "shawl", "eye",    "head",    "leg",
			    "lash",  "pupil", "hair",    "chest", "armlet", "necktie", "shoe"
			  };
    for(int i = 0; i < MuLin_part_num; ++i)
	  Part_name[i] = tem2[i];
    mulin_distortion.excursion.x=0;
    mulin_distortion.excursion.y=0;
    mulin_distortion.excursion.z=0;
    mulin_distortion.excursion.a=0;
    mulin_distortion.excursion.b=0;
    mulin_distortion.excursion.c=0;

    mulin_pos.z=102;
}

void world::map_height_init(string file_name)
{
    int width, height, total_bytes;
    char* pixels = 0;
    bool reshape=false;

    // 打开文件，如果失败，返回
    ifstream pFile(file_name, ios::in | ios::binary);
    if (!pFile)
    {
	  pFile.close();
	  string newfi=file_name.substr(0,file_name.length()-9)+"front.bmp";
	  pFile.open(newfi,ios::in|ios::binary);
	  reshape=true;
    }
    if (!pFile)
    {
	  my_print.error("bmp_file not found to init map height!"+ file_name);
	  pFile.close();
	  return;
    }

    pFile.seekg(0x0012);
    pFile.read((char*)&width, 4);
    pFile.read((char*)&height, 4);
    pFile.seekg(54);

    // 计算每行像素所占字节数，并根据此数据计算总像素字节数

    int line_bytes = width * 3;
    while (line_bytes % 4 != 0)
	  ++line_bytes;
    total_bytes = line_bytes * height;


    // 根据总像素字节数分配内存
    pixels = new char[total_bytes];
    if (pixels == 0)
    {
	  cerr << "Mem not get! for texture!!!" << endl;
	  pFile.close();
	  return;
    }

    // 读取像素数据
    if (!pFile.read((char*)pixels, total_bytes))
    {
	  cerr << "bmp read failed!!!" << endl;
	  delete[] pixels;
	  pFile.close();
	  return;
    }

    if(reshape)
    {
	  if(height!=width)
	  {
		my_print.warning("the height and width of "+file_name+"is not equal!");
	  }
	  char* oripixels = new char[total_bytes];
	  memcpy(oripixels,pixels,total_bytes);

	  for (int i = height / 2; i < height; ++i)
	  {
		int right = i + 1;
		int w = height - i - 1;
		while (w < right)
		{
		    pixels[i*line_bytes + 3 * w] = oripixels[line_bytes*(height-i-1) + 3 * (w)];
		    pixels[i*line_bytes + 3 * w + 1] = oripixels[line_bytes*(height-i-1) + 3 * (w) + 1];
		    pixels[i*line_bytes + 3 * w + 2] = oripixels[line_bytes*(height-i-1) + 3 * (w) + 2];
		    w++;
		}
	  }
	  for (int i = 0; i < height / 2; ++i)
	  {
		int right = height - i - 1;
		int w = i;
		while (w < right)
		{
		    pixels[i*line_bytes + 3 * w] = oripixels[line_bytes*i + 3 * (width - w - 1)];
		    pixels[i*line_bytes + 3 * w + 1] = oripixels[line_bytes*i + 3 * (width - w - 1) + 1];
		    pixels[i*line_bytes + 3 * w + 2] = oripixels[line_bytes*i + 3 * (width - w - 1) + 2];
		    w++;
		}
	  }
	  for (int i = 0; i < height; ++i)
	  {
		int right = height / 2 - abs(i - height / 2);
		int w = 0;
		while (w < right)
		{
		    pixels[i*line_bytes + 3 * w] = oripixels[line_bytes*(w) + 3 *(width-i-1)];
		    pixels[i*line_bytes + 3 * w + 1] = oripixels[line_bytes*(w) + 3 *(width-i-1) + 1];
		    pixels[i*line_bytes + 3 * w + 2] = oripixels[line_bytes*(w) + 3 *(width-i-1) + 2];
		    w++;
		}
	  }
	  for (int i = 0; i < height; ++i)
	  {
		int w = width - (height / 2 - abs(i - height / 2)) - 1;
		while (w < width)
		{
		    pixels[i*line_bytes + 3 * w] = oripixels[line_bytes*(height-w-1)+ 3 * (i)];
		    pixels[i*line_bytes + 3 * w + 1] = oripixels[line_bytes*(height-w-1)+ 3 * (i) + 1];
		    pixels[i*line_bytes + 3 * w + 2] = oripixels[line_bytes*(height-w-1)+ 3 * (i) + 2];
		    w++;
		}
	  }
	  delete[] oripixels;
    }

    double pix_num_ratio_w = width / 40.0;
    double pix_num_ratio_h = height / 40.0;
    double all=0,allr=0;
    for (int i = 0; i < 40; ++i)
    {
	  for (int j = 0; j < 40; ++j)
	  {
		double s = 0,sr=0,sg=0;
		int k=0, l=0;
		for (k = 0; k < pix_num_ratio_h; ++k)
		{
		    for (l = 0; l < pix_num_ratio_w; ++l)
		    {
			  sr+=(unsigned char)pixels[(int)(line_bytes*((int)(i*pix_num_ratio_h + k)) + 3 * (j*pix_num_ratio_w + l))];
			  sg+=(unsigned char)pixels[(int)(line_bytes*((int)(i*pix_num_ratio_h + k)) + 3 * (j*pix_num_ratio_w + l)) + 1];
			  s += (unsigned char)pixels[(int)(line_bytes*((int)(i*pix_num_ratio_h + k)) + 3 * (j*pix_num_ratio_w + l))];
			  s += (unsigned char)pixels[(int)(line_bytes*((int)(i*pix_num_ratio_h + k)) + 3 * (j*pix_num_ratio_w + l)) + 1];
			  s += (unsigned char)pixels[(int)(line_bytes*((int)(i*pix_num_ratio_h + k)) + 3 * (j*pix_num_ratio_w + l)) + 2];
		    }
		}
		map_height[i * 41 + j] = s / (k*l*30);
		weather.life_profile[i*40+j]=sg/s;
		weather.temperature_profile[i*40+j]=20*(sr/s)-6;
		all+=s;
		allr+=sr;
	  }
    }
    weather.average_temperature=100*(allr/all)-20;
    for (int i = 0; i < 40; ++i)
    {
	  for (int j = 0; j < 40; ++j)
	  {
		weather.temperature_profile[i*40+j]+=weather.average_temperature;
	  }
    }
    for (int i = 0; i < 41; ++i)
    {
	  map_height[i] = 10;
	  map_height[i + 41 * 40] = 10;
	  map_height[i * 41 + 40] = 10;
	  map_height[i * 41] = 10;
    }
    delete[] pixels;
    pFile.close();
}

int world::t_load()
{
    MuLin_task[wind].init(3);
    MuLin_task[wind].play_on=true;
    MuLin_task[wind2].init(3);
    MuLin_task[wind2].play_on=true;
    MuLin_task[wink].init(2, true, 1, false, false, 0.017);
    MuLin_task[wink].set_state(1, 1);
    MuLin_task[smile].init(3);
    MuLin_task[sad].init(3);
    MuLin_task[right_leg].init(3, true, 2, false, false, 0.05);
    MuLin_task[left_leg].init(3, true, 2, false, false, 0.05);
    MuLin_task[right_hand1].init(2, true, 1, false, false, 0.05);
    MuLin_task[left_hand1].init(2, true, 1, false, false, 0.06);
    MuLin_task[fight].init(4, true, 3, false, false, 0.16);
    MuLin_task[left_head].init(7);
    MuLin_task[right_head].init(7);
    MuLin_task[head_up].init(7);
    MuLin_task[head_down].init(7);
    MuLin_task[battle_state].init(3, false, 0, false, false, 0.02);
    MuLin_task[squat].init(3, false, 0, false, false, 0.02);
    my_world_switch.init();
    map_height_init(my_file.files[enum_file_world] + "_below.bmp");
    collision.init();
    for(int i=0; i<50; i++)
    {
	  float_virw_position pos;
	  pos.x=(rand()%18000)/100.0+10;
	  pos.z=(rand()%18000)/100.0+10;
	  pos.y=collision.map_height(pos.x,pos.z);
	  w_trees.add(pos,1.0);
    }
    virw_time_control.wait_for(0);
    return 0;
}

void world::net_send()
{
    {
	  stringstream ss;
	  set<shoot*>::iterator iter;
	  int num=shoots.size();
	  ss.write((char*)&num,4);
	  for (iter = shoots.begin(); iter != shoots.end(); iter++)
	  {
		ss.write((char*)*iter,sizeof(shoot));
	  }
	  string ssstr=ss.str();
	  my_net.send_to("shoots",ssstr,my_net.broadcast_socket);
    }

    {
	  stringstream ss;
	  set<gra_client*>::iterator iter;
	  int num=my_net.gra_clients.size();
	  ss.write((char*)&num,4);
	  for (iter = my_net.gra_clients.begin(); iter != my_net.gra_clients.end(); iter++)
	  {
		ss.write((char*)*iter,sizeof(gra_client));
	  }
	  string ssstr=ss.str();
	  my_net.send_to("gra_cl",ssstr,my_net.broadcast_socket);
    }
    //grains
    {
	  stringstream ss;
	  grains.lock.slock();
	  grain*p=grains.grains.next;
	  ss.write((char*)&grains.live_num,sizeof(int));
	  while(p!=NULL)
	  {
		ss.write((char*)&(p->model),sizeof(int));
		ss.write((char*)&(p->position),sizeof(float_virw_position));
		ss.write((char*)&(p->color),sizeof(float)*3);
		p=p->next;
	  }
	  grains.lock.unlock();
	  string ssstr=ss.str();
	  my_net.send_to("grains",ssstr,my_net.broadcast_socket);
    }
    //fly_eats
    {
	  stringstream ss;
	  life*p=w_fly_eats.the_head.next;
	  ss.write((char*)&w_fly_eats.live_num,sizeof(int));
	  while(p!=NULL)
	  {
		ss.write((char*)&(p->position),sizeof(float_virw_position));
		ss.write((char*)&(p->life),sizeof(float));
		ss.write((char*)&(p->dead),sizeof(bool));
		p=p->next;
	  }
	  string ssstr=ss.str();
	  my_net.send_to("fly_ea",ssstr,my_net.broadcast_socket);
    }

    //meat_eats
    {
	  stringstream ss;
	  life*p=w_meat_eats.the_head.next;
	  ss.write((char*)&w_meat_eats.live_num,sizeof(int));
	  while(p!=NULL)
	  {
		ss.write((char*)&(p->position),sizeof(float_virw_position));
		ss.write((char*)&(p->life),sizeof(float));
		ss.write((char*)&(p->dead),sizeof(bool));
		p=p->next;
	  }
	  string ssstr=ss.str();
	  my_net.send_to("meat_e",ssstr,my_net.broadcast_socket);
    }

    //grass_eats
    {
	  stringstream ss;
	  life*p=w_grass_eats.the_head.next;
	  ss.write((char*)&w_grass_eats.live_num,sizeof(int));
	  while(p!=NULL)
	  {
		ss.write((char*)&(p->position),sizeof(float_virw_position));
		ss.write((char*)&(p->life),sizeof(float));
		ss.write((char*)&(p->dead),sizeof(bool));
		p=p->next;
	  }
	  string ssstr=ss.str();
	  my_net.send_to("grass_",ssstr,my_net.broadcast_socket);
    }

    //trees
    static int wti=0;
    wti++;
    if(wti%4==0)
    {
	  stringstream ss;
	  life*p=w_trees.the_head.next;
	  ss.write((char*)&w_trees.live_num,sizeof(int));
	  while(p!=NULL)
	  {
		ss.write((char*)&(p->position),sizeof(float_virw_position));
		ss.write((char*)&(p->life),sizeof(float));
		p=p->next;
	  }
	  string ssstr=ss.str();
	  my_net.send_to("trees_",ssstr,my_net.broadcast_socket);
    }

    if(cubes_changed)
    {
	  w_cubes_lock.slock();
	  cubes_changed--;
	  stringstream ss;
	  cube*p=w_cubes.the_head.next;
	  ss.write((char*)&w_cubes.live_num,sizeof(int));
	  while(p!=NULL)
	  {
		ss.write((char*)&(p->position),sizeof(long));
		ss.write((char*)&(p->color),sizeof(float)*4);
		p=p->next;
	  }
	  w_cubes_lock.unlock();
	  string ssstr=ss.str();
	  my_net.send_to("constr",ssstr,my_net.broadcast_socket);
    }

    {
	  stringstream ss;

	  ss.write((char*)&mulin_pos,sizeof(virw_position));
	  ss.write((char*)&Cr,sizeof(double));
	  ss.write((char*)&Cg,sizeof(double));
	  ss.write((char*)&Cb,sizeof(double));
	  ss.write((char*)&Ca,sizeof(double));
	  ss.write((char*)&fog_density,sizeof(float));
	  ss.write((char*)&my_time.big_t,sizeof(int));
	  ss.write((char*)&my_time.small_t,sizeof(int));
	  ss.write((char*)&run_count,sizeof(unsigned int));
	  ss.write((char*)weather.virw_wind,sizeof(double)*2);
	  ss.write((char*)&elevator_height,sizeof(double));

	  string ssstr=ss.str();
	  my_net.send_to("task_c",ssstr,my_net.mulin_socket);
    }

    {
	  stringstream ss;
	  for (int i = 0; i < my_world.MuLin_task_num; ++i)
	  {
		ss.write((char*)&my_world.MuLin_task[i].play_on,sizeof(bool));
		ss.write((char*)&my_world.MuLin_task[i].state,sizeof(double));
	  }
	  for(int i=0; i<my_world.MuLin_part_num; ++i)
	  {
		ss.write((char*)&my_world.MuLin_show[i],sizeof(bool));
	  }
	  ss.write((char*)&my_world.mulin_pos,sizeof(virw_position));
	  ss.write((char*)&my_world.mulin_distortion.excursion,sizeof(virw_position));
	  ss.write((char*)&my_world.Cr,sizeof(double));
	  ss.write((char*)&my_world.Cg,sizeof(double));
	  ss.write((char*)&my_world.Cb,sizeof(double));
	  ss.write((char*)&my_world.Ca,sizeof(double));
	  ss.write((char*)&my_world.fog_density,sizeof(float));
	  ss.write((char*)&my_time.big_t,sizeof(int));
	  ss.write((char*)&my_time.small_t,sizeof(int));
	  ss.write((char*)&my_world.run_count,sizeof(unsigned int));

	  ss.write((char*)&my_world.mulin_distortion.head_eye_angle,sizeof(double)*2);
	  ss.write((char*)&elevator_height,sizeof(double));

	  string ssstr=ss.str();
	  my_net.send_to("task_c",ssstr,my_net.shtb_and_unknown_socket);
    }
}

void world::rand_color()
{
    static double con1, con2, con3, con4,co1 = Cr, co2 = Cg, co3 = Cb,co4=Ca;
    double ra;
    if (fabs(Cr - co1) < 0.01&&fabs(Cg - co2) < 0.01)
    {
	  ra = rand() % 14000;
	  co1 = ra / 10000.0-0.2;
	  ra = rand() % 14000;
	  co2 = ra / 10000.0-0.2;
	  ra = rand() % 14000;
	  co3 = ra / 10000.0-0.2;
	  ra = rand() % 10000;
	  co4 = ra / 10000.0;
	  con1 = (co1 - Cr) / 1000;
	  con2 = (co2 - Cg) / 1000;
	  con3 = (co3 - Cb) / 1000;
	  con4 = (co4 - Ca) / 1000;
    }
    Cr += con1;
    Cg += con2;
    Cb += con3;
    Ca+=con4;
}

int world::t_loop()
{
    ++run_count;
    float_virw_position pos;
    if(w_fly_eats.live_num==0&&run_count%5000==1000)
    {
	  pos.x=(rand()%20000)/100.0;
	  pos.y=30+(rand()%7000)/100.0;
	  pos.z=(rand()%20000)/100.0;
	  w_fly_eats.add(pos,1.0);
    }
    if(w_grass_eats.live_num==0&&run_count%5000==2000)
    {

	  pos.x=(rand()%20000)/100.0;
	  pos.z=(rand()%20000)/100.0;
	  pos.y=collision.map_height(pos.x,pos.z);
	  w_grass_eats.add(pos,0.8);
    }
    if(w_trees.live_num==0&&run_count%5000==3000)
    {

	  pos.x=(rand()%18000)/100.0+10;
	  pos.z=(rand()%18000)/100.0+10;
	  pos.y=collision.map_height(pos.x,pos.z);
	  w_trees.add(pos,0.8);
    }
    if(w_meat_eats.live_num==0&&run_count%5000==4000)
    {

	  pos.x=(rand()%20000)/100.0;
	  pos.z=(rand()%20000)/100.0;
	  pos.y=collision.map_height(pos.x,pos.z);
	  w_meat_eats.add(pos,0.8);
    }

    my_time.time_go();
    weather.time_move();
    rand_color();

    bool world_switched=true;
    if(mulin_pos.x<0.5)
    {
	  if(my_world_switch.switch_world(enum_direction_west))
		mulin_pos.x+=199;
	  else
	  {
		mulin_pos.x=0.5;
		world_switched=false;
	  }
    }
    else if(mulin_pos.z<0.5)
    {
	  if(my_world_switch.switch_world(enum_direction_north))
		mulin_pos.z+=199;
	  else
	  {
		mulin_pos.z=0.5;
		world_switched=false;
	  }
    }
    else if(mulin_pos.x>199.5)
    {
	  if(my_world_switch.switch_world(enum_direction_east))
		mulin_pos.x-=199;
	  else
	  {
		mulin_pos.x=199.5;
		world_switched=false;
	  }
    }
    else if(mulin_pos.z>199.5)
    {
	  if(my_world_switch.switch_world(enum_direction_south))
		mulin_pos.z-=199;
	  else
	  {
		mulin_pos.z=199.5;
		world_switched=false;
	  }
    }
    else
    {
	  world_switched=false;
    }
    if(world_switched)
    {
	  map_height_init(my_file.files[enum_file_world] + "_below.bmp");
	  collision.init();
	  {
		stringstream ssa;
		int len=my_file.files[enum_file_world].length();
		ssa.write((char*)&len,sizeof(int));
		ssa.write((char*)my_file.files[enum_file_world].data(),len);
		ssa.write((char*)(my_world.map_height),sizeof(double)*41*41);
		string ssstr=ssa.str();
		my_net.send_to("map_he",ssstr,my_net.broadcast_socket);
	  }
    }

    physic();
    if(cycle_time<30000)
    {
	  static int tmc=0;
	  tmc++;
	  if((unsigned)tmc>=33333/cycle_time)
	  {
		tmc=0;
		net_send();
	  }
    }
    else
    {
	  net_send();
    }
    virw_time_control.wait_for(cycle_time);
    return 0;
}

void world::physic()
{
    physic_collision();
    physic_gravity_wind();
    physic_move();
}

void world::physic_gravity_wind()
{
    {
	  mulin_vel.y-=0.01;
	  mulin_vel.x*=0.9;
	  mulin_vel.y*=0.9;
	  mulin_vel.z*=0.9;
	  mulin_vel.b*=0.9;
	  mulin_ground=false;
    }

    {
	  grain*p=grains.grains.next;
	  double wind[2];
	  wind[0]=weather.virw_wind[0];
	  wind[1]=weather.virw_wind[1];

	  while(p!=NULL)
	  {
		bool transed=false;
		if(p->position.x<1||p->position.z<1||p->position.x>199||p->position.z>199||p->position.y<-1)
		    transed=true;
		if((transed||p->life<0)&&!p->attach)
		{
		    p->pre->next=p->next;
		    if(p->next!=NULL)
		    {
			  p->next->pre=p->pre;
		    }
		    grain*de=p;
		    p=p->next;
		    delete de;
		    --grains.live_num;
		    continue;
		}
		double hei=collision.map_height(p->position.x,p->position.z)+0.05;
		if(p->position.y<hei)
		{
		    --p->life;
		    double grou_re=hei-p->position.y;
		    collision_reflect refl=my_world.collision.map_reflect(p->position.x,p->position.z);
		    double vg=p->velocity.x*refl.x+p->velocity.y*refl.y+p->velocity.z*refl.z-grou_re*0.3;
		    p->velocity.x-=2*vg*refl.x;
		    p->velocity.y-=2*vg*refl.y;
		    p->velocity.z-=2*vg*refl.z;
		}
		else
		{
		    p->velocity.y-=p->mass*gravity_accleration;
		}

		double t_wind=pow(1-p->wind_resistance,0.3);
		if(p->position.y>hei+0.1)
		{
		    p->velocity.x+=0.1*(wind[0]*t_wind-p->velocity.x);
		    p->velocity.z+=0.1*(wind[1]*t_wind-p->velocity.z);
		}
		else
		{
		    p->velocity.x+=0.1*(0.1*wind[0]*t_wind-p->velocity.x);
		    p->velocity.z+=0.1*(0.1*wind[1]*t_wind-p->velocity.z);
		}

		p->velocity.y*=p->wind_resistance;
		p->velocity.x*=p->wind_resistance;
		p->velocity.z*=p->wind_resistance;
		p=p->next;
	  }
    }

    {
	  life*p=w_fly_eats.the_head.next;
	  while(p!=NULL)
	  {
		p->velocity.y-=0.02;

		p->velocity.x*=0.7;
		p->velocity.y*=0.7;
		p->velocity.z*=0.7;
		p->velocity.a*=0.5;
		p->velocity.b*=0.5;
		p->velocity.c*=0.5;
		if(p->ground)
		{
		    collision_reflect refl=collision.map_reflect(p->position.x,p->position.z);
		    p->velocity.x+=0.06*refl.x;
		    p->velocity.y+=0.06*refl.y;
		    p->velocity.z+=0.06*refl.z;
		}
		p=p->next;
	  }
    }

    {
	  life*p=w_grass_eats.the_head.next;
	  while(p!=NULL)
	  {
		p->velocity.x*=0.8;
		p->velocity.y*=0.8;
		p->velocity.z*=0.8;
		p->velocity.a*=0.5;
		p->velocity.b*=0.5;
		p->velocity.c*=0.5;
		p->velocity.y-=0.1;
		if(p->ground)
		{
		    collision_reflect refl=my_world.collision.map_reflect(p->position.x,p->position.z);
		    p->velocity.x+=0.07*refl.x;
		    p->velocity.y+=0.3*refl.y;
		    p->velocity.z+=0.07*refl.z;
		    float dxz=sqrt(refl.x*refl.x+refl.z*refl.z);
		    if(abs(refl.y)>0.02)
			  p->position.a=atan(dxz/refl.y)*57.3;
		    else
			  p->position.a=0;
		}
		p=p->next;
	  }
    }

    {
	  life*p=w_meat_eats.the_head.next;
	  while(p!=NULL)
	  {
		p->velocity.x*=0.7;
		p->velocity.y*=0.7;
		p->velocity.z*=0.7;
		p->velocity.a*=0.5;
		p->velocity.b*=0.5;
		p->velocity.c*=0.5;
		p->velocity.y-=0.1;
		if(p->ground)
		{
		    collision_reflect refl=my_world.collision.map_reflect(p->position.x,p->position.z);
		    p->velocity.x+=0.09*refl.x;
		    p->velocity.y+=0.3*refl.y;
		    p->velocity.z+=0.09*refl.z;
		    float dxz=sqrt(refl.x*refl.x+refl.z*refl.z);
		    if(abs(refl.y)>0.02)
			  p->position.a=atan(dxz/refl.y)*57.3;
		    else
			  p->position.a=0;
		}
		p=p->next;
	  }
    }
}

void world::physic_collision()
{
    long*data;
    long tkind=0;
    long this_space;
    map<long,long>::iterator ite;
    if(cubes_changed)
	  collision.const_changed=true;
    if(collision.const_changed)
    {
	  collision.coll2m.clear();
	  data=collision.plat_2m;
	  tkind=enum_id_plat;
	  tkind=tkind<<48;
	  for(int i=0; i<collision.plat_2m_num; ++i)
	  {
		collision.coll2m.insert(make_pair(data[i],tkind));
	  }

	  collision.const_coll10cm.clear();
	  //book
	  tkind=enum_id_book;
	  tkind=tkind<<48;
	  this_space=(((long)(100.05*10))<<40)+(((long)(31.25*10))<<20)+(long)(104.05*10);
	  collision.const_coll10cm.insert(make_pair(this_space,tkind));

	  //cubes
	  tkind=enum_id_cubes;
	  tkind=tkind<<48;
	  w_cubes_lock.slock();
	  {
		cube*p=w_cubes.the_head.next;
		while(p!=NULL)
		{
		    if(collision.const_coll10cm.count(p->position))
		    {
			  ite=collision.const_coll10cm.find(p->position);
			  long kind=ite->second;
			  kind=kind>>48;
			  switch(kind)
			  {
			  case(enum_id_cubes):
			  {
				p=w_cubes.destroy(p);
			  }
				break;
			  default:
			  {
				p=p->next;
			  }
				break;
			  }
		    }
		    else
		    {
			  collision.const_coll10cm.insert(make_pair(p->position,tkind));
			  p=p->next;
		    }
		}
	  }
	  {
		cube*p=w_cubes.the_head.next;
		while(p!=NULL)
		{
		    if(p->color[3]<0)
		    {
			  p=w_cubes.destroy(p);
		    }
		    else
		    {
			  p=p->next;
		    }
		}
	  }
	  w_cubes_lock.unlock();

	  collision.const_changed=false;
    }
    collision.coll10cm=collision.const_coll10cm;
    collision.coll50cm.clear();

    map<long,long>*coll2m=&collision.coll2m;
    map<long,long>*coll50cm=&collision.coll50cm;
    map<long,long>*coll10cm=&collision.coll10cm;
    data=collision.plat_50cm;
    tkind=enum_id_plat;
    tkind=tkind<<48;
    for(int i=0; i<collision.plat_50cm_num; ++i)
    {
	  coll50cm->insert(make_pair(data[i],tkind));
    }

    if(elevator_state==1)
    {
	  elevator_height-=0.05;
	  if(elevator_height<collision.map_height(100,100)+0.5)
	  {
		elevator_state=-190;
	  }
    }
    else if(elevator_state==3)
    {
	  elevator_height+=0.05;
	  if(elevator_height>29.99)
	  {
		elevator_state=190;
	  }
    }
    if(elevator_state>10)
    {
	  elevator_state--;
	  if(elevator_state==100)
		elevator_state=0;
    }
    else if(elevator_state<-10)
    {
	  elevator_state++;
	  if(elevator_state==-100)
		elevator_state=2;
    }

    if(elevator_height<collision.map_height(100,100)+0.4)
    {
	  elevator_height= collision.map_height(100,100)+0.5;
    }

    long tr_ele=(long)((elevator_height-30.0)*4)<<20;
    data=collision.elevator_50cm;
    tkind=enum_id_elevator;
    tkind=tkind<<48;
    for(int i=0; i<collision.elevator_50cm_num; ++i)
    {
	  coll50cm->insert(make_pair(data[i]+tr_ele,tkind));
    }

    tkind=enum_id_client;
    tkind=tkind<<48;
    //clients
    {
	  set<gra_client*>::iterator iter;
	  for (iter = my_net.gra_clients.begin(); iter != my_net.gra_clients.end(); iter++)
	  {
		if((*iter)->attach!=NULL)
		{
		    switch((*iter)->attach_kind)
		    {
		    case(enum_kind_grain):
			  ((grain*)((*iter)->attach))->position.x=(*iter)->eye_look.x;
			  ((grain*)((*iter)->attach))->position.y=(*iter)->eye_look.y;
			  ((grain*)((*iter)->attach))->position.z=(*iter)->eye_look.z;
			  break;
		    case(enum_kind_life):
			  ((life*)((*iter)->attach))->position.x=(*iter)->eye_look.x;
			  ((life*)((*iter)->attach))->position.y=(*iter)->eye_look.y;
			  ((life*)((*iter)->attach))->position.z=(*iter)->eye_look.z;
			  break;
		    case(enum_kind_mulin):
		    {
			  if(mulin_attach&&attach_check)
			  {
				mulin_pos.x=(*iter)->eye_look.x;
				mulin_pos.y=(*iter)->eye_look.y;
				mulin_pos.z=(*iter)->eye_look.z;
			  }
		    }
			  break;
		    default:
			  break;
		    }
		}
		if((*iter)->login_state!=enum_login_mulin)
		{
		    this_space=(((long)((*iter)->pos.x*2))<<40)+(((long)(((*iter)->pos.y-0.125)*4))<<20)+(long)((*iter)->pos.z*2);
		    coll50cm->insert(make_pair(this_space,tkind));
		}
	  }
    }

    //mulin
    collision.add_mulin();

    //trees
    {
	  life*p=w_trees.the_head.next;
	  while(p!=NULL)
	  {
		collision.add(p,enum_id_tree);
		p=p->next;
	  }
    }

    //grass_eats
    {
	  life*p=w_grass_eats.the_head.next;
	  while(p!=NULL)
	  {
		unsigned int a,b;
		a=p->position.x/10;
		b=p->position.z/10;
		if(a<20&&b<20&&!p->dead)
		    mm[a][b]++;
		if(mm[a][b]>mmb)
		    mmb=mm[a][b];
		collision.add(p,enum_id_grass_eat);
		p=p->next;
	  }
    }

    //fly_eats
    {
	  life*p=w_fly_eats.the_head.next;
	  while(p!=NULL)
	  {
		collision.add(p,enum_id_fly_eat);
		p=p->next;
	  }
    }

    //meat_eats
    {
	  life*p=w_meat_eats.the_head.next;
	  while(p!=NULL)
	  {
		collision.add(p,enum_id_meat_eat);
		p=p->next;
	  }
    }

    //grain
    {
	  grain*p=grains.grains.next;
	  while(p!=NULL)
	  {
		//////////////////////////////////////////////////////////////////////////
		this_space=(((long)(p->position.x*0.5))<<40)+(((long)(p->position.y))<<20)+(long)(p->position.z*0.5);
		if(coll2m->count(this_space))
		{
		    p->velocity.y+=(30-p->position.y);
		}
		else
		{
		    this_space=(((long)(p->position.x*2))<<40)+(((long)(p->position.y*4))<<20)+(long)(p->position.z*2);
		    if(coll50cm->count(this_space))
		    {
			  p->velocity.y+=(30-p->position.y);
		    }
		    else
		    {
			  this_space=(((long)(p->position.x*10))<<40)+(((long)(p->position.y*10))<<20)+(long)(p->position.z*10);
			  if(coll10cm->count(this_space))
			  {
				ite=coll10cm->find(this_space);
				unsigned long col=ite->second;
				int kind=col>>48;
				switch(kind)
				{
				case(enum_id_maple):
				case(enum_id_hail):
				case(enum_id_snow):
				case(enum_id_gear):
				case(enum_id_petal):
				{
				    unsigned long point=col&0xffffffffffff;
				    //if(p->position.y>((grain*)point)->position.y)
				    {
					  p->velocity.x+=(p->position.x-((grain*)point)->position.x);
					  p->velocity.y+=(p->position.y-((grain*)point)->position.y);
					  p->velocity.z+=(p->position.z-((grain*)point)->position.z);
				    }
				}
				    break;
				case(enum_id_tree):
				{
				    unsigned long point=col&0xffffffffffff;
				    ((life*)point)->life+=0.2;
				    p->life-=9999;
				}
				    break;
				case(enum_id_fly_eat):
				case(enum_id_meat_eat):
				case(enum_id_grass_eat):
				{
				    unsigned long point=col&0xffffffffffff;
				    ((life*)point)->ener+=0.1;
				    p->life-=9999;
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
				coll10cm->insert(make_pair(this_space,((unsigned long)p->model<<48)+(unsigned long)p));
			  }
		    }
		}
		p=p->next;
	  }
    }

    //shoot
    set<shoot*>::iterator iter;
    for (iter = shoots.begin(); iter != shoots.end();)
    {
	  bool tryget=true;
	  set<gra_client*>::iterator gciter;
	  for (gciter = my_net.gra_clients.begin(); gciter != my_net.gra_clients.end(); gciter++)
	  {
		if((*gciter)->id_socket==(*iter)->from)
		{
		    break;
		}
	  }
	  if((*iter)->boom)
	  {
		(*iter)->bom_r+=0.1;
		if((*iter)->bom_r>1)
		{
		    switch((*iter)->kind)
		    {
		    case(enum_shoot_add_tree):
		    {
			  float_virw_position tem;
			  tem.x=(*iter)->my_attack.x;
			  tem.y=(*iter)->my_attack.y;
			  tem.z=(*iter)->my_attack.z;
			  w_trees.add(tem,1.0);
			  break;
		    }
		    case(enum_shoot_add_grass_eat):
		    {
			  float_virw_position tem;
			  tem.x=(*iter)->my_attack.x;
			  tem.y=(*iter)->my_attack.y;
			  tem.z=(*iter)->my_attack.z;
			  w_grass_eats.add(tem,1.0);
			  break;
		    }
		    case(enum_shoot_add_meat_eat):
		    {
			  float_virw_position tem;
			  tem.x=(*iter)->my_attack.x;
			  tem.y=(*iter)->my_attack.y;
			  tem.z=(*iter)->my_attack.z;
			  w_meat_eats.add(tem,1.0);
			  break;
		    }
		    case(enum_shoot_add_fly_eat):
		    {
			  float_virw_position tem;
			  tem.x=(*iter)->my_attack.x;
			  tem.y=(*iter)->my_attack.y;
			  tem.z=(*iter)->my_attack.z;
			  w_fly_eats.add(tem,1.0);
			  break;
		    }
		    default:
			  break;
		    }
		    delete *iter;
		    iter=shoots.erase(iter);
		    continue;
		}
		iter++;
		continue;
	  }
	  if((*iter)->kind==enum_shoot_get&&(*gciter)->attach!=NULL)
	  {
		tryget=false;
		switch((*gciter)->attach_kind)
		{
		case(enum_kind_grain):
		    ((grain*)((*gciter)->attach))->attach=false;
		    break;
		case(enum_kind_life):
		    ((life*)((*gciter)->attach))->attach=false;
		    break;
		case(enum_kind_mulin):
		    mulin_attach=false;
		    attach_check=false;
		    break;
		default:
		    break;
		}
		(*gciter)->attach=NULL;
		(*iter)->boom=true;
		iter++;
		continue;
	  }
	  (*iter)->live+=0.02;
	  (*iter)->my_attack.b+=(*iter)->my_attack_glimmer.b;
	  double sqrt_tem=sqrt((*iter)->live);
	  double speed=10*sqrt_tem;
	  int dif=speed/0.01;
	  double step=10.0/dif;
	  for(int i=0; i<dif; ++i)
	  {
		(*iter)->my_attack.x+=step*sqrt_tem*(*iter)->my_attack_glimmer.x;
		(*iter)->my_attack.y+=step*sqrt_tem*(*iter)->my_attack_glimmer.y;
		(*iter)->my_attack.z+=step*sqrt_tem*(*iter)->my_attack_glimmer.z;

		if((*iter)->my_attack.y <= collision.map_height((*iter)->my_attack.x, (*iter)->my_attack.z)-0.12 || (*iter)->my_attack.x >= 199|| (*iter)->my_attack.x <= 1 || (*iter)->my_attack.z >= 199|| (*iter)->my_attack.z <= 1 || (*iter)->my_attack.y >= 100)
		{
		    (*iter)->boom=true;
		    break;
		}
		this_space=(((long)((*iter)->my_attack.x*0.5))<<40)+(((long)((*iter)->my_attack.y))<<20)+(long)((*iter)->my_attack.z*0.5);
		if(coll2m->count(this_space))
		{
		    (*iter)->boom=true;
		    break;
		}
		this_space=(((long)((*iter)->my_attack.x*2))<<40)+(((long)((*iter)->my_attack.y*4))<<20)+(long)((*iter)->my_attack.z*2);
		if(coll50cm->count(this_space))
		{
		    (*iter)->boom=true;
		    ite=coll50cm->find(this_space);
		    break;
		}
		this_space=(((long)((*iter)->my_attack.x*10))<<40)+(((long)((*iter)->my_attack.y*10))<<20)+(long)((*iter)->my_attack.z*10);
		if(coll10cm->count(this_space))
		{
		    (*iter)->boom=true;
		    ite=coll10cm->find(this_space);
		    unsigned long col=ite->second;
		    int kind=col>>48;
		    switch(kind)
		    {
		    case(enum_id_maple):
		    case(enum_id_hail):
		    case(enum_id_snow):
		    case(enum_id_gear):
		    case(enum_id_petal):
		    {
			  unsigned long point=col&0xffffffffffff;
			  if((*iter)->kind==enum_shoot_destroy)
				((grain*)point)->life-=9999;
			  else if((*iter)->kind==enum_shoot_get)
			  {
				if(tryget)

				{
				    (*gciter)->attach=(void*)point;
				    (*gciter)->attach_kind=enum_kind_grain;
				    ((grain*)point)->attach=true;
				}
			  }
			  else if((*iter)->kind==enum_shoot_grow)
				((grain*)point)->life+=99;
		    }
			  break;
		    case(enum_id_fly_eat):
		    case(enum_id_grass_eat):
		    case(enum_id_meat_eat):
		    case(enum_id_tree):
		    {
			  unsigned long point=col&0xffffffffffff;
			  if((*iter)->kind==enum_shoot_destroy)
			  {
				((life*)point)->life-=9999;
			  }
			  else if((*iter)->kind==enum_shoot_get)
			  {
				if(tryget)
				{
				    (*gciter)->attach=(void*)point;
				    (*gciter)->attach_kind=enum_kind_life;
				    ((life*)point)->attach=true;
				}
			  }
			  else if((*iter)->kind==enum_shoot_grow)
				((life*)point)->life+=1.0;
		    }
			  break;
		    case(enum_id_mulin):
		    {
			  //if((*gciter)->login_state==enum_login_shtb)
			  {
				if((*iter)->kind==enum_shoot_destroy)
				{
				    mulin_health-=0.1;
				    mulin_energy-=0.1;
				}
				else if((*iter)->kind==enum_shoot_get)
				{
				    if(tryget)
				    {
					  (*gciter)->attach=(void*)0xffffffffffffffff;
					  (*gciter)->attach_kind=enum_kind_mulin;
					  attach_check=false;
					  mulin_attach=true;
					  string nos;
					  my_net.send_to("chk_gt",nos,(*gciter)->id_socket);
				    }
				}
				else if((*iter)->kind==enum_shoot_grow)
				{
				    mulin_health+=0.1;
				    mulin_energy+=0.1;
				}
			  }
		    }
			  break;
		    default:
			  break;
		    }
		    break;
		}
	  }
	  iter++;
    }
}

void world::physic_move()
{
    if(!mulin_attach)
    {
	  mulin_pos.x+=mulin_vel.x;
	  mulin_pos.y+=mulin_vel.y;
	  mulin_pos.z+=mulin_vel.z;
	  mulin_pos.b+=mulin_vel.b;
    }

    {
	  grain*p=grains.grains.next;
	  while(p!=NULL)
	  {
		p->position.x+=p->velocity.x;
		p->position.y+=p->velocity.y;
		p->position.z+=p->velocity.z;
		p->position.a+=p->velocity.a;
		p->position.b+=p->velocity.b;
		p->position.c+=p->velocity.c;
		p=p->next;
	  }
    }

    w_fly_eats.clean_and_map();
    w_meat_eats.clean_and_map();
    w_grass_eats.clean_and_map();
    w_trees.clean_and_map();

    {
	  w_fly_eats.move();
	  w_meat_eats.move();
	  w_grass_eats.move();
	  w_trees.move();
    }
}

world::~world()
{
    //dtor
}
