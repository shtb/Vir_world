#include "paint_board.h"
#include"interfaces.h"

paint_board::paint_board(unsigned int wid,unsigned int hei,unsigned int refresh_buf)
{
	  if(wid>0&&hei>0&&wid<10000&&hei<10000)
	  {
		    refresh_buffer=refresh_buf;
		    width=wid;
		    height=hei;
		    line_byte = wid * 3;
		    while(line_byte % 4 != 0)
		    {
				++line_byte;
		    }
		    board=new unsigned char[line_byte*hei];
		    clear();
	  }
	  else
	  {
		    my_print.error("wrong paint board width or hei!");
		    cerr<<"wrong paint board init:"<<wid<<"-"<<hei<<endl;
	  }
}

void paint_board::put_rect(unsigned int x,unsigned int y,unsigned int w,unsigned int h,unsigned char*p)
{
	  if(x+w<=width&&y+h<=height)
	  {
		    int pl_byte = w * 3;
		    while(pl_byte % 4 != 0)
		    {
				++pl_byte;
		    }
		    int tem_c,tem_d;
		    for(unsigned int i=0; i<h; ++i)
		    {
				tem_c=line_byte*(y+i);
				tem_d=pl_byte*i;
				if(p!=NULL)
					  memcpy(board+tem_c+3*x,p+tem_d,pl_byte);
				else
					  for(unsigned int te=0; te<w; ++te)
					  {
						    *(board+tem_c+3*x+te*3)=cr;
						    *(board+tem_c+3*x+te*3+1)=cg;
						    *(board+tem_c+3*x+te*3+2)=cb;
					  }
		    }
		    buffered+=w*h;
	  }
}

void paint_board::set_texture_id(GLuint id)
{
	buffered=0;
	glDeleteTextures(1, &last_texture);
	last_texture=id;
}

GLuint paint_board::get_texture_show()
{
	  if(buffered>refresh_buffer)
	  {
		    buffered=0;
		    GLuint texture_ID;
		    glGenTextures(1, &texture_ID);
		    if(texture_ID == 0)
		    {
				my_print.error("paint_board:texture number not get!!!");
				return 0;
		    }
		    else
		    {
				glBindTexture(GL_TEXTURE_2D, texture_ID);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, board);
		    }
		    glDeleteTextures(1, &last_texture);
		    last_texture=texture_ID;
		    return texture_ID;
	  }
	  else
	  {
		    return last_texture;
		    return 0;
	  }
}

void paint_board::clear()
{
	  memset(board,255,line_byte*height);
	  buffered+=refresh_buffer;
}

paint_board::~paint_board()
{
	  if(exist)
	  {
		    delete[] board;
		    glDeleteTextures(1, &last_texture);
	  }
}
