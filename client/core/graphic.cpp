#include <sys/time.h>
#include "graphic.h"
#include "../base/interfaces.h"
#include "../base/gra_tools.h"
using namespace std;

double mouse_last_time=0;

void graphic::keys()
{
	virw_position last_pos;
	virw_position last_eye;
	last_pos = my_gra.my_pos;
	last_eye = my_gra.eye_focus;
	double x = my_gra.look_at->x - my_gra.my_pos.x;
	double y = my_gra.look_at->y - my_gra.my_pos.y;
	double z = my_gra.look_at->z - my_gra.my_pos.z;
	double dims = sqrt(x * x + z * z);

	{
		double dx = my_gra.eye_focus.x - my_gra.my_pos.x;
		double dy = my_gra.eye_focus.y - my_gra.my_pos.y;
		double dz = my_gra.eye_focus.z - my_gra.my_pos.z;
		double l = sqrt(dx * dx + dy * dy + dz * dz);
		mouse_last_time+=1.0;
		double tspeed=sqrt(mouse_last_time)*speed;
		if(mouse_button_left)
		{
			my_gra.my_pos.x+=dx*tspeed;
			my_gra.my_pos.y+=dy*tspeed;
			my_gra.my_pos.z+=dz*tspeed;
			my_gra.eye_focus.x = dx / l + my_gra.my_pos.x;
			my_gra.eye_focus.y = dy / l + my_gra.my_pos.y;
			my_gra.eye_focus.z = dz / l + my_gra.my_pos.z;
		}
		else if(mouse_button_right)
		{
			my_gra.my_pos.x-=dx*tspeed;
			my_gra.my_pos.y-=dy*tspeed;
			my_gra.my_pos.z-=dz*tspeed;
			my_gra.eye_focus.x = dx / l + my_gra.my_pos.x;
			my_gra.eye_focus.y = dy / l + my_gra.my_pos.y;
			my_gra.eye_focus.z = dz / l + my_gra.my_pos.z;
		}
		else
		{
			mouse_last_time=0;
		}
	}

	if(my_com.keys_on[kF1])
	{
		if(speed > 0.0003)
			speed *= 0.95;
	}
	if(my_com.keys_on[kF2])
	{
		if(speed < 0.6)
			speed *= 1.05;
	}

	if(my_com.keys_on[klshift])
	{
		if(my_com.keys_on_last[kleft] > 0 || my_com.keys_on[kleft])
		{
			my_gra.look_at->z -= sqrt(my_com.keys_on_last[kleft]) * my_gra.speed * x / dims;
			my_gra.look_at->x += sqrt(my_com.keys_on_last[kleft]) * my_gra.speed * z / dims;
			if(my_com.keys_on[kleft])
				my_com.keys_on_last[kleft] += 1;
			else
				my_com.keys_on_last[kleft] -= sqrt(my_com.keys_on_last[kleft]);
		}
		if(my_com.keys_on_last[kright] > 0 || my_com.keys_on[kright])
		{
			my_gra.look_at->z += sqrt(my_com.keys_on_last[kright]) * my_gra.speed * x / dims;
			my_gra.look_at->x -= sqrt(my_com.keys_on_last[kright]) * my_gra.speed * z / dims;
			if(my_com.keys_on[kright])
				my_com.keys_on_last[kright] += 1;
			else
				my_com.keys_on_last[kright] -= sqrt(my_com.keys_on_last[kright]);
		}
		if(my_com.keys_on_last[kdown] > 0 || my_com.keys_on[kdown])
		{
			my_gra.look_at->z -= sqrt(my_com.keys_on_last[kdown]) * my_gra.speed * z / dims;
			my_gra.look_at->x -= sqrt(my_com.keys_on_last[kdown]) * my_gra.speed * x / dims;
			if(my_com.keys_on[kdown])
				my_com.keys_on_last[kdown] += 1;
			else
				my_com.keys_on_last[kdown] -= sqrt(my_com.keys_on_last[kdown]);
		}
		if(my_com.keys_on_last[kup] > 0 || my_com.keys_on[kup])
		{
			my_gra.look_at->z += sqrt(my_com.keys_on_last[kup]) * my_gra.speed * z / dims;
			my_gra.look_at->x += sqrt(my_com.keys_on_last[kup]) * my_gra.speed * x / dims;
			if(my_com.keys_on[kup])
				my_com.keys_on_last[kup] += 1;
			else
				my_com.keys_on_last[kup] -= sqrt(my_com.keys_on_last[kup]);
		}
		if(my_com.keys_on_last[kpgup] > 0 || my_com.keys_on[kpgup])
		{
			my_gra.look_at->y += sqrt(my_com.keys_on_last[kpgup]) * my_gra.speed;
			if(my_com.keys_on[kpgup])
				my_com.keys_on_last[kpgup] += 1;
			else
				my_com.keys_on_last[kpgup] -= sqrt(my_com.keys_on_last[kpgup]);
		}
		if(my_com.keys_on_last[kpgdown] > 0 || my_com.keys_on[kpgdown])
		{
			my_gra.look_at->y -= sqrt(my_com.keys_on_last[kpgdown]) * my_gra.speed;
			if(my_com.keys_on[kpgdown])
				my_com.keys_on_last[kpgdown] += 1;
			else
				my_com.keys_on_last[kpgdown] -= sqrt(my_com.keys_on_last[kpgdown]);
		}
	}
	else
	{
		if(my_com.keys_on_last[kleft] > 0 || my_com.keys_on[kleft])
		{
			my_gra.my_pos.z -= sqrt(my_com.keys_on_last[kleft]) * my_gra.speed * x / dims;
			my_gra.my_pos.x += sqrt(my_com.keys_on_last[kleft]) * my_gra.speed * z / dims;
			if(my_com.keys_on[kleft])
				my_com.keys_on_last[kleft] += 1;
			else
				my_com.keys_on_last[kleft] -= sqrt(my_com.keys_on_last[kleft]);
		}
		if(my_com.keys_on_last[kright] > 0 || my_com.keys_on[kright])
		{
			my_gra.my_pos.z += sqrt(my_com.keys_on_last[kright]) * my_gra.speed * x / dims;
			my_gra.my_pos.x -= sqrt(my_com.keys_on_last[kright]) * my_gra.speed * z / dims;
			if(my_com.keys_on[kright])
				my_com.keys_on_last[kright] += 1;
			else
				my_com.keys_on_last[kright] -= sqrt(my_com.keys_on_last[kright]);
		}
		if(my_com.keys_on_last[kdown] > 0 || my_com.keys_on[kdown])
		{
			my_gra.my_pos.z -= sqrt(my_com.keys_on_last[kdown]) * my_gra.speed * z / dims;
			my_gra.my_pos.x -= sqrt(my_com.keys_on_last[kdown]) * my_gra.speed * x / dims;
			if(my_com.keys_on[kdown])
				my_com.keys_on_last[kdown] += 1;
			else
				my_com.keys_on_last[kdown] -= sqrt(my_com.keys_on_last[kdown]);
		}
		if(my_com.keys_on_last[kup] > 0 || my_com.keys_on[kup])
		{
			my_gra.my_pos.z += sqrt(my_com.keys_on_last[kup]) * my_gra.speed * z / dims;
			my_gra.my_pos.x += sqrt(my_com.keys_on_last[kup]) * my_gra.speed * x / dims;
			if(my_com.keys_on[kup])
				my_com.keys_on_last[kup] += 1;
			else
				my_com.keys_on_last[kup] -= sqrt(my_com.keys_on_last[kup]);
		}
		if(my_com.keys_on_last[kpgup] > 0 || my_com.keys_on[kpgup])
		{
			my_gra.my_pos.y += sqrt(my_com.keys_on_last[kpgup]) * my_gra.speed;
			if(my_com.keys_on[kpgup])
				my_com.keys_on_last[kpgup] += 1;
			else
				my_com.keys_on_last[kpgup] -= sqrt(my_com.keys_on_last[kpgup]);
		}
		if(my_com.keys_on_last[kpgdown] > 0 || my_com.keys_on[kpgdown])
		{
			my_gra.my_pos.y -= sqrt(my_com.keys_on_last[kpgdown]) * my_gra.speed;
			if(my_com.keys_on[kpgdown])
				my_com.keys_on_last[kpgdown] += 1;
			else
				my_com.keys_on_last[kpgdown] -= sqrt(my_com.keys_on_last[kpgdown]);
		}
	}

	if(my_com.keys_on[klalt] && my_gra.look_at == &my_gra.eye_focus)
	{
		my_gra.eye_focus.x = my_gra.my_pos.x + x;
		my_gra.eye_focus.y = my_gra.my_pos.y + y;
		my_gra.eye_focus.z = my_gra.my_pos.z + z;
	}

	/*x = my_gra.look_at->x - my_gra.my_pos.x;
	  z = my_gra.look_at->z - my_gra.my_pos.z;
	  dims = sqrt(x * x + z * z);
	  if(dims < 0.1 || my_gra.my_pos.x >= 199|| my_gra.my_pos.x <= 1 || my_gra.my_pos.z >= 199|| my_gra.my_pos.z <= 1 || my_gra.my_pos.y >= 100)
	  {
		my_gra.my_pos = last_pos;
	  }
	  if(my_gra.my_pos.y <= my_gra.get_map_height(my_gra.my_pos.x, my_gra.my_pos.z))
	  {
		my_gra.my_pos = last_pos;
	  }*/
}

