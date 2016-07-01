#ifndef WORLD_STRCTS_H_INCLUDED
#define WORLD_STRCTS_H_INCLUDED

#include"../../pub/STDHF.h"
#include"../../pub/world_pub_structs.h"

using namespace std;

struct distortion
{
        virw_position excursion;
        double body_happy_value=0;
        double body_head_angle[2];
        double head_eye_angle[2];
        bool task_walk = false;
        bool task_fight = false;
        bool task_left_hand_1 = false;
        bool task_right_hand_1 = false;
        bool battle_state = false;
        bool squat_state = false;
};


#endif // WORLD_STRCTS_H_INCLUDED
