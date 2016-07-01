#pragma once
#include<sys/time.h>
#include"../../pub/STDHF.h"
//please use wait_for()once before you really use it.

using namespace std;

class time_control
{
public:
	long start_time,live_past_time=0;
	time_control();
	long get_now_time_long();
	void restart_timer();
	string get_used_time_string();
	long get_used_time_long();
	void wait_for(int usecwtime);
private:
	long last, target;
};
