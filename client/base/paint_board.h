#ifndef PAINT_BOARD_H
#define PAINT_BOARD_H

#include <GL/glut.h>

class paint_board
{
public:
	  unsigned int refresh_buffer;
	  unsigned int width=0;
	  unsigned int height=0;
	  unsigned char cr=0,cg=0,cb=0;
	  paint_board(unsigned int wid,unsigned int hei,unsigned int refresh_buf);
	  ~paint_board();
	  void set_texture_id(GLuint id);
	  GLuint get_texture_show();
	  void put_pixel(unsigned int x,unsigned int y)
	  {
		    if(x<width&&y<height)
		    {
				board[line_byte*y+3*x]=cr;
				board[line_byte*y+3*x+1]=cg;
				board[line_byte*y+3*x+2]=cb;
				++buffered;
		    }
	  };
	  void put_rect(unsigned int x,unsigned int y,unsigned int w,unsigned int h,unsigned char*p);
	  void change_color(unsigned char r,unsigned char g,unsigned char b)
	  {
		    cr=r;
		    cg=g;
		    cb=b;
	  };
	  void clear();
	  unsigned char* get_image()
	  {
		    buffered+=refresh_buffer;
		    return board;
	  };
private:
	  GLuint last_texture;
	  int line_byte;
	  unsigned int buffered=0;
	  bool exist=false;
	  unsigned char*board;
};

#endif // PAINT_BOARD_H
