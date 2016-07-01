#include<iostream>
#include"daemon.h"

using namespace std;

int thread_shell(thread_control*tc)
{
    tc->t_run();
    return 0;
}

int main()
{
    srand(time(NULL));
    my_net.thread_name="Internet";
    my_world.thread_name="World";
    thread thread_world(thread_shell,&my_world);
    thread thread_net(thread_shell,&my_net);
    class daemon my_daemon;
    my_daemon.run();
    thread_net.join();
    thread_world.join();
    return 0;
}
