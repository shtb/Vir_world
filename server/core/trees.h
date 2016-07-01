#ifndef TREES_H
#define TREES_H
#include<thread>
#include"../../pub/spinlock.h"
#include"../../pub/STDHF.h"
#include"../base/lives.h"
#include"../base/world_structs.h"

using namespace std;

class trees:public lives
{
public:
	trees();
	void move();
	~trees();
};

#endif // TREES_H
