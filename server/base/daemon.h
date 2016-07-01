#ifndef DAEMON_H
#define DAEMON_H

#include<iostream>
#include<string>
#include"../base/interfaces.h"

class daemon
{
public:
	daemon();
	void init();
	void wake();
	void dormancy();
	void run();
	static void net_rec(std::string mes,int sock);
	virtual ~daemon();
protected:
private:
};

#endif // DAEMON_H
