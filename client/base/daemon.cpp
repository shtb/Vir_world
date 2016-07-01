#include "daemon.h"
#include <iostream>
#include <unistd.h>
#include <thread>
#include "interfaces.h"
using namespace std;

daemon::daemon()
{
    // ctor
}

void daemon::wake()
{
    my_net.t_load_sy = true;
    while(!my_net.t_loaded_sy)
    {
        usleep(1000);
    }
    my_net.t_start_sy = true;
    while(!my_net.t_started_sy)
    {
        usleep(1000);
    }
    my_com.t_load_sy = true;
    while(!my_com.t_loaded_sy)
    {
        usleep(1000);
    }
    my_com.t_start_sy = true;
    while(!my_com.t_started_sy)
    {
        usleep(1000);
    }

    my_gra.t_load_sy = true;
    while(!my_gra.t_loaded_sy)
    {
        usleep(1000);
    }
    my_gra.t_start_sy = true;
    while(!my_gra.t_started_sy)
    {
        usleep(1000);
    }
    my_voice.start();
}
void parse_init(stringstream& ss)
{
    string choose;
    ss>>choose;
    if(choose=="world_server:")
    {
        ss>>my_net.ip_addr>>my_net.port;
    }
    else if(choose=="accum:")
    {
        ss>>my_gra.accum.on>>my_gra.accum.para;
        my_gra.accum.init=true;
    }
    else
    {
        my_print.error("unknown init option:"+choose);
    }
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
        string s;
        while(getline(config_file,s))
        {
            if(s[0]!='/'&&s.length()!=0)
            {
                stringstream ss(s);
                parse_init(ss);
            }
        }
    }
    config_file.close();
}

void daemon::run()
{
    init();
    wake();
    my_print.message("hi!");
    while(!good_bye)
    {
        usleep(1000);
    }
    dormancy();
    daemon_bye = true;
}

void daemon::dormancy()
{
    my_net.t_save_sy = true;
    while(!my_net.t_saved_sy)
    {
        usleep(1000);
    }
    my_net.t_bye_sy = true;
    while(!my_net.t_finished_sy)
    {
        usleep(1000);
    }

    my_com.t_save_sy = true;
    while(!my_com.t_saved_sy)
    {
        usleep(1000);
    }
    my_com.t_bye_sy = true;
    while(!my_com.t_finished_sy)
    {
        usleep(1000);
    }

    my_gra.t_save_sy = true;
    while(!my_gra.t_saved_sy)
    {
        usleep(1000);
    }
    my_gra.t_bye_sy = true;
    while(!my_gra.t_finished_sy)
    {
        usleep(1000);
    }

    my_print.message("bye!");
}

daemon::~daemon()
{
    // dtor
}