void set_material(GLfloat r, GLfloat g, GLfloat b, GLfloat a, GLfloat emi)
{
	GLfloat mat[4];
	mat[0] = r;
	mat[1] = g;
	mat[2] = b;
	mat[3] = a;
	GLfloat matl[4];
	matl[0] = r * 1.2f;
	matl[1] = g * 1.2f;
	matl[2] = b * 1.2f;
	matl[3] = a;
	GLfloat mate[4];
	mate[0] = r * emi;
	mate[1] = g * emi;
	mate[2] = b * emi;
	mate[3] = a;
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, mate);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matl);
}

graphic::graphic():big_paint(1366,768,50)
{
	eye_picture=new char[480*270*3];
	string tem1[] = { "wind",     "wink",        "smile",        "wind2", "sad",       "right_leg",
				"left_leg", "right_hand1", "left_hand1",   "fight", "left_head", "right_head",
				"head_up",  "head_down",   "battle_state", "squat"
			    };
	for(int i = 0; i < MuLin_task_num; ++i)
		Task_name[i] = tem1[i];
	string tem2[] = { "skirt", "ear",   "eyebrow", "shawl", "eye",    "head",    "leg",
				"lash",  "pupil", "hair",    "chest", "armlet", "necktie", "shoe"
			    };
	for(int i = 0; i < MuLin_part_num; ++i)
		Part_name[i] = tem2[i];

	dpy = XOpenDisplay(0);
	root_window = XRootWindow(dpy, 0);
}

