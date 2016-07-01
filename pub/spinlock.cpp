#include "spinlock.h"

spinlock::spinlock()
{
	flag.clear(std::memory_order_release);
}

void spinlock::lock()
{
	while(flag.test_and_set(std::memory_order_acquire))
		usleep(100);
}

void spinlock::slock()
{
	int try_num = 0;
	while(flag.test_and_set(std::memory_order_acquire))
	{
		++try_num;
		if(try_num > 1000)
		{
            usleep(100);
			try_num = 0;
		}
	}
}

void spinlock::unlock()
{
	flag.clear(std::memory_order_release);
}
