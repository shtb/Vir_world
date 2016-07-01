#include "gra_link.h"
#include"../base/interfaces.h"

gra_link::gra_link()
{
    work_mode=Inte_server;
    limit_link=10;
}

void gra_link::accept_one_link(int sock_id)
{
    gra_client*p=new gra_client;
    p->id_socket=sock_id;
    p->pos.x=-1;
    p->pos.y=-1;
    p->pos.z=-1;
    gra_clients.insert(p);

    stringstream ss;
    ss.write((char*)p,sizeof(gra_client));
    string ssstr=ss.str();
    send_to("pre_da",ssstr,sock_id);
    my_world.cubes_changed=1;
}

void gra_link::end_one_link(int sock_id)
{
    set<gra_client*>::iterator iter;
    for (iter = gra_clients.begin(); iter != gra_clients.end(); iter++)
    {
	  if((*iter)->id_socket==sock_id)
	  {
		if((*iter)->attach!=NULL)
		{
		    switch((*iter)->attach_kind)
		    {
		    case(enum_kind_grain):
			  ((grain*)((*iter)->attach))->attach=false;
			  break;
		    case(enum_kind_life):
			  ((life*)((*iter)->attach))->attach=false;
			  break;
		    default:
			  break;
		    }
		}
		delete *iter;
		gra_clients.erase(iter);
		break;
	  }
    }
}

void gra_link::send_to(string head,string& my_message, int socket)
{
    send_lock.lock();
    if(head.length()!=6)
    {
	  my_print.error("send message head length is wrong!"+head);
	  return;
    }
    stringstream mess_head;
    mess_head.write(head.c_str(),6);
    int mess_len=my_message.length();
    mess_head.write((char*)&mess_len,sizeof(int));
    string send_message=mess_head.str()+my_message;
    const char*send_str=send_message.data();
    int send_str_len=send_message.length();
    if(socket==broadcast_socket)
    {
	  set<gra_client*>::iterator net_iter;
	  for (net_iter = gra_clients.begin(); net_iter != gra_clients.end(); net_iter++)
	  {
		net_send((*net_iter)->id_socket, send_str,send_str_len);
	  }
    }
    else if(socket==mulin_socket)
    {
	  set<gra_client*>::iterator net_iter;
	  for (net_iter = gra_clients.begin(); net_iter != gra_clients.end(); net_iter++)
	  {
		if((*net_iter)->login_state==enum_login_mulin)
		    net_send((*net_iter)->id_socket, send_str,send_str_len);
	  }
    }
    else if(socket==shtb_and_unknown_socket)
    {
	  set<gra_client*>::iterator net_iter;
	  for (net_iter = gra_clients.begin(); net_iter != gra_clients.end(); net_iter++)
	  {
		if((*net_iter)->login_state==enum_login_shtb||(*net_iter)->login_state==enum_login_unknown)
		{
		    net_send((*net_iter)->id_socket, send_str,send_str_len);
		}
	  }
    }
    else if(socket==shtb_socket)
    {
	  set<gra_client*>::iterator net_iter;
	  for (net_iter = gra_clients.begin(); net_iter != gra_clients.end(); net_iter++)
	  {
		if((*net_iter)->login_state==enum_login_shtb)
		    net_send((*net_iter)->id_socket, send_str,send_str_len);
	  }
    }
    else if((unsigned)socket<0xfffffff0)
    {
	  net_send(socket, send_str,send_str_len);
    }
    send_lock.unlock();
}

void gra_link::send_to(string head,stringstream& my_message, int socket)
{
    send_lock.lock();
    if(head.length()!=6)
    {
	  my_print.error("send message head length is wrong!"+head);
	  return;
    }
    stringstream mess_head;
    mess_head.write(head.c_str(),6);
    int mess_len=my_message.str().length();
    mess_head.write((char*)&mess_len,sizeof(int));
    string send_message=mess_head.str()+my_message.str();
    const char*send_str=send_message.data();
    int send_str_len=send_message.length();
    if(socket==broadcast_socket)
    {
	  set<gra_client*>::iterator net_iter;
	  for (net_iter = gra_clients.begin(); net_iter != gra_clients.end(); net_iter++)
	  {
		net_send((*net_iter)->id_socket, send_str,send_str_len);
	  }
    }
    else if(socket==mulin_socket)
    {
	  set<gra_client*>::iterator net_iter;
	  for (net_iter = gra_clients.begin(); net_iter != gra_clients.end(); net_iter++)
	  {
		if((*net_iter)->login_state==enum_login_mulin)
		    net_send((*net_iter)->id_socket, send_str,send_str_len);
	  }
    }
    else if(socket==shtb_and_unknown_socket)
    {
	  set<gra_client*>::iterator net_iter;
	  for (net_iter = gra_clients.begin(); net_iter != gra_clients.end(); net_iter++)
	  {
		if((*net_iter)->login_state==enum_login_shtb||(*net_iter)->login_state==enum_login_unknown)
		{
		    net_send((*net_iter)->id_socket, send_str,send_str_len);
		}
	  }
    }
    else if(socket==shtb_socket)
    {
	  set<gra_client*>::iterator net_iter;
	  for (net_iter = gra_clients.begin(); net_iter != gra_clients.end(); net_iter++)
	  {
		if((*net_iter)->login_state==enum_login_shtb)
		    net_send((*net_iter)->id_socket, send_str,send_str_len);
	  }
    }
    else if((unsigned)socket<0xfffffff0)
    {
	  net_send(socket, send_str,send_str_len);
    }
    send_lock.unlock();
}

