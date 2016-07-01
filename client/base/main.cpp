#include "interfaces.h"
#include "../../pub/STDHF.h"
#include "daemon.h"
#include "../core/graphic.h"
#include "../../pub/inte_link.h"
#include "../core/Com.h"

using namespace std;

int thread_shell(thread_control* tc)
{
        tc->t_run();
        return 0;
}

int main(int argc, char** argv)
{
        srand(time(NULL));
        my_gra.argc = argc;
        my_gra.argv = argv;

        my_gra.thread_name = "Graphic";
        my_com.thread_name = "Communicate";
        my_net.thread_name = "Internet";
        thread thread_gra(thread_shell, &my_gra);
        thread thread_com(thread_shell, &my_com);
        thread thread_net(thread_shell, &my_net);
        class daemon my_daemon;
        my_daemon.run();
        thread_gra.join();
        thread_com.join();
        thread_net.join();
        return 0;
}