void graphic::display_world()
{
	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, World_texture_above);
	glBegin(GL_QUADS);
	{
		glTexCoord2f(0.999f, 0.001f);
		glVertex3d(0, 100, 0);
		glTexCoord2f(0.999f, 0.999f);
		glVertex3d(0, 100, 200);
		glTexCoord2f(0.000f, 0.999f);
		glVertex3d(200, 100, 200);
		glTexCoord2f(0.000f, 0.001f);
		glVertex3d(200, 100, 0);
	}
	glEnd();

	glBindTexture(GL_TEXTURE_2D, World_texture_front);
	glBegin(GL_QUADS);
	{
		glTexCoord2f(0.999f, 0.001f);
		glVertex3d(0, 10, 0);
		glTexCoord2f(0.999f, 0.999f);
		glVertex3d(0, 100, 0);
		glTexCoord2f(0.001f, 0.999f);
		glVertex3d(200, 100, 0);
		glTexCoord2f(0.001f, 0.001f);
		glVertex3d(200, 10, 0);
	}
	glEnd();

	glBindTexture(GL_TEXTURE_2D, World_texture_behind);
	glBegin(GL_QUADS);
	{
		glTexCoord2f(0.001f, 0.999f);
		glVertex3d(0, 100, 200);
		glTexCoord2f(0.001f, 0.001f);
		glVertex3d(0, 10, 200);
		glTexCoord2f(0.999f, 0.001f);
		glVertex3d(200, 10, 200);
		glTexCoord2f(0.999f, 0.999f);
		glVertex3d(200, 100, 200);
	}
	glEnd();
	glBindTexture(GL_TEXTURE_2D, World_texture_below);
	glBegin(GL_QUADS);
	{
		for(int i = 0; i < 40; ++i)
		{
			for(int j = 0; j < 40; ++j)
			{
				glTexCoord2d(i / 40.0, (j + 1) / 40.0);
				glVertex3d(i * 5 , map_height[(j + 1) * 41 + i], j * 5 +5);
				glTexCoord2d(i / 40.0, j / 40.0);
				glVertex3d(i * 5 , map_height[j * 41 + i], j * 5 );
				glTexCoord2d((i + 1) / 40.0, j / 40.0);
				glVertex3d(i * 5 +5, map_height[j * 41 + i + 1], j * 5 );
				glTexCoord2d((i + 1) / 40.0, (j + 1) / 40.0);
				glVertex3d(i * 5 +5, map_height[(j + 1) * 41 + i + 1], j * 5 +5);
			}
		}
	}
	glEnd();
	glBindTexture(GL_TEXTURE_2D, World_texture_left);
	glBegin(GL_QUADS);
	{
		glTexCoord2f(0.999f, 0.999f);
		glVertex3d(0, 100, 0);
		glTexCoord2f(0.001f, 0.999f);
		glVertex3d(0, 100, 200);
		glTexCoord2f(0.001f, 0.001f);
		glVertex3d(0, 10, 200);
		glTexCoord2f(0.999f, 0.001f);
		glVertex3d(0, 10, 0);
	}
	glEnd();

	glBindTexture(GL_TEXTURE_2D, World_texture_right);
	glBegin(GL_QUADS);
	{
		glTexCoord2f(0.001f, 0.001f);
		glVertex3d(200, 10, 0);
		glTexCoord2f(0.999f, 0.001f);
		glVertex3d(200, 10, 200);
		glTexCoord2f(0.999f, 0.999f);
		glVertex3d(200, 100, 200);
		glTexCoord2f(0.001f, 0.999f);
		glVertex3d(200, 100, 0);
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void graphic::display_char()
{
	glEnable(GL_TEXTURE_2D);

	double glass_dis = 3;
	double relative_pos[3];
	virw_position te_my_pos=my_pos;
	virw_position te_look_at=*look_at;
	if(use3d)
	{
		double dx,dz,l;
		dx=look_at->x-my_pos.x;
		dz=look_at->z-my_pos.z;
		l=sqrt(dx*dx+dz*dz);
		if(p3d_red)
		{
			te_my_pos.x+=0.03*(dz/l);
			te_my_pos.z-=0.03*(dx/l);
			te_look_at.x+=0.03*(dz/l);
			te_look_at.z-=0.03*(dx/l);
		}
		else
		{
			te_my_pos.x-=0.03*(dz/l);
			te_my_pos.z+=0.03*(dx/l);
			te_look_at.x-=0.03*(dz/l);
			te_look_at.z+=0.03*(dx/l);
		}
	}
	relative_pos[0] = te_my_pos.x - te_look_at.x;
	relative_pos[1] = te_my_pos.y - te_look_at.y;
	relative_pos[2] = te_my_pos.z - te_look_at.z;

	double tem_l = sqrt(pow(relative_pos[0], 2) + pow(relative_pos[2], 2)+pow(relative_pos[1], 2));
	double tem_lll = sqrt(pow(relative_pos[0], 2) + pow(relative_pos[2], 2));
	if(tem_l < 0.3)
	{
		glass_dis = tem_l / 0.1;
	}

	panel tex, tex2;
	double di = 0.01016 * glass_dis;

	double relative_angle[2];
	if(relative_pos[2] < 0)
	{
		relative_angle[0] = 180 + atan(relative_pos[0] / relative_pos[2]) * 57.29578;
	}
	else
	{
		relative_angle[0] = atan(relative_pos[0] / relative_pos[2]) * 57.29578;
	}
	relative_angle[1] = atan(relative_pos[1] / tem_lll) * 57.29578;
	double len = sqrt(pow(relative_pos[0], 2) + pow(relative_pos[1], 2) + pow(relative_pos[2], 2));

	while(my_print.system.length()>600)
	{
		my_print.cut_string(my_print.system);
	}
	while(my_print.big_screen.length()>1000)
	{
		my_print.cut_string(my_print.big_screen);
	}

	glPushMatrix();
	glTranslated(100, 31.7, 106);
	if(my_print.big_screen != "")
	{
		tex = my_font.get_panel(my_print.big_screen, 540);
		set_material(1.2, 1.2, 1.2, 0.7, 1.2);
		glBindTexture(GL_TEXTURE_2D, tex.texture);
		glBegin(GL_QUADS);
		{
			glTexCoord2f(1, 1);
			glVertex3d(-2, -tex.height / 300.0, 0);
			glTexCoord2f(1, 0);
			glVertex3d(-2, tex.height / 300.0, 0);
			glTexCoord2f(0, 0);
			glVertex3d(2, tex.height / 300.0, 0);
			glTexCoord2f(0, 1);
			glVertex3d(2, -tex.height / 300.0, 0);
		}
		glEnd();
	}
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
	set_material(1 - Cr, 1 - Cg, 1 - Cb, Ca+0.3, 1.0f);
	glPushMatrix();
	glTranslated(100.05, 31.25, 104.05);
	glRotated(my_states.world_run_count, 0, 1, 0);
	glutSolidCube(0.1);
	glPopMatrix();

	glEnable(GL_TEXTURE_2D);

	glPushMatrix();
	glTranslated(te_my_pos.x - di * relative_pos[0] / len,
			te_my_pos.y - di * relative_pos[1] / len,
			te_my_pos.z - di * relative_pos[2] / len);
	glRotated(relative_angle[0], 0, 1, 0);
	glRotated(relative_angle[1], -1, 0, 0);

	double pos = 0.0059 * glass_dis;
	if(my_print.history != "")
	{
		tex = my_font.get_panel(my_print.history, 600);
		pos = 0.0059 * glass_dis - tex.height * glass_dis / 86000.0;
	}
	if(my_print.typing_pre + my_print.typing != "")
	{
		tex2 = my_font.get_panel(my_print.typing_pre + my_print.typing, 600);
		pos -= tex2.height * glass_dis / 86000.0;
	}
	if(pos < -0.004 * glass_dis)
	{
		my_font.cutstring(my_print.history,600);
		tex = my_font.get_panel(my_print.history, 600);
	}
	pos = 0.0059 * glass_dis;
	if(my_print.history != "")
	{
		set_material(0.9, 1.0, 1.3, 0.66, 1.2);
		glBindTexture(GL_TEXTURE_2D, tex.texture);
		glBegin(GL_QUADS);
		{
			glTexCoord2f(0, 1);
			glVertex3d(-0.01 * glass_dis, pos - tex.height * glass_dis / 86000.0, 0);
			glTexCoord2f(0, 0);
			glVertex3d(-0.01 * glass_dis, pos, 0);
			glTexCoord2f(1, 0);
			glVertex3d(-0.0034 * glass_dis, pos, 0);
			glTexCoord2f(1, 1);
			glVertex3d(-0.0034 * glass_dis, pos - tex.height * glass_dis / 86000.0, 0);
		}
		glEnd();
		pos -= tex.height * glass_dis / 86000.0;
	}
	if(my_print.typing_pre + my_print.typing != "")
	{
		set_material(1.5, 1.5, 2.0, 0.8, 1.2);
		glBindTexture(GL_TEXTURE_2D, tex2.texture);
		glBegin(GL_QUADS);
		{
			glTexCoord2f(0, 1);
			glVertex3d(-0.01 * glass_dis, pos - tex2.height * glass_dis / 86000.0, 0);
			glTexCoord2f(0, 0);
			glVertex3d(-0.01 * glass_dis, pos, 0);
			glTexCoord2f(1, 0);
			glVertex3d(-0.0034 * glass_dis, pos, 0);
			glTexCoord2f(1, 1);
			glVertex3d(-0.0034 * glass_dis, pos - tex2.height * glass_dis / 86000.0, 0);
		}
		glEnd();
		pos -= tex2.height * glass_dis / 86000.0;
	}
	if(my_com.input_sc.show != "")
	{
		tex2 = my_font.get_panel(my_com.input_sc.show, 600);
		set_material(1.5, 1.5, 2.0, 0.9, 1.2);
		glBindTexture(GL_TEXTURE_2D, tex2.texture);
		glBegin(GL_QUADS);
		{
			glTexCoord2f(0, 1);
			glVertex3d(-0.01 * glass_dis, pos - tex2.height * glass_dis / 86000.0, 0);
			glTexCoord2f(0, 0);
			glVertex3d(-0.01 * glass_dis, pos, 0);
			glTexCoord2f(1, 0);
			glVertex3d(-0.0034 * glass_dis, pos, 0);
			glTexCoord2f(1, 1);
			glVertex3d(-0.0034 * glass_dis, pos - tex2.height * glass_dis / 86000.0, 0);
		}
		glEnd();
	}
	my_states.refresh_show();
	{
		double spos = -0.0059 * glass_dis;
		if(my_states.state_show != "")
		{
			tex = my_font.get_panel(my_states.state_show, 1600);
			set_material(1.2, 1.5, 1.2, 0.7, 1.2);
			glBindTexture(GL_TEXTURE_2D, tex.texture);
			glBegin(GL_QUADS);
			{
				glTexCoord2f(0, 1);
				glVertex3d(-0.01 * glass_dis, spos, 0);
				glTexCoord2f(0, 0);
				glVertex3d(-0.01 * glass_dis, spos + tex.height * glass_dis / 86000.0, 0);
				glTexCoord2f(1, 0);
				glVertex3d(0.01 * glass_dis, spos + tex.height * glass_dis / 86000.0, 0);
				glTexCoord2f(1, 1);
				glVertex3d(0.01 * glass_dis, spos, 0);
			}
			glEnd();
			pos += tex.height * glass_dis / 86000.0;
		}
	}
	pos = -0.005528 * glass_dis;
	if(my_print.system != "")
	{
		tex = my_font.get_panel(my_print.system, 1080);
		if(pos + tex.height * glass_dis / 86000.0 > -0.001)
		{
			my_font.cutstring(my_print.system,1080);
			tex = my_font.get_panel(my_print.system, 1080);
		}
		set_material(1.5, 1.5, 1.5, 0.8, 1.2);
		glBindTexture(GL_TEXTURE_2D, tex.texture);
		glBegin(GL_QUADS);
		{
			glTexCoord2f(0, 1);
			glVertex3d(-0.002 * glass_dis, pos, 0);
			glTexCoord2f(0, 0);
			glVertex3d(-0.002 * glass_dis, pos + tex.height * glass_dis / 86000.0, 0);
			glTexCoord2f(1, 0);
			glVertex3d(0.0094 * glass_dis, pos + tex.height * glass_dis / 86000.0, 0);
			glTexCoord2f(1, 1);
			glVertex3d(0.0094 * glass_dis, pos, 0);
		}
		glEnd();
		pos += tex.height * glass_dis / 86000.0;
	}

	double bpos = 0.0056 * glass_dis;
	if(my_print.talked != "")
	{
		tex = my_font.get_panel(my_print.talked, 720);
		set_material(1.6, 1.5, 1.4, 0.6, 1.2);
		glBindTexture(GL_TEXTURE_2D, tex.texture);
		glBegin(GL_QUADS);
		{
			glTexCoord2f(0, 1);
			glVertex3d(0.002 * glass_dis, bpos - tex.height * glass_dis / 90000.0, 0);
			glTexCoord2f(0, 0);
			glVertex3d(0.002 * glass_dis, bpos, 0);
			glTexCoord2f(1, 0);
			glVertex3d(0.01 * glass_dis, bpos, 0);
			glTexCoord2f(1, 1);
			glVertex3d(0.01 * glass_dis, bpos - tex.height * glass_dis / 90000.0, 0);
		}
		glEnd();
		bpos -= tex.height * glass_dis / 90000.0;
	}
	if(my_print.talking != "")
	{
		tex = my_font.get_panel(my_print.talking, 640);
		set_material(2.0, 2.0, 2.0, 0.8, 1.2);
		glBindTexture(GL_TEXTURE_2D, tex.texture);
		glBegin(GL_QUADS);
		{
			glTexCoord2f(0, 1);
			glVertex3d(0.002 * glass_dis, bpos - tex.height * glass_dis / 80000.0, 0);
			glTexCoord2f(0, 0);
			glVertex3d(0.002 * glass_dis, bpos, 0);
			glTexCoord2f(1, 0);
			glVertex3d(0.01 * glass_dis, bpos, 0);
			glTexCoord2f(1, 1);
			glVertex3d(0.01 * glass_dis, bpos - tex.height * glass_dis / 80000.0, 0);
		}
		glEnd();
		bpos -= tex.height * glass_dis / 80000.0;
	}
	if(bpos <= pos || bpos < -0.005 * glass_dis)
		my_font.cutstring(my_print.talked,720);

	double tpos = 0.0056 * glass_dis;
	if(my_print.title != "")
	{
		tex = my_font.get_panel(my_print.title, 320);
		set_material(1.5, 1.5, 2.0, 0.8, 1.2);
		glBindTexture(GL_TEXTURE_2D, tex.texture);
		glBegin(GL_QUADS);
		{
			glTexCoord2f(0, 1);
			glVertex3d(-0.003 * glass_dis, tpos - tex.height * glass_dis / 80000.0, 0);
			glTexCoord2f(0, 0);
			glVertex3d(-0.003 * glass_dis, tpos, 0);
			glTexCoord2f(1, 0);
			glVertex3d(0.0016 * glass_dis, tpos, 0);
			glTexCoord2f(1, 1);
			glVertex3d(0.0016 * glass_dis, tpos - tex.height * glass_dis / 80000.0, 0);
		}
		glEnd();
		tpos = 0.0056 * glass_dis - tex.height * glass_dis / 80000.0;
	}
	if(tpos <= pos || tpos < -0.005 * glass_dis)
		my_font.cutstring(my_print.title,300);

	if(my_print.mutable_message != "")
	{
		tex = my_font.get_panel(my_print.mutable_message, 360);
		set_material(1.5, 1.5, 2.0, 0.8, 1.2);
		glBindTexture(GL_TEXTURE_2D, tex.texture);
		glBegin(GL_QUADS);
		{
			glTexCoord2f(0, 1);
			glVertex3d(-0.003 * glass_dis, tpos - tex.height * glass_dis / 86000.0, 0);
			glTexCoord2f(0, 0);
			glVertex3d(-0.003 * glass_dis, tpos, 0);
			glTexCoord2f(1, 0);
			glVertex3d(0.0016 * glass_dis, tpos, 0);
			glTexCoord2f(1, 1);
			glVertex3d(0.0016 * glass_dis, tpos - tex.height * glass_dis / 86000.0, 0);
		}
		glEnd();
		tpos = 0.0056 * glass_dis - tex.height * glass_dis / 80000.0;
	}
	if(tpos <= pos || tpos < -0.005 * glass_dis)
		my_font.cutstring(my_print.mutable_message,350);

	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
}

void graphic::display_paint()
{
	static int x=0;
	big_paint.change_color(255,0,0);
	big_paint.put_pixel(x/60,live_tree_num);
	big_paint.change_color(0,255,0);
	big_paint.put_pixel(x/60,live_grass_eat_num);
	big_paint.change_color(0,0,255);
	big_paint.put_pixel(x/60,live_meat_eat_num);
	big_paint.change_color(0,0,0);
	big_paint.put_pixel(x/60,live_fly_eat_num);
	x++;
	if(x>1366*60)
	{
		big_paint.clear();
		x=0;
	}

	glEnable(GL_TEXTURE_2D);

	set_material(1,1,1,1,1);
	glPushMatrix();
	glTranslated(100, 31.7, 94);
	{
		glBindTexture(GL_TEXTURE_2D, big_paint.get_texture_show());
		glBegin(GL_QUADS);
		{
			glTexCoord2f(0, 0);
			glVertex3d(-2, -1.125, 0);
			glTexCoord2f(0, 1);
			glVertex3d(-2, 1.125, 0);
			glTexCoord2f(1, 1);
			glVertex3d(2, 1.125, 0);
			glTexCoord2f(1, 0);
			glVertex3d(2, -1.125, 0);
		}
		glEnd();
	}
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
}

void graphic::display_light()
{
	GLfloat sun_light_position[] = { 1, 1, -1, 0 };
	GLfloat sun_light_ambient[] = { GLfloat(0.3 * light), GLfloat(0.3 * light), GLfloat(0.2 * light), 1.0f };
	GLfloat sun_light_diffuse[] = { GLfloat(0.2 * light), GLfloat(0.15 * light), GLfloat(0.1 * light), 1.0f };
	GLfloat sun_light_specular[] = { GLfloat(0.5 * light), GLfloat(0.4 * light), GLfloat(0.3 * light), 1.0f };

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, sun_light_position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, sun_light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, sun_light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, sun_light_specular);

	GLfloat sun2_light_position[] = { -1, -1, 1, 0 };
	GLfloat sun2_light_ambient[] = { GLfloat(0.45 * light), GLfloat(0.4 * light), GLfloat(0.4 * light), 1.0f };
	GLfloat sun2_light_diffuse[] = { GLfloat(0.45 * light), GLfloat(0.4 * light), GLfloat(0.4 * light), 1.0f };
	GLfloat sun2_light_specular[] = { GLfloat(0.65 * light), GLfloat(0.6 * light), GLfloat(0.6 * light), 1.0f };
	glEnable(GL_LIGHT1);
	glLightfv(GL_LIGHT1, GL_POSITION, sun2_light_position);
	glLightfv(GL_LIGHT1, GL_AMBIENT, sun2_light_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, sun2_light_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, sun2_light_specular);
}

