#include "../../pub/thread_control.h"
#include"../base/interfaces.h"
#include <unistd.h>

using namespace std;

thread_control::thread_control()
{
    // ctor
}

int thread_control::t_run()
{
    while(!t_load_sy)
    {
	  usleep(1000);
    }
    t_load();
    my_print.message(thread_name + " : loaded!");
    t_loaded_sy = true;

    while(!t_start_sy)
    {
	  usleep(1000);
    }
    t_start();
    my_print.message(thread_name + " : started!\n");
    t_started_sy = true;

    while(!t_save_sy)
    {
	  t_loop();
	  ++t_run_times;
    }
    t_save();
    my_print.message(thread_name + " : saved!");
    t_saved_sy = true;

    while(!t_bye_sy)
    {
	  usleep(1000);
    }
    t_finish();
    my_print.message(thread_name + " : bye!\n");
    t_finished_sy = true;
    return 0;
}

int thread_control::t_load()
{
    return 0;
}

int thread_control::t_start()
{
    return 0;
}

int thread_control::t_loop()
{
    usleep(100000);
    return 0;
}

int thread_control::t_save()
{
    return 0;
}

int thread_control::t_finish()
{
    return 0;
}

thread_control::~thread_control()
{
    // dtor
}
