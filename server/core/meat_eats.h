#ifndef MEAT_EATS_H
#define MEAT_EATS_H
#include<thread>
#include"../../pub/spinlock.h"
#include"../../pub/STDHF.h"
#include"../base/lives.h"
#include"../base/world_structs.h"

using namespace std;

class meat_eats:public lives
{
public:
	meat_eats();
	void move();
	~meat_eats();
};

#endif // MEAT_EATS_H
