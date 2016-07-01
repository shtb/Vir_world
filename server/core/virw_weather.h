#ifndef VIRW_WEATHER_H
#define VIRW_WEATHER_H
#include"../../pub/STDHF.h"

using namespace std;

class virw_weather
{
	public:
		double life_profile[40*40];
		double temperature_profile[40*40];
		double average_temperature=0;
		double float_temperature;
		double virw_wind[2]={};
		virw_weather();
		double get_life_content(double x,double y);
		void time_move();
		virtual ~virw_weather();
	protected:
	private:
};

#endif // VIRW_WEATHER_H
