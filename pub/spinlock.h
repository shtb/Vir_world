#ifndef SPIN_L
#define SPIN_L

#include <atomic>
#include "STDHF.h"

class spinlock
{
	volatile std::atomic_flag flag;

public:
	spinlock();
	void lock();
	void slock();
	void unlock();
};

#endif
