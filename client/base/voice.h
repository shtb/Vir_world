#ifndef VOICE_H
#define VOICE_H

#include </usr/include/AL/al.h>
#include </usr/include/AL/alc.h>
#include <iostream>
#include<thread>
#include<unistd.h>

using namespace std;

class voice
{
public:
	bool voice_on=false;
	double amplitude=1;
	double frequency=350;
	double am1=0.6;
	double am2=0.2;
	double am3=0.1;
	double am4=0.1;

	voice();
	~voice();
	void start();

	ALCcontext* cc;
	ALCdevice* dev;
};

#endif // VOICE_H
