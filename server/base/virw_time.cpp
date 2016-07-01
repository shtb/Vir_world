#include"virw_time.h"
#include"../base/interfaces.h"

void virw_time::time_go()
{
	++small_t;
	if (small_t >= 259200)
	{
		small_t = 0;
		++big_t;
	}
	double tem=(small_t-129660.0)/129600.0;

	my_world.fog_density=0.202-0.2*sqrt(1.001-(tem*tem));
	my_world.weather.float_temperature=10*sin((small_t-64800)*0.000024240684);
}
