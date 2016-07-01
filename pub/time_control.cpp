#include"../../pub/time_control.h"
#include<iomanip>

time_control::time_control()
{
    timeval now;
    gettimeofday(&now,0);
    last=now.tv_sec*1000000L+now.tv_usec;
    start_time=last;
}

void time_control::restart_timer()
{
	timeval now;
	gettimeofday(&now,0);
	start_time=now.tv_sec*1000000L+now.tv_usec;
}

string time_control::get_used_time_string()
{
	timeval now;
	gettimeofday(&now,0);
	long used=now.tv_sec*1000000L+now.tv_usec-start_time;
	stringstream ss;
	ss<<setiosflags(ios::fixed)<<setprecision(2)<<used/1000000.0<<"s";
	return ss.str();
}

long time_control::get_used_time_long()
{
	timeval now;
	gettimeofday(&now,0);
	return now.tv_sec*1000000L+now.tv_usec-start_time;
}

long time_control::get_now_time_long()
{
    timeval now;
    gettimeofday(&now,0);
    return now.tv_sec*1000000L+now.tv_usec;
}

void time_control::wait_for(int usectime)
{
    timeval now;
    target=last+usectime;
    usectime-=210;
    while(usectime>50)
    {
	  if(usectime>100)
		usleep(usectime);
	  else
		usleep(50);
	  gettimeofday(&now,0);
	  last=now.tv_sec*1000000L+now.tv_usec;
	  usectime=target-last;
    }
}
