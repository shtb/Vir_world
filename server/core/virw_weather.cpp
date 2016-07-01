#include "virw_weather.h"
#include"../base/world_structs.h"
#include"../base/interfaces.h"

virw_weather::virw_weather()
{
	//ctor
}

double virw_weather::get_life_content(double x,double y)
{
	int a=(x+100)/5;
	int b=(y+100)/5;
	return life_profile[b*40+a];
}

void virw_weather::time_move()
{
	double rad=0.002-(rand()%40000)/10000000.0;
	virw_wind[0]+=rad;
	rad=0.002-(rand()%40000)/10000000.0;
	virw_wind[1]+=rad;

	int raa=rand()%1000000;
	if(raa<600)
	{
		int rnum=rand()%1200;

		virw_position pos;
		pos.x=95.0-((rand()%1900000)/10000.0);
		pos.y=40+((rand()%100000)/10000.0);
		pos.z=95.0-((rand()%1900000)/10000.0);
		float co[4]= {-2};

		int choose=-1;
		int temper=average_temperature+float_temperature;
		int proba_hail=temper;
		if(proba_hail<0)
			proba_hail=0;
		else if(proba_hail>30)
			proba_hail=30;
		int proba_snow=10-temper;
		if(proba_snow<0)
			proba_snow=0;
		else if(proba_snow>30)
			proba_snow=30;
		double all=proba_hail+proba_snow+50.0;
		int ra=rand()%(int)all;
		double ite=ra/all;
		if(ra>50)
		{
			if(ite-50.0/all>proba_hail/all)
			{
				choose=enum_id_snow;
				co[0]=-2;
			}
			else
			{
				choose=enum_id_hail;
				co[0]=-2;
			}
		}
		else
		{
			if(ra<15)
			{
				choose=enum_id_maple;
				co[0]=-2;
			}
			else if(ra<25)
			{
				choose=enum_id_gear;
				co[0]=-2;
			}
			else if(ra<35)
			{
				choose=enum_id_maple;
				co[0]=0.1;
				co[1]=0.3;
				co[2]=0.03;
			}
			else
			{
				choose=enum_id_petal;
				co[0]=-2;
			}
		}
		my_world.grains.add(rnum,pos,choose,co);
	}

}

virw_weather::~virw_weather()
{
	//dtor
}
