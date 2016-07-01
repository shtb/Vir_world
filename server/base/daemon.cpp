#include "daemon.h"

#include<iostream>
#include<unistd.h>
#include<thread>
#include"../base/interfaces.h"
using namespace std;

daemon::daemon()
{
        //ctor
}

void daemon::init()
{
        ifstream config_file(my_file.files[enum_file_config],ios::in);
        if(!config_file)
        {
                my_print.error("config.conf not found!");
        }
        else
        {
                config_file>>my_net.ip_addr>>my_net.port;
                config_file>>my_world.my_world_switch.this_world_order>>my_world.my_world_switch.using_world_num;
        }
        config_file.close();
}

void daemon::wake()
{
        my_world.t_load_sy=true;
        while(!my_world.t_loaded_sy)
        {
                usleep(1000);
        }

        my_net.t_load_sy=true;
        while(!my_net.t_loaded_sy)
        {
                usleep(1000);
        }
        my_net.t_start_sy=true;
        while(!my_net.t_started_sy)
        {
                usleep(1000);
        }

        my_world.t_start_sy=true;
        while(!my_world.t_started_sy)
        {
                usleep(1000);
        }
}

void daemon::net_rec(string message,int sock)
{
        stringstream ss;
        ss<<"received from:";
        ss<<sock<<endl;
        ss<<"said:";
        ss<<message<<endl;
        my_print.message(ss.str());
}

void daemon::run()
{
        init();
        wake();
        my_print.message("WAKED!");
        while(1)
        {
                usleep(1000);
        }
        dormancy();
}

void daemon::dormancy()
{
        my_net.t_save_sy=true;
        while(!my_net.t_saved_sy)
        {
                usleep(1000);
        }
        my_net.t_bye_sy=true;
        while(!my_net.t_finished_sy)
        {
                usleep(1000);
        }
}

daemon::~daemon()
{
        //dtor
}