void graphic::display()
{
	display_light();
	display_world();
	display_paint();

	glPushMatrix();
	glTranslated(mulin_pos.x+ mulin_distortion.x, mulin_pos.y+ mulin_distortion.y, mulin_pos.z+ mulin_distortion.z);
	glRotated(mulin_distortion.b+mulin_pos.b, 0, 1, 0);
	MuLin_model_display.draw(MuLin_show, head_eye_angle, MuLin_eye_texture);
	glPopMatrix();

	for(int i = 0; i < live_grain_num; ++i)
	{
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, grains[i].color);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, grains[i].color);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, grains[i].color);

		glPushMatrix();
		glTranslatef(grains[i].pos.x, grains[i].pos.y, grains[i].pos.z);
		glRotatef(grains[i].pos.a, 1, 0, 0);
		glRotatef(grains[i].pos.b, 0, 1, 0);
		glRotatef(grains[i].pos.c, 0, 0, 1);
		models[grains[i].model_id].list_draw();
		glPopMatrix();
	}
	for(int i = 0; i < live_fly_eat_num; ++i)
	{
		glPushMatrix();
		glTranslatef(fly_eats[i].x,fly_eats[i].y,fly_eats[i].z);
		glRotatef(fly_eats[i].b, 0, 1, 0);
		glRotatef(fly_eats[i].a, -1, 0, 0);
		glRotatef(fly_eats[i].c, 0, 0, 1);
		float temb=pow(fly_eats_big[i],0.3333);
		glScalef(temb,temb,temb);
		if(fly_eats_dead[i])
			models[enum_id_fly_eat].no_color_draw();
		else
			models[enum_id_fly_eat].less_draw();
		glPopMatrix();
	}
	for(int i = 0; i < live_grass_eat_num; ++i)
	{
		glPushMatrix();
		glTranslatef(grass_eats[i].x,grass_eats[i].y,grass_eats[i].z);
		glRotatef(grass_eats[i].b, 0, 1, 0);
		glRotatef(grass_eats[i].a, -1, 0, 0);
		glRotatef(grass_eats[i].c, 0, 0, 1);
		float temb=pow(grass_eats_big[i],0.3333);
		glScalef(temb,temb,temb);
		if(grass_eats_dead[i])
			models[enum_id_grass_eat].no_color_draw();
		else
			models[enum_id_grass_eat].less_draw();
		glPopMatrix();
	}
	for(int i = 0; i < live_meat_eat_num; ++i)
	{
		glPushMatrix();
		glTranslatef(meat_eats[i].x,meat_eats[i].y,meat_eats[i].z);
		glRotatef(meat_eats[i].b, 0, 1, 0);
		glRotatef(meat_eats[i].a, -1, 0, 0);
		glRotatef(meat_eats[i].c, 0, 0, 1);
		float temb=pow(meat_eats_big[i],0.3333);
		glScalef(temb,temb,temb);
		if(meat_eats_dead[i])
			models[enum_id_meat_eat].no_color_draw();
		else
			models[enum_id_meat_eat].less_draw();
		glPopMatrix();
	}

	for(int i = 0; i < live_tree_num; ++i)
	{
		glPushMatrix();
		glTranslatef(trees[i].x,trees[i].y,trees[i].z);
		float temb=pow(trees_big[i],0.3333);
		glScalef(temb,temb,temb);
		models[enum_id_tree].less_draw();
		glPopMatrix();
	}

	for(int i = 0; i < live_cube_num; ++i)
	{
		glPushMatrix();
		set_material(cubes_color[i*4],cubes_color[i*4+1],cubes_color[i*4+2],cubes_color[i*4+3],1.0);
		double px=((cubes[i]&0xfffff0000000000ul)>>40)/10.0;
		double py=((cubes[i]&0x00000fffff00000ul)>>20)/10.0;
		double pz=(cubes[i]&0x0000000000ffffful)/10.0;
		glTranslated(px+0.05,py+0.05,pz+0.05);
		glutSolidCube(0.1);
		glPopMatrix();
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	if(my_com.cub_com!=0&&my_com.cub_state>=1)
	{
		glPushMatrix();
		set_material(Cr,Cg,Cb,0.6,2.0);
		double px,py,pz;
		double sx,sy,sz;
		if(my_com.cub_state==1)
		{
			px=eye_focus.x+cub_tem1.x;
			py=eye_focus.y+cub_tem1.y;
			pz=eye_focus.z+cub_tem1.z;
			px/=2;
			py/=2;
			pz/=2;
			sx=eye_focus.x-cub_tem1.x;
			sy=eye_focus.y-cub_tem1.y;
			sz=eye_focus.z-cub_tem1.z;
		}
		else
		{
			px=cub_tem2.x+cub_tem1.x;
			py=cub_tem2.y+cub_tem1.y;
			pz=cub_tem2.z+cub_tem1.z;
			px/=2;
			py/=2;
			pz/=2;
			sx=cub_tem2.x-cub_tem1.x;
			sy=cub_tem2.y-cub_tem1.y;
			sz=cub_tem2.z-cub_tem1.z;
		}
		glTranslated(px,py,pz);
		glScaled(sx,sy,sz);
		glutSolidCube(1);
		glPopMatrix();
	}

	if(post&&look_at == &eye_focus)
	{
		if(shoot_kind==enum_shoot_cube||shoot_kind==enum_shoot_decube)
		{
			glPushMatrix();
			if(rand_cube_color)
			{
				set_material(Cr,Cg,Cb,Ca,1.0);
			}
			else
			{
				set_material(construct_cube_color[0],construct_cube_color[1],construct_cube_color[2],construct_cube_color[3],1.0);
			}
			int xx=eye_focus.x*10;
			int yy=eye_focus.y*10;
			int zz=eye_focus.z*10;
			double px=xx/10.0;
			double py=yy/10.0;
			double pz=zz/10.0;
			glBegin(GL_LINES);
			glVertex3d(px-200,py,pz);
			glVertex3d(px+200,py,pz);
			glVertex3d(px-200,py+0.10,pz);
			glVertex3d(px+200,py+0.10,pz);
			glVertex3d(px-200,py,pz+0.10);
			glVertex3d(px+200,py,pz+0.10);
			glVertex3d(px-200,py+0.10,pz+0.10);
			glVertex3d(px+200,py+0.10,pz+0.10);
			glVertex3d(px,py-200,pz);
			glVertex3d(px,py+200,pz);
			glVertex3d(px+0.10,py-200,pz);
			glVertex3d(px+0.10,py+200,pz);
			glVertex3d(px,py-200,pz+0.10);
			glVertex3d(px,py+200,pz+0.10);
			glVertex3d(px+0.10,py-200,pz+0.10);
			glVertex3d(px+0.10,py+200,pz+0.10);
			glVertex3d(px,py,pz-200);
			glVertex3d(px,py,pz+200);
			glVertex3d(px+0.10,py,pz-200);
			glVertex3d(px+0.10,py,pz+200);
			glVertex3d(px,py+0.10,pz-200);
			glVertex3d(px,py+0.10,pz+200);
			glVertex3d(px+0.10,py+0.10,pz-200);
			glVertex3d(px+0.10,py+0.10,pz+200);
			glEnd();
			glTranslated(px+0.05,py+0.05,pz+0.05);
			glutSolidCube(0.1);
			glPopMatrix();
		}
		else
		{
			glPushMatrix();
			glTranslated(look_at->x, look_at->y, look_at->z);
			set_material(0.6, 0.4, 0, 0.7, 1.0);
			glBegin(GL_LINES);
			glVertex3d(-0.01, 0, 0);
			glVertex3d(0.01, 0, 0);
			glVertex3d(0, -0.01, 0);
			glVertex3d(0, 0.01, 0);
			glVertex3d(0, 0, -0.01);
			glVertex3d(0, 0, 0.01);
			glEnd();
			glPopMatrix();
		}
	}

	//gra_client and shoots;
	{
		gra_clients_lock.slock();
		set<gra_client*>::iterator iter;
		for(iter=gra_clients.begin(); iter!=gra_clients.end(); iter++)
		{
			if((*iter)->id_socket!=my_states.myself.id_socket&&(*iter)->login_state!=enum_login_mulin)
			{
				if((*iter)->login_state==enum_login_shtb)
				{
					set_material((*iter)->color[0],(*iter)->color[1],(*iter)->color[2],(*iter)->color[3],1.0);
				}
				else
				{
					set_material(0.4,0.4,0.4,1.0,0);
				}
				glPushMatrix();
				glTranslated((*iter)->pos.x,(*iter)->pos.y,(*iter)->pos.z);
				glScaled(0.1,0.3,0.1);
				glutSolidOctahedron();
				glPopMatrix();
			}
		}
		gra_clients_lock.unlock();
	}
	{
		shoots_lock.slock();
		set<shoot*>::iterator iter;
		for (iter = shoots.begin(); iter != shoots.end(); iter++)
		{
			if((*iter)->boom)
			{
				switch((*iter)->kind)
				{
					case(enum_shoot_destroy):
						set_material(1-Cr,1-Cg,1-Cb,0.6,true);
						glPushMatrix();
						glTranslated((*iter)->my_attack.x,(*iter)->my_attack.y,(*iter)->my_attack.z);
						glScaled((*iter)->bom_r,(*iter)->bom_r*1.5,(*iter)->bom_r);
						glutSolidOctahedron();
						glPopMatrix();
						break;
					case(enum_shoot_grow):
						set_material(1-Cr,1-Cg,1-Cb,0.6,true);
						glPushMatrix();
						glTranslated((*iter)->my_attack.x,(*iter)->my_attack.y,(*iter)->my_attack.z);
						glScaled((*iter)->bom_r,(*iter)->bom_r*1.5,(*iter)->bom_r);
						glutWireOctahedron();
						glPopMatrix();
						break;
					case(enum_shoot_get):
						set_material(1-Cr,1-Cg,1-Cb,0.6,true);
						glPushMatrix();
						glTranslated((*iter)->my_attack.x,(*iter)->my_attack.y,(*iter)->my_attack.z);
						glScaled((*iter)->bom_r,(*iter)->bom_r*1.5,(*iter)->bom_r);
						glutWireSphere(1,9,9);
						glPopMatrix();
						break;
					default:
						set_material(1-Cr,1-Cg,1-Cb,0.6,true);
						glPushMatrix();
						glTranslated((*iter)->my_attack.x,(*iter)->my_attack.y,(*iter)->my_attack.z);
						glScaled((*iter)->bom_r,(*iter)->bom_r*1.5,(*iter)->bom_r);
						glutSolidSphere(1,9,9);
						glPopMatrix();
						break;
						break;
				}
			}
			else
			{
				set_material(1-Cr,1-Cg,1-Cb,0.8,true);
				glPushMatrix();
				glTranslated((*iter)->my_attack.x,(*iter)->my_attack.y-0.01/(*iter)->live,(*iter)->my_attack.z);
				glRotated((*iter)->my_attack.b, 0, 1, 0);
				World_gear_model[13].draw();
				glutSolidSphere(0.03,4,4);
				glPopMatrix();
			}
		}
		shoots_lock.unlock();
	}

	glPushMatrix();
	GLfloat mat53_emission[] = { 1,1,1,1};
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, mat53_emission);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat53_emission);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat53_emission);
	glTranslated(100.05, elevator_height-0.1, 99.94);
	glScaled(1,0.02,1);
	glutSolidSphere(1.5,4,16);
	glPopMatrix();
	glPushMatrix();
	GLfloat mat532_emission[] = { 0.5,0.3,0.1,1};
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, mat532_emission);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat532_emission);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat532_emission);
	glTranslated(100.05, 29.94, 99.94);
	glScaled(1,0.01,1);
	glRotated(90,1,0,0);
	glutSolidTorus(4.25,5.75,4,36);
	glPopMatrix();

	glDepthMask(GL_FALSE);

	/*double ro = my_states.world_run_count/50.0;
	for(int i = 0; i < 15; ++i)
	{
		GLfloat mat5_emission[] = { GLfloat(1.2 - Cr * (i + 4) * 0.5),
						    GLfloat(1.2 - Cg * (i + 4) * 0.5),
						    GLfloat(1.2 - Cb * (i + 4) * 0.5),
						    GLfloat(0.9 - i / 18.0)
						  };
		if(!gear_transparent)
		{
			mat5_emission[3]=1.0;
		}
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, mat5_emission);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat5_emission);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat5_emission);
		glPushMatrix();
		if(i<6)
		{
			glTranslated(100.05, 29.94, 99.94);
		}
		else
		{
			glTranslated(100.05, elevator_height-0.1, 99.94);
		}
		glScaled(0.774,1.0,0.774);
		glPushMatrix();
		glRotated(ro * pow(-2, i), 0, 1, 0);
		World_gear_model[i].draw();
		glPopMatrix();
		glPopMatrix();
	}

	glPushMatrix();
	glTranslated(100.05, elevator_height-0.1, 99.94);
	glutSolidSphere(0.05, 10, 10);
	glPopMatrix();*/

	display_char();


	glDepthMask(GL_TRUE);

	glDisable(GL_BLEND);

	set_material(2, 2, 2, 1, 1.0);
	if(fog.render)
	{
		glEnable(GL_FOG);
		fog.color[0] = Cr;
		fog.color[1] = Cg;
		fog.color[2] = Cb;
		glFogfv(GL_FOG_COLOR, fog.color);
		glFogi(GL_FOG_MODE, GL_EXP);
		glFogf(GL_FOG_DENSITY, fog.density);
		glFogf(GL_FOG_START, 0.01f);
		glFogf(GL_FOG_END, 100.0f);
		glHint(GL_FOG_HINT, GL_DONT_CARE);
	}
	else
	{
		glDisable(GL_FOG);
	}

	if(accum.init)
	{
		glAccum(GL_LOAD, 0);
		accum.init = false;
	}
	if(accum.on)
	{
		glAccum(GL_ACCUM, accum.para);
		glAccum(GL_MULT, 1.0 / (accum.para+1.0));
		glAccum(GL_RETURN, 1.0);
	}
	if(mirror)
	{
		GLuint texID;
		glGenTextures(1, &texID);
		glBindTexture(GL_TEXTURE_2D, texID);
		glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0, 1366, 768, 0);
		glTexParameteri(GL_TEXTURE_2D,
		    GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,
		    GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		big_paint.set_texture_id(texID);
	}
		//glReadPixels(0,0,13,7,GL_RGB,GL_UNSIGNED_BYTE,big_paint.get_image());
}

