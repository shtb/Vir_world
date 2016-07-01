#include"../../pub/STDHF.h"

using namespace std;

enum Mu_task_enum
{
    wind = 0,
    wink = 1,
    smile = 2,
    wind2 = 3,
    sad = 4,
    right_leg = 5,
    left_leg = 6,
    right_hand1 = 7,
    left_hand1 = 8,
    fight = 9,
    left_head = 10,
    right_head = 11,
    head_up = 12,
    head_down = 13,
    battle_state = 14,
    squat = 15
};
enum Mu_part_enum
{
    skirt=0, ear,   eyebrow, shawl, eye,    head,    leg,
    lash,  pupil, hair,    chest, armlet, necktie, shoe
};

enum login_state_enum
{
    enum_login_unknown,enum_login_shtb,enum_login_mulin
};

enum virtual_ID
{
    enum_id_mulin,
    enum_id_world_front,
    enum_id_world_behind,
    enum_id_world_below,
    enum_id_world_above,
    enum_id_world_left,
    enum_id_world_right,
    enum_id_trans_world,
    enum_id_elevator,
    enum_id_snow,
    enum_id_gear,
    enum_id_maple,
    enum_id_petal,
    enum_id_hail,
    enum_id_plat,
    enum_id_tree,
    enum_id_grass_eat,
    enum_id_meat_eat,
    enum_id_fly_eat,
    enum_id_book,
    enum_id_cubes,
    enum_id_client
};

enum kind_ID
{
    enum_kind_grain,enum_kind_life,enum_kind_mulin
};

struct virw_position
{
    double x=100,y=30,z=100,a=0,b=0,c=0;
};
struct virw_velocity
{
    double x=0,y=0,z=0,a=0,b=0,c=0;
};

struct float_virw_position
{
    float x=0,y=0,z=0,a=0,b=0,c=0;
};

struct float_virw_velocity
{
    float x=0,y=0,z=0,a=0,b=0,c=0;
};

struct gra_client
{
    unsigned char log_num=0;
    int login_state=enum_login_unknown;
    int id_socket;
    float color[4]= {0.1,0.1,0.1,1.0};
    virw_position pos;
    virw_position eye_look;
    int attach_kind=-1;
    void *attach=NULL;
};

enum shoot_kind {enum_shoot_cube,enum_shoot_decube,enum_shoot_destroy,enum_shoot_get,enum_shoot_grow,enum_shoot_add_tree,enum_shoot_add_grass_eat,enum_shoot_add_meat_eat,enum_shoot_add_fly_eat,enum_shoot_max};

struct shoot
{
    virw_position my_attack;
    virw_position my_attack_glimmer;
    bool boom=false;
    unsigned char kind=enum_shoot_destroy;
    double bom_r=0;
    double live=0;
    int from;
};