void gra_link::parse(string &choose, stringstream& ss,int connect)
{
    if(choose == "get_ma")
    {
	  stringstream ssa;
	  int len=my_file.files[enum_file_world].length();
	  ssa.write((char*)&len,sizeof(int));
	  ssa.write((char*)my_file.files[enum_file_world].data(),len);
	  ssa.write((char*)(my_world.map_height),sizeof(double)*41*41);
	  string ssstr=ssa.str();
	  my_net.send_to("map_he",ssstr,connect);
    }
    else if(choose=="mulinc")
    {
    }
    else if(choose=="bmp123")
    {
	  string ssstr=ss.str();
	  send_to("bmp123",ssstr,shtb_and_unknown_socket);
    }
    else if(choose=="login1")
    {
	  set<gra_client*>::iterator iter;
	  for (iter = gra_clients.begin(); iter != gra_clients.end(); iter++)
	  {
		if((*iter)->id_socket==connect)
		{
		    break;
		}
	  }
	  if(ss.str()=="1234")
	  {
		my_print.message("shtb login!");
		(*iter)->login_state=enum_login_shtb;
		stringstream ss;
		ss.write((char*)*iter,sizeof(gra_client));
		string ssstr=ss.str();
		send_to("pre_da",ssstr,connect);
	  }
	  else if(ss.str()=="5678")
	  {
		my_print.message("mulin login!");
		(*iter)->login_state=enum_login_mulin;
	  }
	  else
	  {
		my_print.warning("illegal login!");
		cout<<ss.str().length()<<" "<<ss.str()<<endl;
		(*iter)->log_num++;
		if((*iter)->log_num>2)
		{
		    shutdown((*iter)->id_socket,SHUT_RDWR);
		}
	  }
    }
    else if(choose=="pos_re")
    {
	  set<gra_client*>::iterator iter;
	  for (iter = gra_clients.begin(); iter != gra_clients.end(); iter++)
	  {
		if((*iter)->id_socket==connect)
		{
		    break;
		}
	  }
	  ss.read((char*)&((*iter)->pos),sizeof(virw_position));
	  ss.read((char*)&((*iter)->eye_look),sizeof(virw_position));
	  if((*iter)->eye_look.x>199)
		(*iter)->eye_look.x=199;
	  if((*iter)->eye_look.z>199)
		(*iter)->eye_look.z=199;
	  if((*iter)->eye_look.y>99)
		(*iter)->eye_look.y=99;
	  if((*iter)->eye_look.x<0)
		(*iter)->eye_look.x=0;
	  if((*iter)->eye_look.z<0)
		(*iter)->eye_look.z=0;
	  if((*iter)->eye_look.y<0)
		(*iter)->eye_look.y=0;
    }
    else if(choose=="comman")
    {
	  char ch_com[7];
	  ch_com[6]='\0';
	  ss.read(ch_com,6);
	  string command=ch_com;
	  if(command=="grain1")
	  {
		float co[3];
		virw_position pos;
		int model,num;
		ss.read((char*)&num,sizeof(int));
		ss.read((char*)&pos,sizeof(virw_position));
		ss.read((char*)&model,sizeof(int));
		ss.read((char*)&co,sizeof(float)*3);
		my_world.grains.add(num,pos,model,co);
	  }
	  else if(command=="client")
	  {
		set<gra_client*>::iterator iter;
		for (iter = gra_clients.begin(); iter != gra_clients.end(); iter++)
		{
		    if((*iter)->id_socket==connect)
		    {
			  break;
		    }
		}
		ss.read((char*)(*iter)->color,sizeof(float)*4);
	  }
	  else if(command=="cw1234")
	  {
		ss.read((char*)my_world.MuLin_show,sizeof(bool)*20);
	  }
	  else
	  {
		my_print.error("unknown command:"+command);
	  }
    }
    else if(choose=="shoot1")
    {
	  virw_position my_pos;
	  virw_position* look_at=new virw_position;
	  ss.read((char*)&my_pos,sizeof(virw_position));
	  ss.read((char*)look_at,sizeof(virw_position));
	  shoot *p=new shoot;
	  ss.read((char*)&(p->kind),sizeof(unsigned char));
	  if(p->kind==enum_shoot_cube||p->kind==enum_shoot_decube)
	  {
		float color[4];
		color[0]=my_pos.x;
		color[1]=my_pos.y;
		color[2]=my_pos.z;
		color[3]=my_pos.a;
		my_world.w_cubes_lock.slock();
		my_world.w_cubes.add(*look_at,color);
		my_world.w_cubes_lock.unlock();
		delete look_at;
		delete p;
	  }
	  else
	  {
		p->my_attack=my_pos;
		double lengt=sqrt(pow(look_at->x-my_pos.x,2)+pow(look_at->y-my_pos.y,2)+pow(look_at->z-my_pos.z,2));
		p->my_attack_glimmer.x=(look_at->x-my_pos.x)/lengt;
		p->my_attack_glimmer.y=(look_at->y-my_pos.y)/lengt;
		p->my_attack_glimmer.z=(look_at->z-my_pos.z)/lengt;
		p->my_attack_glimmer.b=20;
		p->from=connect;
		p->my_attack.x+=p->my_attack_glimmer.x;
		p->my_attack.y+=p->my_attack_glimmer.y;
		p->my_attack.z+=p->my_attack_glimmer.z;
		delete look_at;
		my_world.shoots.insert(p);
	  }
    }
    else if(choose=="chk_rg")
    {
	  my_world.attach_check=true;
    }
    else if(choose=="cub_co")
    {
	  char ca;
	  ss>>ca;
	  my_world.w_cubes_lock.slock();
	  switch(ca)
	  {
	  case('m'):
	  {
		virw_position op1,op2,pos1,pos2,posd;
		ss.read((char*)&pos1,sizeof(virw_position));
		ss.read((char*)&pos2,sizeof(virw_position));
		ss.read((char*)&posd,sizeof(virw_position));
		op1.x=min(pos1.x,pos2.x);
		op1.y=min(pos1.y,pos2.y);
		op1.z=min(pos1.z,pos2.z);
		op2.x=max(pos1.x,pos2.x);
		op2.y=max(pos1.y,pos2.y);
		op2.z=max(pos1.z,pos2.z);
		long sp2,spd;
		sp2=(((long)(pos2.x*10))<<40)+(((long)(pos2.y*10))<<20)+(long)(pos2.z*10);
		spd=(((long)(posd.x*10))<<40)+(((long)(posd.y*10))<<20)+(long)(posd.z*10);
		spd-=sp2;
		cube*p=my_world.w_cubes.the_head.next;
		while(p!=NULL)
		{
		    long tea=(p->position/1000000000000);
		    long teb=(p->position%1000000);
		    double px=tea/10.0;
		    double pz=teb/10.0;
		    double py=(p->position-teb-(tea*1000000000000))/10000000.0;

		    if(px>=op1.x&&px<=op2.x&&py>=op1.y&&py<=op2.y&&pz>=op1.z&&pz<=op2.z)
		    {
			  p->position+=spd;
		    }
		    p=p->next;
		}
	  }
		break;
	  case('c'):
	  {
		virw_position op1,op2,pos1,pos2,posd;
		ss.read((char*)&pos1,sizeof(virw_position));
		ss.read((char*)&pos2,sizeof(virw_position));
		ss.read((char*)&posd,sizeof(virw_position));
		op1.x=min(pos1.x,pos2.x);
		op1.y=min(pos1.y,pos2.y);
		op1.z=min(pos1.z,pos2.z);
		op2.x=max(pos1.x,pos2.x);
		op2.y=max(pos1.y,pos2.y);
		op2.z=max(pos1.z,pos2.z);
		long sp2,spd;
		sp2=(((long)(pos2.x*10))<<40)+(((long)(pos2.y*10))<<20)+(long)(pos2.z*10);
		spd=(((long)(posd.x*10))<<40)+(((long)(posd.y*10))<<20)+(long)(posd.z*10);
		spd-=sp2;
		cube*p=my_world.w_cubes.the_head.next;
		while(p!=NULL)
		{
		    double px=((p->position&0xfffff0000000000ul)>>40)/10.0;
		    double py=((p->position&0x00000fffff00000ul)>>20)/10.0;
		    double pz=(p->position&0x0000000000ffffful)/10.0;
		    if(px>=op1.x&&px<=op2.x&&py>=op1.y&&py<=op2.y&&pz>=op1.z&&pz<=op2.z)
		    {
			  my_world.w_cubes.add(p->position+spd,p->color);
		    }
		    p=p->next;
		}
	  }
		break;
	  case('d'):
	  {
		virw_position op1,op2,pos1,pos2;
		ss.read((char*)&pos1,sizeof(virw_position));
		ss.read((char*)&pos2,sizeof(virw_position));
		op1.x=min(pos1.x,pos2.x);
		op1.y=min(pos1.y,pos2.y);
		op1.z=min(pos1.z,pos2.z);
		op2.x=max(pos1.x,pos2.x);
		op2.y=max(pos1.y,pos2.y);
		op2.z=max(pos1.z,pos2.z);
		cube*p=my_world.w_cubes.the_head.next;
		while(p!=NULL)
		{
		    double px=((p->position&0xfffff0000000000ul)>>40)/10.0;
		    double py=((p->position&0x00000fffff00000ul)>>20)/10.0;
		    double pz=(p->position&0x0000000000ffffful)/10.0;
		    if(px>=op1.x&&px<=op2.x&&py>=op1.y&&py<=op2.y&&pz>=op1.z&&pz<=op2.z)
		    {
			  p=my_world.w_cubes.destroy(p);
		    }
		    else
			  p=p->next;
		}
	  }
		break;
	  case('r'):
	  {
		float co[4];
		ss.read((char*)co,sizeof(float)*4);
		virw_position op1,op2,pos1,pos2;
		ss.read((char*)&pos1,sizeof(virw_position));
		ss.read((char*)&pos2,sizeof(virw_position));
		op1.x=min(pos1.x,pos2.x);
		op1.y=min(pos1.y,pos2.y);
		op1.z=min(pos1.z,pos2.z);
		op2.x=max(pos1.x,pos2.x);
		op2.y=max(pos1.y,pos2.y);
		op2.z=max(pos1.z,pos2.z);
		cube*p=my_world.w_cubes.the_head.next;
		while(p!=NULL)
		{
		    double px=((p->position&0xfffff0000000000ul)>>40)/10.0;
		    double py=((p->position&0x00000fffff00000ul)>>20)/10.0;
		    double pz=(p->position&0x0000000000ffffful)/10.0;
		    if(px>=op1.x&&px<=op2.x&&py>=op1.y&&py<=op2.y&&pz>=op1.z&&pz<=op2.z)
		    {
			  p->color[0]=co[0];
			  p->color[1]=co[1];
			  p->color[2]=co[2];
			  p->color[3]=co[3];
		    }
		    p=p->next;
		}
	  }
		break;
	  default:
		break;
	  }
	  my_world.cubes_changed=2;
	  my_world.w_cubes_lock.unlock();
    }
    else
    {
	  set<gra_client*>::iterator iter;
	  for (iter = gra_clients.begin(); iter != gra_clients.end(); iter++)
	  {
		if((*iter)->id_socket==connect)
		{
		    break;
		}
	  }
	  if((*iter)->login_state==enum_login_shtb)
	  {
		if(choose=="cycle_")
		{
		    ss.read((char*)&my_world.cycle_time,sizeof(int));
		}
		else if(choose=="st_tlk")
		{
			send_to("st_tlk",ss,mulin_socket);
		}
		else
		{
		    cout<<"unknown message:"<<choose<<" //from root:"<<connect<<endl;
		}
	  }
	  else if((*iter)->login_state==enum_login_mulin)
	  {
		if(choose=="ml_dis")
		{
		    ss.read((char*)&my_world.mulin_distortion,sizeof(distortion));

		    for(int i = 0; i < my_world.MuLin_task_num; ++i)
		    {
			  ss.read((char*)&my_world.MuLin_task[i].play_on, sizeof(bool));
			  ss.read((char*)&my_world.MuLin_task[i].state, sizeof(double));
		    }

		    for(int i=0; i<my_world.MuLin_part_num; ++i)
		    {
			  ss.read((char*)&my_world.MuLin_show[i],sizeof(bool));
		    }
		}
		else if(choose=="ml_tlk")
		{
			send_to("ml_tlk",ss,shtb_socket);
		}
		else
		{
		    cout<<"unknown message:"<<choose<<" //from mulin:"<<connect<<endl;
		}
	  }
	  else
	  {
		cout<<"visitor link send message:"<<choose<<" //from "<<connect<<endl;
	  }
    }
}

gra_link::~gra_link()
{
}