void graphic::mixture(int task_or, int task_part_or)
{
	double state;
	int time = 0;

	state = MuLin_task[task_or].state; // k-快,m-慢

	if(MuLin_task[task_or].real == 1) // m-k-m-k-m
	{
		state = (sin(3.1415926535897 * (state - 0.5)) + 1) / 2;
	}
	else if(MuLin_task[task_or].real == 2)   // k-m-k
	{
		state = sin(3.1415926535897 / 2 * state);
	}
	else if(MuLin_task[task_or].real == 3)   // m-k-m
	{
		state = sin(3.1415926535897 / 2 * (state - 1.0)) + 1;
	}
	else if(MuLin_task[task_or].real == 4)   // m-k|-m-k-m
	{
		if(MuLin_task[task_or].direction)
			state = sin(3.1415926535897 / 2 * state);
		else
			state = (sin(3.1415926535897 * (state - 0.5)) + 1) / 2;
	}
	state *= MuLin_task[task_or].limit;

	GL_model_group* group_d = &(MuLin_model_display.group[MuLin_task[task_or].task_part[task_part_or].part_order]);
	int min, max;
	min = group_d->min;
	max = group_d->max;
	for(int i = min; i < max; ++i)
	{
		MuLin_model_display.ver[i] += state * MuLin_task[task_or].task_part[task_part_or].contrast[time];
		++time;
	}
}

