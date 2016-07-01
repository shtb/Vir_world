#ifndef DAEMON_H
#define DAEMON_H

#include <iostream>
#include <string>
#include "../../pub/inte_link.h"
#include "../core/graphic.h"
#include "../core/Com.h"

class daemon
{
public:
        daemon();
        void init();
        void wake();
        void dormancy();
        void run();
        virtual ~daemon();

protected:
private:
};

#endif // DAEMON_H
