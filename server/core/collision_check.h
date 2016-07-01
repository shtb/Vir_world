#pragma once
#include"../../pub/STDHF.h"
#include<map>
#include<set>
#include"../base/coll_model.h"
#include"../base/lives.h"
using namespace std;

class collision_model
{
public:
	set<long>col_model;
	int id;
	virw_position pos;
	long get_long(double,double,double,int);
	void init(life*obj,int ide);
};

struct collision_reflect
{
	double x,y,z;
};

class collision_check
{
public:
	bool model_loaded=false;
	map<long,long>coll2m;
	map<long,long>coll50cm;
	map<long,long>coll10cm;
      map<long,long>const_coll10cm;
      bool const_changed=true;
	collision_reflect reflect[3200];
	void clean();
	void init();
	void load_models();
	void mixture(int task_or, int task_part_or);
	double map_height(double x,double z);
	coll_model MuLin_model_const, MuLin_model_display;
	coll_model fly_eat_model,grass_eat_model,meat_eat_model,tree_model;
	collision_reflect map_reflect(double x,double z);
	void add_mulin();
	long add(life*p,virtual_ID id);
	const int plat_50cm_num=291;
	const int plat_2m_num=60;
	const int elevator_50cm_num=16;
    long plat_50cm[291]= {197912217780421,197912217780422,197912217780423,197912217780424,197912217780425,197912217780426,199011729408192,199011729408193,199011729408194,199011729408195,199011729408196,199011729408197,199011729408198,199011729408199,199011729408200,199011729408201,199011729408202,199011729408203,199011729408204,199011729408205,199011729408206,199011729408207,199011729408208,200111241035966,200111241035967,200111241035968,200111241035969,200111241035970,200111241035971,200111241035972,200111241035973,200111241035974,200111241035975,200111241035976,200111241035977,200111241035978,200111241035979,200111241035980,200111241035981,200111241035982,200111241035983,200111241035984,200111241035985,201210752663741,201210752663742,201210752663743,201210752663744,201210752663745,201210752663746,201210752663747,201210752663748,201210752663749,201210752663750,201210752663751,201210752663752,201210752663753,201210752663754,201210752663755,201210752663756,201210752663757,201210752663758,201210752663759,201210752663760,201210752663761,201210752663762,202310264291540,203409775919290,203409775919291,203409775919316,203409775919317,204509287547065,204509287547066,204509287547067,204509287547092,204509287547093,204509287547094,205608799174841,205608799174842,205608799174843,205608799174868,205608799174869,205608799174870,205608799174871,207807822430391,207807822430424,207807822430425,208907334058166,208907334058167,208907334058200,208907334058201,210006845685942,210006845685943,210006845685976,210006845685977,211106357313717,211106357313718,211106357313719,211106357313752,211106357313753,212205868941493,212205868941494,212205868941495,212205868941528,212205868941529,212205868941530,213305380569269,213305380569270,213305380569271,213305380569304,213305380569305,213305380569306,214404892197045,214404892197046,214404892197047,214404892197080,214404892197081,214404892197082,214404892197083,215504403824821,215504403824822,215504403824823,215504403824836,215504403824837,215504403824838,215504403824839,215504403824840,215504403824841,215504403824842,215504403824843,216603915452596,216603915452597,216603915452598,216603915452599,216603915452612,216603915452613,216603915452614,216603915452615,216603915452616,216603915452617,216603915452618,216603915452619,217703427080372,217703427080373,217703427080374,217703427080375,217703427080388,217703427080389,217703427080393,217703427080394,217703427080395,218802938708148,218802938708149,218802938708150,218802938708151,218802938708164,218802938708165,218802938708170,218802938708171,219902450335924,219902450335925,219902450335926,219902450335927,219902450335940,219902450335941,219902450335946,219902450335947,221001961963700,221001961963701,221001961963702,221001961963703,221001961963716,221001961963717,221001961963718,221001961963721,221001961963722,221001961963723,222101473591476,222101473591477,222101473591478,222101473591479,222101473591492,222101473591493,222101473591494,222101473591495,222101473591496,222101473591497,222101473591498,222101473591499,223200985219253,223200985219254,223200985219255,223200985219268,223200985219269,223200985219270,223200985219271,223200985219272,223200985219273,223200985219274,223200985219275,224300496847029,224300496847030,224300496847031,224300496847064,224300496847065,224300496847066,224300496847067,225400008474805,225400008474806,225400008474807,225400008474840,225400008474841,225400008474842,226499520102581,226499520102582,226499520102583,226499520102616,226499520102617,226499520102618,227599031730357,227599031730358,227599031730359,227599031730392,227599031730393,228698543358133,228698543358134,228698543358135,228698543358168,228698543358169,229798054985910,229798054985911,229798054985944,229798054985945,230897566613687,230897566613720,231997078241496,233096589869240,233096589869241,233096589869242,233096589869243,233096589869268,233096589869269,233096589869270,233096589869271,234196101497017,234196101497018,234196101497019,234196101497044,234196101497045,234196101497046,235295613124794,235295613124795,235295613124820,235295613124821,236395124752571,236395124752596,237494636380349,237494636380350,237494636380351,237494636380352,237494636380353,237494636380354,237494636380355,237494636380364,237494636380365,237494636380366,237494636380367,237494636380368,237494636380369,237494636380370,237494636380371,238594148008125,238594148008126,238594148008127,238594148008128,238594148008129,238594148008130,238594148008131,238594148008140,238594148008141,238594148008142,238594148008143,238594148008144,238594148008145,239693659635905,239693659635906,239693659635907,239693659635916,239693659635917,239693659635918,240793171263683};
    long plat_2m[60]= {50577565286447,50577565286448,50577565286449,50577565286450,50577565286451,50577565286452,51677076914222,51677076914223,51677076914224,51677076914225,51677076914226,51677076914227,51677076914228,51677076914229,52776588541998,52776588541999,52776588542000,52776588542001,52776588542002,52776588542003,52776588542004,52776588542005,53876100169774,53876100169775,53876100169776,53876100169779,53876100169780,53876100169781,53876100169782,54975611797550,54975611797551,54975611797552,54975611797555,54975611797556,54975611797557,54975611797558,56075123425326,56075123425327,56075123425328,56075123425329,56075123425330,56075123425331,56075123425332,56075123425333,57174635053102,57174635053103,57174635053104,57174635053105,57174635053106,57174635053107,57174635053108,57174635053109,58274146680879,58274146680880,58274146680881,58274146680882,58274146680883,58274146680884,59373658308657,59373658308658};
    long elevator_50cm[16]= {217703427080390,217703427080391,217703427080392,217703427080393,218802938708166,218802938708167,218802938708168,218802938708169,219902450335942,219902450335943,219902450335944,219902450335945,221001961963718,221001961963719,221001961963720,221001961963721};
	collision_check();
	~collision_check();
protected:
private:
};
