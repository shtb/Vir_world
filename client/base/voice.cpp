#include "voice.h"
#include"interfaces.h"
#include<algorithm>

voice::voice()
{
	dev=alcOpenDevice(NULL);
	cc=alcCreateContext(dev,NULL);
	alcMakeContextCurrent(cc);
}

void al_daemon(voice*voi)
{
	ALuint buffer;
	ALuint source;
	short data[2205];
	ALsizei size=4410,freq=22050;
	ALenum format=4353;//4352-4355:mono8-stereo16;
	alGenSources(1,&source);
	bool playing_start=false;
	while(!good_bye)
	{
		static int cou=0;
		cou++;
		static double lf=0;
		double fre=voi->frequency/10.0;
		for(int i=0; i<2205; ++i)
		{
			lf+=6.2831853*fre/2205.0;
			data[i]=voi->amplitude*32000*(voi->am1*sin(lf)+voi->am2*sin(2*lf)+voi->am3*sin(3*lf)+voi->am4*sin(4*lf));
		}
		if(lf>700000)
		{
			lf=lf-628318.53;
		}

		ALint  queued = 2;
		alGetSourcei(source , AL_BUFFERS_QUEUED , &queued);
		int last_qu_t=0;
		while(queued>1)
		{
			last_qu_t++;
			ALuint buf;
			alSourceUnqueueBuffers(source ,1, &buf);
			alDeleteBuffers(1,&buf);
			alGetSourcei(source , AL_BUFFERS_QUEUED , &queued);
			if(last_qu_t>10)
			{
				playing_start=false;
				last_qu_t=0;
				break;
			}
			usleep(20000);
		}
		alGenBuffers(1,&buffer);
		alBufferData(buffer,format,data,size,freq);
		alSourceQueueBuffers(source,1,&buffer);
		if(!playing_start)
		{
			alSourcePlay(source);
			playing_start=true;
		}
		while(!voi->voice_on&&!good_bye)
		{
			usleep(100000);
		}
	}
	alDeleteSources(1,&source);
	alDeleteBuffers(1,&buffer);
}

void voice::start()
{
	thread AL_daemon(al_daemon,this);
	AL_daemon.detach();
}

voice::~voice()
{
	alcDestroyContext( cc );
	alcCloseDevice( dev );
}
