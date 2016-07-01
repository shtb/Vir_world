#include "GL_model.h"
#include "interfaces.h"

void GL_model::copy_to(GL_model& model)
{
    model.ver_num = this->ver_num;
    model.nor_num = this->nor_num;
    model.ver = new float[3 * ver_num + 3];
    model.nor = new float[3 * nor_num + 3];
    model.group_num = this->group_num;
    model.group_name = new string[group_num];
    model.material = this->material;
    model.group = new GL_model_group[group_num];
    for(int i = 3; i < 3 * ver_num + 3; ++i)
    {
	  model.ver[i] = this->ver[i];
    }
    for(int i = 3; i < 3 * nor_num + 3; ++i)
    {
	  model.nor[i] = this->nor[i];
    }
    for(int i = 0; i < group_num; ++i)
    {
	  model.group_name[i] = this->group_name[i];
	  model.group[i].triangle_num = this->group[i].triangle_num;
	  model.group[i].min = this->group[i].min;
	  model.group[i].max = this->group[i].max;
	  model.group[i].triangle = new GL_model_triangle[this->group[i].triangle_num];
	  for(int j = 0; j < group[i].triangle_num; ++j)
	  {
		model.group[i].triangle[j] = this->group[i].triangle[j];
	  }
    }
}

void GL_model::save_ori(string fil)
{
    ofstream file(fil, ios::out | ios::binary);
    if(!file)
    {
	  my_print.error("file open failed!--" + fil);
    }
    if(can_draw)
    {
	  GL_model_triangle* tri;
	  for(int g = 0; g < group_num; ++g)
	  {
		for(int i = 0; i < group[g].triangle_num; ++i)
		{
		    tri = &group[g].triangle[i];
		    tri->ver_nor[0] /= 3;
		    tri->ver_nor[1] /= 3;
		    tri->ver_nor[2] /= 3;
		    tri->ver_pos[0]/= 3;
		    tri->ver_pos[1] /= 3;
		    tri->ver_pos[2] /= 3;
		}
	  }
    }
    file.write((char*)&ver_num, 4);
    file.write((char*)&nor_num, 4);
    file.write((char*)ver, 12 * ver_num + 12);
    file.write((char*)nor, 12 * nor_num + 12);
    file.write((char*)&group_num, 4);
    for(int i = 0; i < group_num; ++i)
    {
	  size_t len = group_name[i].length();
	  file.write((char*)&len, 4);
	  file.write(group_name[i].c_str(), len);
	  file.write((char*)&group[i].triangle_num, 4);
	  file.write((char*)&group[i].min, 4);
	  file.write((char*)&group[i].max, 4);
	  file.write((char*)&material[i], sizeof(GL_model_material));
	  file.write((char*)group[i].triangle, sizeof(GL_model_triangle) * group[i].triangle_num);
    }
    if(can_draw)
    {
	  GL_model_triangle* tri;
	  for(int g = 0; g < group_num; ++g)
	  {
		for(int i = 0; i < group[g].triangle_num; ++i)
		{
		    tri = &group[g].triangle[i];
		    tri->ver_nor[0] *= 3;
		    tri->ver_nor[1] *= 3;
		    tri->ver_nor[2] *= 3;
		    tri->ver_pos[0] *= 3;
		    tri->ver_pos[1] *= 3;
		    tri->ver_pos[2] *= 3;
		}
	  }
    }
    file.close();
}

void GL_model::scale(double ratio_a, double ratio_b, double ratio_c)
{
    for(int i = 3; i < (3 * ver_num + 3); i += 3)
    {
	  ver[i] *= ratio_a;
	  ver[i + 1] *= ratio_b;
	  ver[i + 2] *= ratio_c;
    }
}

void GL_model::translate(double ratio_a, double ratio_b, double ratio_c)
{
    for(int i = 3; i < (3 * ver_num + 3); i += 3)
    {
	  ver[i] += ratio_a;
	  ver[i + 1] += ratio_b;
	  ver[i + 2] += ratio_c;
    }
}