void graphic::idle()
{
	static unsigned int run_time;
	run_time++;
	if(shoot_speed>0)
	{
		if(my_com.keys_on[klctrl]&&run_time%shoot_speed==0)
		{
			stringstream ss;
			if(shoot_kind==enum_shoot_cube)
			{
				virw_position tmpo;
				if(rand_cube_color)
				{
					tmpo.x=Cr;
					tmpo.y=Cg;
					tmpo.z=Cb;
					tmpo.a=Ca;
				}
				else
				{
					tmpo.x=construct_cube_color[0];
					tmpo.y=construct_cube_color[1];
					tmpo.z=construct_cube_color[2];
					tmpo.a=construct_cube_color[3];
				}
				ss.write((char*)&tmpo,sizeof(virw_position));
			}
			else if(shoot_kind==enum_shoot_decube)
			{
				virw_position tmpo;
				tmpo.a=-1;
				ss.write((char*)&tmpo,sizeof(virw_position));
			}
			else
			{
				ss.write((char*)&my_pos,sizeof(virw_position));
			}
			ss.write((char*)look_at,sizeof(virw_position));
			ss.write((char*)&shoot_kind,sizeof(unsigned char));
			string ssstr=ss.str();
			my_net.send_to("shoot1",ssstr);
		}
	}

	if(world_changed!=-1)
	{
		switch(world_changed)
		{
			case(0):
			{
				GLuint last_front_tex=World_texture_front;
				World_texture_front = tool_load_texture(my_file.files[enum_file_world] + "_front.bmp",0);
				if(World_texture_front==0)
					my_print.error(my_file.files[enum_file_world] + "_front.bmp file not found!");
				world_changed++;
				glDeleteTextures(1, &last_front_tex);
			}
				break;
			case(1):
			{
				glDeleteTextures(1, &World_texture_below);
				World_texture_below = tool_load_texture(my_file.files[enum_file_world] + "_below.bmp",0);
				if(World_texture_below==0)
					World_texture_below = tool_load_texture(my_file.files[enum_file_world] + "_front.bmp",2);
				world_changed++;
			}
				break;
			case(2):
			{
				glDeleteTextures(1, &World_texture_left);
				World_texture_left = tool_load_texture(my_file.files[enum_file_world] + "_left.bmp",0);
				if(World_texture_left==0)
					World_texture_left=World_texture_front;
				world_changed++;
			}
				break;
			case(3):
			{
				glDeleteTextures(1, &World_texture_behind);
				World_texture_behind = tool_load_texture(my_file.files[enum_file_world] + "_behind.bmp",0);
				if(World_texture_behind==0)
					World_texture_behind=World_texture_front;
				world_changed++;
			}
				break;
			case(4):
			{
				glDeleteTextures(1, &World_texture_right);
				World_texture_right = tool_load_texture(my_file.files[enum_file_world] + "_right.bmp",0);
				if(World_texture_right==0)
					World_texture_right=World_texture_front;
				world_changed++;
			}
				break;
			case(5):
			{
				glDeleteTextures(1, &World_texture_above);
				World_texture_above = tool_load_texture(my_file.files[enum_file_world] + "_above.bmp",0);
				if(World_texture_above==0)
					World_texture_above= tool_load_texture(my_file.files[enum_file_world] + "_front.bmp",1);
				world_changed=-1;
			}
				break;
			default:
				my_print.error("TEXTURE_CHANGE ERROR!");
				break;
		}
	}

	if(eye_pic_ch)
	{
		GLuint last_tex=MuLin_eye_texture;
		GLuint texture_ID;
		glGenTextures(1, &texture_ID);
		if(texture_ID == 0)
		{
			cerr << "texture number not get!!!" << endl;
		}
		else
		{
			glBindTexture(GL_TEXTURE_2D, texture_ID);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 480, 270, 0, GL_RGB, GL_UNSIGNED_BYTE, eye_picture);
			MuLin_eye_texture=texture_ID;
		}
		glDeleteTextures(1, &last_tex);
		eye_pic_ch=false;
	}

	keys();

	if(!good_bye)
	{
		stringstream ss;
		ss.write((char*)&my_pos,sizeof(virw_position));
		ss.write((char*)&eye_focus,sizeof(virw_position));
		string ssstr=ss.str();
		my_net.send_to("pos_re",ssstr);
	}

	timeval now;
	gettimeofday(&now, 0);
	long waited = 0;
	static long last = now.tv_sec * 1000000 + now.tv_usec;
	waited = now.tv_sec * 1000000 + now.tv_usec - last;
	if(refresh_by_net)
	{
		int wait_t=0;
		while(!net_flush && !t_save_sy&&wait_t<100)
		{
			wait_t++;
			usleep(1000);
		}
	}
	else
	{
		while(waited < 1000000 / picture_num && !t_save_sy)
		{
			gettimeofday(&now, 0);
			waited = now.tv_sec * 1000000 + now.tv_usec - last;
			usleep(500);
		}
	}

	last = now.tv_sec * 1000000 + now.tv_usec;
	if(t_save_sy)
	{
		t_save();
		t_saved_sy = true;
	}
	if(t_bye_sy)
	{
		t_finished_sy = true;
	}
	if(daemon_bye)
	{
		usleep(600000);
		exit(0);
	}
	if(net_flush)
	{
		static size_t all_ver_size = 12 * MuLin_model_display.ver_num + 12;
		memcpy(MuLin_model_display.ver, MuLin_model_const.ver, all_ver_size);

		for(int i = 0; i < MuLin_task_num; ++i)
		{
			if(MuLin_task[i].play_on)
			{
				for(int j = 0; j < MuLin_task[i].task_part_num; ++j)
					if(MuLin_show[MuLin_task[i].task_part[j].part_order])
					{
						mixture(i, j);
					}
			}
		}
		net_flush = false;
	}
	static bool first_time = true;
	if(first_time)
	{
		models[enum_id_maple].creat_list();
		models[enum_id_petal].creat_list();
		models[enum_id_snow].creat_list();
		models[enum_id_hail].creat_list();
		models[enum_id_gear].creat_list();
		my_print.started = true;
		glAccum(GL_LOAD, 0);
		first_time = false;
	}

	mulin_look=mulin_pos;
	mulin_look.y+=1.55;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, 1.7, 0.01, 300);
	glClearColor(Cr,Cg,Cb,1.0);

	if(use3d)
	{
		double dx,dz,l;
		dx=look_at->x-my_pos.x;
		dz=look_at->z-my_pos.z;
		l=sqrt(dx*dx+dz*dz);
		p3d_red=true;
		glColorMask(true,false,false,true);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		//gluLookAt(my_pos.x+0.03*(dz/l), my_pos.y, my_pos.z-0.03*(dx/l), look_at->x, look_at->y, look_at->z, 0, 1, 0);
		gluLookAt(my_pos.x+0.03*(dz/l), my_pos.y, my_pos.z-0.03*(dx/l), look_at->x+0.03*(dz/l), look_at->y, look_at->z-0.03*(dx/l), 0, 1, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		display();
		p3d_red=false;
		glColorMask(false,true,true,true);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(my_pos.x-0.03*(dz/l), my_pos.y, my_pos.z+0.03*(dx/l), look_at->x-0.03*(dz/l), look_at->y, look_at->z+0.03*(dx/l), 0, 1, 0);
		glClear( GL_DEPTH_BUFFER_BIT);
		display();
		glColorMask(true,true,true,true);
	}
	else
	{
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(my_pos.x, my_pos.y, my_pos.z, look_at->x, look_at->y, look_at->z, 0, 1, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		display();
	}
	glutSwapBuffers();

}

void graphic::key(unsigned char key, int, int)
{
	if(key == 27)
	{
		good_bye = true;
	}
	else
	{
		key_board_node key_tem;
		key_tem.key = key;
		key_tem.state = key_down;
		my_com.push_key(key_tem);
	}
}

void graphic::keyup(unsigned char key, int, int)
{
	key_board_node key_tem;
	key_tem.key = key;
	key_tem.state = key_up;
	my_com.push_key(key_tem);
}

void graphic::skey(int key, int, int)
{
	switch(key)
	{
		case(7):
		{
			my_print.typing_pre="";
			my_com.input_receiver=default_input;
		}
			break;
		case(10):
		{
			if(my_com.input_lock)
			{
				my_com.input_lock=false;
				if(my_print.typing=="")
				{
					my_print.typing_pre="";
					my_com.input_receiver=default_input;
				}
			}
			else
			{
				my_com.input_lock=true;
			}
		}
			break;
		case(11):
		{
			if(full_screen)
			{
				glutReshapeWindow(640, 360);
				accum.init = true;
				glutSetCursor(GLUT_CURSOR_FULL_CROSSHAIR);
			}
			else
			{
				glutFullScreen();
				accum.init = true;
				glutSetCursor(GLUT_CURSOR_NONE);
			}
			full_screen = !full_screen;
		}
			break;
		case(12):
		{
			my_font.change_ground();
		}
			break;
		case(114):
		{
			if(my_com.cub_com==0)
			{
				my_com.keys_on[klctrl]=true;
				stringstream ss;
				if(shoot_kind==enum_shoot_cube)
				{
					virw_position tmpo;
					if(rand_cube_color)
					{
						tmpo.x=Cr;
						tmpo.y=Cg;
						tmpo.z=Cb;
						tmpo.a=Ca;
					}
					else
					{
						tmpo.x=construct_cube_color[0];
						tmpo.y=construct_cube_color[1];
						tmpo.z=construct_cube_color[2];
						tmpo.a=construct_cube_color[3];
					}
					ss.write((char*)&tmpo,sizeof(virw_position));
				}
				else if(shoot_kind==enum_shoot_decube)
				{
					virw_position tmpo;
					tmpo.a=-1;
					ss.write((char*)&tmpo,sizeof(virw_position));
				}
				else
				{
					ss.write((char*)&my_pos,sizeof(virw_position));
				}
				ss.write((char*)look_at,sizeof(virw_position));
				ss.write((char*)&shoot_kind,sizeof(unsigned char));
				string ssstr=ss.str();
				my_net.send_to("shoot1",ssstr);
			}
			else if(my_com.cub_com=='m'||my_com.cub_com=='c')
			{
				if(my_com.cub_state==0)
				{
					my_print.message("two position left:");
					my_com.cub_ss.write((char*)&eye_focus,sizeof(virw_position));
					cub_tem1=eye_focus;
					my_com.cub_state++;
				}
				else if(my_com.cub_state==1)
				{
					my_print.message("one position left:");
					my_com.cub_ss.write((char*)&eye_focus,sizeof(virw_position));
					cub_tem2=eye_focus;
					my_com.cub_state++;
				}
				else if(my_com.cub_state==2)
				{
					my_com.cub_ss.write((char*)&eye_focus,sizeof(virw_position));
					string ssstr=my_com.cub_ss.str();
					my_net.send_to("cub_co",ssstr);
					my_com.cub_com=0;
				}
			}
			else if(my_com.cub_com=='r')
			{
				if(my_com.cub_state==0)
				{
					my_print.message("one position left:");
					my_com.cub_ss.write((char*)&eye_focus,sizeof(virw_position));
					cub_tem1=eye_focus;
					my_com.cub_state++;
				}
				else if(my_com.cub_state==1)
				{
					my_com.cub_ss.write((char*)&eye_focus,sizeof(virw_position));
					string ssstr=my_com.cub_ss.str();
					my_net.send_to("cub_co",ssstr);
					my_com.cub_com=0;
				}
			}
			else if(my_com.cub_com=='d')
			{
				if(my_com.cub_state==0)
				{
					my_print.message("one position left:");
					my_com.cub_ss.write((char*)&eye_focus,sizeof(virw_position));
					cub_tem1=eye_focus;
					my_com.cub_state++;
				}
				else if(my_com.cub_state==1)
				{
					my_com.cub_ss.write((char*)&eye_focus,sizeof(virw_position));
					string ssstr=my_com.cub_ss.str();
					my_net.send_to("cub_co",ssstr);
					my_com.cub_com=0;
				}
			}
		}
			break;
		default:
		{
			key_board_node key_tem;
			key_tem.key = 1000 + key;
			key_tem.state = key_down;
			my_com.push_key(key_tem);
		}
			break;
	}
}

void graphic::skeyup(int key, int, int)
{
	key_board_node key_tem;
	key_tem.key = 1000 + key;
	key_tem.state = key_up;
	my_com.push_key(key_tem);
}

void shell_display()
{
	my_gra.display();
}
void shell_key(unsigned char key, int, int)
{
	my_gra.key(key, 0, 0);
}
void shell_keyup(unsigned char key, int, int)
{
	my_gra.keyup(key, 0, 0);
}
void shell_skey(int key, int, int)
{
	my_gra.skey(key, 0, 0);
}
void shell_skeyup(int key, int, int)
{
	my_gra.skeyup(key, 0, 0);
}
void shell_idle()
{
	my_gra.idle();
}

void graphic::init_task()
{
	MuLin_task[wind].init(3);
	MuLin_task[wind2].init(3);
	MuLin_task[wink].init(2, true, 1, false, false, 0.017);
	MuLin_task[smile].init(3);
	MuLin_task[sad].init(3);
	MuLin_task[right_leg].init(3, true, 2, false, false, 0.05);
	MuLin_task[left_leg].init(3, true, 2, false, false, 0.05);
	MuLin_task[right_hand1].init(2, true, 1, false, false, 0.05);
	MuLin_task[left_hand1].init(2, true, 1, false, false, 0.06);
	MuLin_task[fight].init(4, true, 3, false, false, 0.16);
	MuLin_task[left_head].init(7);
	MuLin_task[right_head].init(7);
	MuLin_task[head_up].init(7);
	MuLin_task[head_down].init(7);
	MuLin_task[battle_state].init(3, false, 0, false, false, 0.02);
	MuLin_task[squat].init(3, false, 0, false, false, 0.02);
}

void graphic::load_models()
{
	my_print.message("加载模型:" + my_file.files[enum_file_model_magic]);
	World_gear_model[0].init(my_file.files[enum_file_model_magic]);
	World_gear_model[0].scale(14, 1, 14);
	for(int i = 1; i < 15; ++i)
	{
		World_gear_model[0].copy_to(World_gear_model[i]);
		World_gear_model[i].scale(pow(0.7, i), pow(0.7, i), pow(0.7, i));
	}

	my_print.message("加载模型:" + my_file.files[enum_file_model_muLin]);
	MuLin_model_display.init(my_file.files[enum_file_model_muLin]);
	MuLin_model_display.copy_to(MuLin_model_const);

	ifstream color_file(my_file.pub_path+"color/" + color_file_name + ".color", ios::binary | ios::in);
	if (!color_file)
	{
		my_print.warning(my_file.pub_path + "color/" + color_file_name + ".color file  not found!");
	}
	else
	{
		for (int i = 0; i < my_gra.MuLin_model_display.group_num; ++i)
		{
			color_file.read((char*)&(my_gra.MuLin_model_display.material[i]), sizeof(GL_model_material));
		}
	}
	color_file.close();

	models[enum_id_maple].init(my_file.files[enum_file_model_maple]);
	models[enum_id_petal].init(my_file.files[enum_file_model_petal]);
	models[enum_id_hail].init(my_file.files[enum_file_model_hail]);
	models[enum_id_snow].init(my_file.files[enum_file_model_snow]);
	models[enum_id_gear].init(my_file.files[enum_file_model_gear]);

	models[enum_id_tree].init(my_file.files[enum_file_model_tree]);
	models[enum_id_grass_eat].init(my_file.files[enum_file_model_grass_eat]);
	models[enum_id_meat_eat].init(my_file.files[enum_file_model_meat_eat]);
	models[enum_id_fly_eat].init(my_file.files[enum_file_model_fly_eat]);

	my_print.message("加载动画数据:" + my_file.files[enum_file_model_contrast]);
	ifstream con(my_file.files[enum_file_model_contrast], ios::in | ios::binary);
	string task_s, part_s;
	int con_num = 0;
	char ta[21] = {};
	char pa[21] = {};
	int len = 0;
	int part_order = -1, task_order = -1;

	while(con.read((char*)&len, 4))
	{
		con.read(ta, 20);
		ta[len] = '\0';
		task_s = ta;
		con.read((char*)&len, 4);
		con.read(pa, 20);
		pa[len] = '\0';
		part_s = pa;
		con.read((char*)&con_num, 4);
		// my_print.message("加载动画数据:" + task_s + " " + part_s);
		for(int i = 0; i < MuLin_task_num; ++i)
		{
			if(task_s == Task_name[i])
			{
				task_order = i;
				break;
			}
		}
		if(task_order == -1)
		{
			my_print.error("find unkown task name——" + task_s + " " + part_s);
		}
		for(int i = 0; i < MuLin_part_num; ++i)
		{
			if(part_s == Part_name[i])
			{
				part_order = i;
				break;
			}
		}
		if(part_order == -1)
		{
			my_print.error("find unkown part name——" + task_s + " " + part_s);
		}
		MuLin_task[task_order].task_part[MuLin_task[task_order].part_postion].init(con_num, part_order);
		con.read((char*)MuLin_task[task_order].task_part[MuLin_task[task_order].part_postion].contrast, 4 * con_num);
		MuLin_task[task_order].part_postion++;
		task_order = -1;
		part_order = -1;
	}
}

double graphic::get_map_height(double x, double y)
{
	int a = y / 5;
	int b = x / 5;
	if(a < 0 || b < 0 || a > 39 || b > 39)
	{
		return 0;
	}
	if(x + y < 5 * (a + b) + 5)
	{
		double i = map_height[a * 41 + b];
		double j = map_height[a * 41 + b + 1];
		double k = map_height[a * 41 + b + 41];
		return i + (j - i) * (x / 5 - b) + (k - i) * (y / 5 - a);
	}
	else
	{
		double i = map_height[a * 41 + b + 42];
		double j = map_height[a * 41 + b + 1];
		double k = map_height[a * 41 + b + 41];
		return i + (k - i) * (1 - (x / 5 - b)) + (j - i) * (1 - (y / 5 - a));
	}
}

void mouse_move(int x,int y)
{
	if(!my_gra.full_screen)
	{
		return;
	}
	static bool auto_moved=false;
	static int last_x=x,last_y=y;
	double mdx=x-last_x;
	double mdy=y-last_y;
	last_x=x;
	last_y=y;
	if(auto_moved&&(mdx>100||mdy>60||mdx<-60||mdy<-100))
	{
		auto_moved=false;
		return;
	}
	double dx = my_gra.look_at->x - my_gra.my_pos.x;
	double dy = my_gra.look_at->y - my_gra.my_pos.y;
	double dz = my_gra.look_at->z - my_gra.my_pos.z;
	double l = sqrt(dx * dx + dy * dy + dz * dz);
	my_gra.eye_focus.x = dx / l + my_gra.my_pos.x;
	my_gra.eye_focus.y = dy / l + my_gra.my_pos.y;
	my_gra.eye_focus.z = dz / l + my_gra.my_pos.z;

	my_gra.eye_focus.y-=0.001*mdy;

	double dims = sqrt(dx * dx + dz * dz);
	if(mdx>0)
	{
		my_gra.eye_focus.z += 0.001*mdx*dx / dims;
		my_gra.eye_focus.x -= 0.001*mdx* dz / dims;
	}
	else if(mdx<0)
	{
		my_gra.eye_focus.z += 0.001*mdx*dx / dims;
		my_gra.eye_focus.x -= 0.001*mdx* dz / dims;
	}
	if(x>1250||x<100||y<100||y>650)
	{
		auto_moved=true;
		XSelectInput(my_gra.dpy, my_gra.root_window, KeyReleaseMask);
		XWarpPointer(my_gra.dpy, None, my_gra.root_window, 0, 0, 0, 0, 675, 375);
		XFlush(my_gra.dpy);
	}
}

void mouse_click(int button,int state,int,int)
{
	if(button==GLUT_LEFT_BUTTON&&state==GLUT_DOWN)
	{
		my_gra.mouse_button_left=true;
	}
	else if(button==GLUT_LEFT_BUTTON&&state==GLUT_UP)
	{
		my_gra.mouse_button_left=false;
	}
	else if(button==GLUT_RIGHT_BUTTON&&state==GLUT_DOWN)
	{
		my_gra.mouse_button_right=true;
	}
	else
	{
		my_gra.mouse_button_right=false;
	}
}

int graphic::t_run()
{
    look_at = &eye_focus;
	my_pos.z = 101;
	my_pos.y = 31.6;
	mulin_pos.y=30;
	eye_focus.y = 31.6;
	while(!t_load_sy)
	{
		usleep(1000);
	}
	my_font.init(my_file.files[enum_file_font]);
	init_task();
	t_load();
	load_models();
	t_loaded_sy = true;
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGBA | GLUT_MULTISAMPLE);
	glutCreateWindow("MuLin");
	glutDisplayFunc(shell_display);
	glutKeyboardFunc(shell_key);
	glutKeyboardUpFunc(shell_keyup);
	glutSpecialFunc(shell_skey);
	glutSpecialUpFunc(shell_skeyup);
	glutPassiveMotionFunc(mouse_move);
	glutMotionFunc(mouse_move);
	glutMouseFunc(mouse_click);
	glutIdleFunc(shell_idle);
	glEnable(GL_DEPTH_TEST);
	glEnable(GLUT_MULTISAMPLE);
	glutFullScreen();
	glutSetCursor(GLUT_CURSOR_NONE);
	t_started_sy = true;
	glutMainLoop();
	return 0;
}

