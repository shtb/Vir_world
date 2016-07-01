#include "states.h"
#include"interfaces.h"
#include<iomanip>
states::states()
{
}

void states::refresh_show()
{
        stringstream ss;
        ss<<"\\cfzfid:"<<myself.id_socket;
        if(myself.login_state==enum_login_shtb)
        {
                ss<<"(shtb)";
        }
        ss<<" \\caaz|\\cfzf ";
        ss<<"time:" << time_big << ":" << time_small / 10800 << "-" << (time_small % 10800) / 180 << "-" <<left<<setw(3)<< (time_small % 180) / 3;
        ss<<" \\caaz|\\cfzf ";
        ss<<"speed:"<<my_gra.speed*60;
        ss<<" \\caaz|\\cfzf ";
        ss<<setw(7);
        switch(my_gra.shoot_kind)
        {
        case(enum_shoot_cube):
                ss<<" cube ";
                break;
        case(enum_shoot_decube):
                ss<<"decube";
                break;
        case(enum_shoot_destroy):
                ss<<"destroy";
                break;
        case(enum_shoot_get):
                ss<<" get ";
                break;
        case(enum_shoot_grow):
                ss<<" grow ";
                break;
        case(enum_shoot_add_tree):
                ss<<" tree ";
                break;
        case(enum_shoot_add_grass_eat):
                ss<<"grass_eat";
                break;
        case(enum_shoot_add_meat_eat):
                ss<<"meat_eat";
                break;
        case(enum_shoot_add_fly_eat):
                ss<<"fly_eat";
                break;
        default:
                break;
        }
        ss<<" \\caaz|\\cfzf ";
        ss<<"cube_num:"<<my_gra.live_cube_num;
        ss<<" \\caaz|\\cfzf ";
        if(my_com.input_receiver==default_input)
        {
                ss<<"default";
        }
        else if(my_com.input_receiver==command_input)
        {
                ss<<"command";
        }
        else if(my_com.input_receiver==talk_input)
        {
                ss<<"talk";
        }
        else if(my_com.input_receiver==material_change_input)
        {
                ss<<"material";
        }
        else
        {
                ss<<"unknown";
        }
        if(my_com.using_method==sc_input)
        {
                ss<<"(sc)";
        }
        if(my_com.input_lock)
        {
                ss<<"(L)";
        }
        ss<<" \\caaz|\\cfzf ";
        ss<<"my_pos:"<<setprecision(4)<<setw(5)<<my_gra.my_pos.x<<" "<<setw(5)<<my_gra.my_pos.y<<" "<<setw(5)<<my_gra.my_pos.z<<" ";
        ss<<" \\caaz|\\cfzf ";

        state_show=ss.str();
}

states::~states()
{
}
