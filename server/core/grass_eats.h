#ifndef GRASS_EATS_H
#define GRASS_EATS_H
#include<thread>
#include"../../pub/spinlock.h"
#include"../../pub/STDHF.h"
#include"../base/lives.h"
#include"../base/world_structs.h"

using namespace std;

class grass_eats:public lives
{
public:
	grass_eats();
	void move();
	~grass_eats();
};

#endif // GRASS_EATS_H