void GL_model::rotate(double ratio_a, double ratio_b, double ratio_c)
{
    ratio_a/=57.2957795;
    ratio_b/=57.2957795;
    ratio_c/=57.2957795;
    for(int i = 3; i < (3 * ver_num + 3); i += 3)
    {
	  float cosa=cos(ratio_a);
	  float cosb=cos(ratio_b);
	  float cosc=cos(ratio_c);
	  float sina=sin(ratio_a);
	  float sinb=sin(ratio_b);
	  float sinc=sin(ratio_c);
	  float x=ver[i];
	  float y=ver[i+1];
	  float z=ver[i+2];
	  ver[i]=x*cosc-y*sinc;
	  ver[i+1]=x*sinc+y*cosc;
	  x=ver[i];
	  y=ver[i+1];
	  ver[i]=x*cosb+z*sinb;
	  ver[i+2]=z*cosb-x*sinb;
	  x=ver[i];
	  z=ver[i+2];
	  ver[i+1]=y*cosa-z*sina;
	  ver[i+2]=y*sina+z*cosa;
    }
}

void GL_model::init(string fil)
{
    ifstream file(fil, ios::in | ios::binary);
    if(!file)
    {
	  my_print.error("file open failed!--" + fil);
    }
    else
    {

	  file.read((char*)&ver_num, 4);
	  file.read((char*)&nor_num, 4);
	  ver = new float[ver_num * 3 + 3];
	  nor = new float[nor_num * 3 + 3];
	  file.read((char*)ver, 12 * ver_num + 12);
	  file.read((char*)nor, 12 * nor_num + 12);
	  file.read((char*)&group_num, 4);
	  group = new GL_model_group[group_num];
	  group_name = new string[group_num];
	  material = new GL_model_material[group_num];
	  for(int i = 0; i < group_num; ++i)
	  {
		int len = 0;
		file.read((char*)&len, 4);
		char* s = new char[len + 1];
		file.read(s, len);
		s[len] = '\0';
		group_name[i] = s;
		delete[] s;
		file.read((char*)&group[i].triangle_num, 4);
		file.read((char*)&group[i].min, 4);
		file.read((char*)&group[i].max, 4);
		group[i].triangle = new GL_model_triangle[group[i].triangle_num];
		file.read((char*)&material[i], sizeof(GL_model_material));
		file.read((char*)group[i].triangle, sizeof(GL_model_triangle) * group[i].triangle_num);
	  }
	  finished = true;
    }
}

void GL_model::destroy()
{
    delete[] ver;
    delete[] nor;
    delete[] group;
    delete[] group_name;
    delete[] material;
    material=NULL;
    for(int i = 0; i < group_num; ++i)
    {
	  delete[] group[i].triangle;
    }
}

void GL_model::copy_color(GL_model mo)
{
    if(mo.finished)
    {
	  for(int i = 0; i < group_num; ++i)
	  {
		material[i] = mo.material[i];
		group_name[i] = mo.group_name[i];
	  }
    }
}

