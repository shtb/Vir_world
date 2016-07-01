#pragma once

#include<thread>
#include"../../pub/spinlock.h"
#include"../../pub/STDHF.h"
#include"../base/lives.h"
#include"../base/world_structs.h"

using namespace std;

class fly_eats:public lives
{
public:
	fly_eats();
	void move();
};