int graphic::t_load()
{
	ifstream gra_conf(my_file.files[enum_file_gra_config],ios::in|ios::binary);
	if(gra_conf)
	{
		gra_conf.read((char*)&accum,sizeof(gra_accum));
		gra_conf.read((char*)&use3d,sizeof(bool));
		gra_conf.read((char*)&post,sizeof(bool));
		gra_conf.read((char*)&shoot_speed,sizeof(int));
		gra_conf.read((char*)&gear_transparent,sizeof(bool));
		gra_conf.read((char*)&refresh_by_net,sizeof(bool));
		gra_conf.read((char*)&picture_num,sizeof(double));
		gra_conf.read((char*)&my_com.input_lock,sizeof(bool));
		size_t sz;
		gra_conf.read((char*)&sz,sizeof(size_t));
		char*tes=new char[sz+1];
		gra_conf.read(tes,sz);
		tes[sz]='\0';
		color_file_name=tes;
		delete[] tes;
	}
	else
	{
		my_print.warning("gra_conf not found!");
	}
	return 0;
}

int graphic::t_save()
{
	ofstream gra_conf(my_file.files[enum_file_gra_config],ios::out|ios::binary);
	if(gra_conf)
	{
		gra_conf.write((char*)&accum,sizeof(gra_accum));
		gra_conf.write((char*)&use3d,sizeof(bool));
		gra_conf.write((char*)&post,sizeof(bool));
		gra_conf.write((char*)&shoot_speed,sizeof(int));
		gra_conf.write((char*)&gear_transparent,sizeof(bool));
		gra_conf.write((char*)&refresh_by_net,sizeof(bool));
		gra_conf.write((char*)&picture_num,sizeof(double));
		gra_conf.write((char*)&my_com.input_lock,sizeof(bool));
		size_t sz=color_file_name.length();
		gra_conf.write((char*)&sz,sizeof(size_t));
		gra_conf.write(color_file_name.c_str(),sz);
	}
	else
	{
		my_print.warning("gra_conf create failed!");
	}
	return 0;
}

graphic::~graphic()
{
	// dtor
}