void GL_model::load_obj(string fil)
{
    ifstream obj(fil, ios::in);
    string s;
    stringstream ss;
    int tri_num[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int all_ver_num = 0;
    int all_nor_num = 0;
    while(getline(obj, s))
    {
	  switch(s[0])
	  {
	  case('v'):
	  {
		switch(s[1])
		{
		case(' '):
		{
		    all_ver_num++;
		    break;
		}
		case('n'):
		{
		    all_nor_num++;
		    break;
		}
		default:
		    break;
		}
		break;
	  }
	  case('f'):
	  {
		tri_num[group_num]++;
		break;
	  }
	  case('g'):
	  {
		group_num++;
		break;
	  }
	  default:
		break;
	  }
    }
    ver = new float[all_ver_num * 3 + 3];
    nor = new float[all_nor_num * 3 + 3];
    ver_num = all_ver_num;
    nor_num = all_nor_num;
    group_name = new string[group_num];
    material = new GL_model_material[group_num];
    group = new GL_model_group[group_num];
    for(int c = 0; c < group_num; ++c)
    {
	  group[c].triangle_num = tri_num[c + 1];
	  group[c].triangle = new GL_model_triangle[tri_num[c + 1]];
    }

    obj.close();
    ifstream robj(fil, ios::in);
    int all_ver_pos = 1, all_nor_pos = 1;
    int pos_group = -1;
    int pos_triangle = 0;
    while(getline(robj, s))
    {
	  switch(s[0])
	  {
	  case('v'):
	  {
		switch(s[1])
		{
		case(' '):
		{
		    stringstream ss(s);
		    ss >> s >> ver[3 * all_ver_pos] >> ver[3 * all_ver_pos + 1] >> ver[3 * all_ver_pos + 2];
		    all_ver_pos++;
		    break;
		}
		case('n'):
		{
		    stringstream ss(s);
		    ss >> s >> nor[3 * all_nor_pos] >> nor[3 * all_nor_pos + 1] >> nor[3 * all_nor_pos + 2];
		    all_nor_pos++;
		    break;
		}
		default:
		    break;
		}
		break;
	  }
	  case('f'):
	  {
		stringstream ss(s);
		char ch;
		int vn, nn;
		ss >> s;

		for(int cv = 0; cv < 3; ++cv)
		{
		    ss >> vn >> ch >> nn >> ch >> nn;
		    group[pos_group].triangle[pos_triangle].ver_pos[cv] = vn;
		    group[pos_group].triangle[pos_triangle].ver_nor[cv] = nn;
		}
		pos_triangle++;
		break;
	  }
	  case('g'):
	  {
		pos_group++;
		stringstream ss(s);
		while(ss >> group_name[pos_group])
		    ;
		pos_triangle = 0;
		break;
	  }
	  default:
		break;
	  }
    }
}

void GL_model::draw(bool show[], double eye_angle[], GLuint draw_texture)
{
    if(!can_draw)
    {
	  GL_model_triangle* tri;
	  for(int g = 0; g < group_num; ++g)
	  {
		for(int i = 0; i < group[g].triangle_num; ++i)
		{
		    tri = &group[g].triangle[i];
		    tri->ver_nor[0] *= 3;
		    tri->ver_nor[1] *= 3;
		    tri->ver_nor[2] *= 3;
		    tri->ver_pos[0] *= 3;
		    tri->ver_pos[1] *= 3;
		    tri->ver_pos[2] *= 3;
		}
	  }
	  can_draw = true;
    }
    GL_model_triangle* tri;
    for(int g = 0; g < group_num; ++g)
    {
	  if(show[g])
	  {
		glPushMatrix();
		glMaterialfv(GL_FRONT, GL_EMISSION, material[g].emission);
		glMaterialfv(GL_FRONT, GL_AMBIENT, material[g].ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, material[g].diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, material[g].specular);
		glMaterialfv(GL_FRONT, GL_SHININESS, &material[g].shininess);
		if(g == 8)
		{
		    glEnable(GL_BLEND);
		    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		    glEnable(GL_TEXTURE_2D);
		    glBindTexture(GL_TEXTURE_2D, draw_texture);
		}
		else if(g == 2 || g == 7)
		{
		    glEnable(GL_BLEND);
		    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}
		else
		{
		    glDisable(GL_BLEND);
		    glDisable(GL_TEXTURE_2D);
		}
		if(g == 8)
		{
		    glTranslated(0.02, 1.6, -0.119);
		    glRotated(eye_angle[0]/25, 0, 1, 0);
		    glRotated(-eye_angle[1]/25, 1, 0, 0);
		    glTranslated(-0.02, -1.6, 0.12);
		    glBegin(GL_TRIANGLES);
		    double wa, wb, ha, hb;
		    tri = &group[g].triangle[0];
		    wb = wa = ver[tri->ver_pos[0]];
		    hb = ha = ver[tri->ver_pos[0] + 1];
		    for(int i = 0; i < group[g].triangle_num; ++i)
		    {
			  tri = &group[g].triangle[i];
			  for(int j = 0; j < 3; ++j)
			  {
				if(wa > ver[tri->ver_pos[j]])
				    wa = ver[tri->ver_pos[j]];
				if(wb < ver[tri->ver_pos[j]])
				    wb = ver[tri->ver_pos[j]];
				if(ha > ver[tri->ver_pos[j] + 1])
				    ha = ver[tri->ver_pos[j] + 1];
				if(hb < ver[tri->ver_pos[j] + 1])
				    hb = ver[tri->ver_pos[j] + 1];
			  }
		    }
		    double wi = (wb - wa) / 5;
		    double he = hb - ha;
		    for(int i = 0; i < group[g].triangle_num; ++i)
		    {
			  tri = &group[g].triangle[i];
			  glTexCoord2f(1-(ver[tri->ver_pos[0]] - wa) / wi, (ver[tri->ver_pos[0] + 1] - ha) / he - 0.15);
			  glNormal3fv(&nor[tri->ver_nor[0]]);
			  glVertex3fv(&ver[tri->ver_pos[0]]);
			  glTexCoord2f(1-(ver[tri->ver_pos[1]] - wa) / wi, (ver[tri->ver_pos[1] + 1] - ha) / he - 0.1);
			  glNormal3fv(&nor[tri->ver_nor[1]]);
			  glVertex3fv(&ver[tri->ver_pos[1]]);
			  glTexCoord2f(1-(ver[tri->ver_pos[2]] - wa) / wi, (ver[tri->ver_pos[2] + 1] - ha) / he - 0.1);
			  glNormal3fv(&nor[tri->ver_nor[2]]);
			  glVertex3fv(&ver[tri->ver_pos[2]]);
		    }
		    glEnd();
		}
		else
		{
		    glBegin(GL_TRIANGLES);
		    for(int i = 0; i < group[g].triangle_num; ++i)
		    {
			  tri = &group[g].triangle[i];

			  glNormal3fv(&nor[tri->ver_nor[0]]);
			  glVertex3fv(&ver[tri->ver_pos[0]]);
			  glNormal3fv(&nor[tri->ver_nor[1]]);
			  glVertex3fv(&ver[tri->ver_pos[1]]);
			  glNormal3fv(&nor[tri->ver_nor[2]]);
			  glVertex3fv(&ver[tri->ver_pos[2]]);
		    }
		    glEnd();
		}
		glPopMatrix();
	  }
    }
}

void GL_model::draw()
{
    if(!can_draw)
    {
	  GL_model_triangle* tri;
	  for(int g = 0; g < group_num; ++g)
	  {
		for(int i = 0; i < group[g].triangle_num; ++i)
		{
		    tri = &group[g].triangle[i];
		    tri->ver_nor[0] *= 3;
		    tri->ver_nor[1] *= 3;
		    tri->ver_nor[2] *= 3;
		    tri->ver_pos[0] *= 3;
		    tri->ver_pos[1] *= 3;
		    tri->ver_pos[2] *= 3;
		}
	  }
	  can_draw = true;
    }
    GL_model_triangle* tri;
    glBegin(GL_TRIANGLES);
    for(int g = 0; g < group_num; ++g)
    {
	  for(int i = 0; i < group[g].triangle_num; ++i)
	  {
		tri = &group[g].triangle[i];

		glNormal3fv(&nor[tri->ver_nor[0]]);
		glVertex3fv(&ver[tri->ver_pos[0]]);
		glNormal3fv(&nor[tri->ver_nor[1]]);
		glVertex3fv(&ver[tri->ver_pos[1]]);
		glNormal3fv(&nor[tri->ver_nor[2]]);
		glVertex3fv(&ver[tri->ver_pos[2]]);
	  }
    }
    glEnd();
}

void GL_model::creat_list()
{
    if(!can_draw)
    {
	  GL_model_triangle* tri;
	  for(int g = 0; g < group_num; ++g)
	  {
		for(int i = 0; i < group[g].triangle_num; ++i)
		{
		    tri = &group[g].triangle[i];
		    tri->ver_nor[0] *= 3;
		    tri->ver_nor[1] *= 3;
		    tri->ver_nor[2] *= 3;
		    tri->ver_pos[0] *= 3;
		    tri->ver_pos[1] *= 3;
		    tri->ver_pos[2] *= 3;
		}
	  }
	  can_draw = true;
    }

    list_pos = glGenLists(1);
    glNewList(list_pos, GL_COMPILE);
    GL_model_triangle* tri;
    glBegin(GL_TRIANGLES);
    for(int i = 0; i < group[0].triangle_num; ++i)
    {
	  tri = &group[0].triangle[i];

	  glVertex3fv(&ver[tri->ver_pos[0]]);
	  glVertex3fv(&ver[tri->ver_pos[1]]);
	  glVertex3fv(&ver[tri->ver_pos[2]]);
    }
    glEnd();
    glEndList();
}
void GL_model::less_draw()
{
    if(!can_draw)
    {
	  GL_model_triangle* tri;
	  for(int g = 0; g < group_num; ++g)
	  {
		for(int i = 0; i < group[g].triangle_num; ++i)
		{
		    tri = &group[g].triangle[i];
		    tri->ver_nor[0] *= 3;
		    tri->ver_nor[1] *= 3;
		    tri->ver_nor[2] *= 3;
		    tri->ver_pos[0] *= 3;
		    tri->ver_pos[1] *= 3;
		    tri->ver_pos[2] *= 3;
		}
	  }
	  can_draw = true;
    }
    GL_model_triangle* tri;
    glBegin(GL_TRIANGLES);
    for(int g = 0; g < group_num; ++g)
    {
	  glMaterialfv(GL_FRONT, GL_EMISSION, material[g].emission);
	  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, material[g].ambient);
	  glMaterialfv(GL_FRONT, GL_SPECULAR, material[g].specular);
	  glMaterialfv(GL_FRONT, GL_SHININESS, &material[g].shininess);
	  for(int i = 0; i < group[g].triangle_num; ++i)
	  {
		tri = &group[g].triangle[i];

		glNormal3fv(&nor[tri->ver_nor[0]]);
		glVertex3fv(&ver[tri->ver_pos[0]]);
		glNormal3fv(&nor[tri->ver_nor[1]]);
		glVertex3fv(&ver[tri->ver_pos[1]]);
		glNormal3fv(&nor[tri->ver_nor[2]]);
		glVertex3fv(&ver[tri->ver_pos[2]]);
	  }
    }
    glEnd();
}

void GL_model::no_color_draw()
{
    if(!can_draw)
    {
	  GL_model_triangle* tri;
	  for(int g = 0; g < group_num; ++g)
	  {
		for(int i = 0; i < group[g].triangle_num; ++i)
		{
		    tri = &group[g].triangle[i];
		    tri->ver_nor[0] *= 3;
		    tri->ver_nor[1] *= 3;
		    tri->ver_nor[2] *= 3;
		    tri->ver_pos[0] *= 3;
		    tri->ver_pos[1] *= 3;
		    tri->ver_pos[2] *= 3;
		}
	  }
	  can_draw = true;
    }
    GL_model_triangle* tri;
    glBegin(GL_TRIANGLES);
    glMaterialfv(GL_FRONT, GL_EMISSION, material[0].emission);
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, material[0].ambient);
    glMaterialfv(GL_FRONT, GL_SPECULAR, material[0].specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, &material[0].shininess);
    for(int g = 0; g < group_num; ++g)
    {
	  for(int i = 0; i < group[g].triangle_num; ++i)
	  {
		tri = &group[g].triangle[i];

		glNormal3fv(&nor[tri->ver_nor[0]]);
		glVertex3fv(&ver[tri->ver_pos[0]]);
		glNormal3fv(&nor[tri->ver_nor[1]]);
		glVertex3fv(&ver[tri->ver_pos[1]]);
		glNormal3fv(&nor[tri->ver_nor[2]]);
		glVertex3fv(&ver[tri->ver_pos[2]]);
	  }
    }
    glEnd();
}


void GL_model::list_draw()
{
    glCallList(list_pos);
}
