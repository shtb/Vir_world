#include "gra_tools.h"
#include "interfaces.h"


//0:nothing;      1:above;        2:below;
GLuint tool_load_texture(string file_name,int reshape)
{
        GLint width, height, total_bytes;
        GLubyte* pixels = 0;
        GLuint texture_ID = 0;

        ifstream pFile(file_name, ios::in | ios::binary);
        if(!pFile)
        {
                pFile.close();
                return 0;
        }

        pFile.seekg(0x0012);
        pFile.read((char*)&width, 4);
        pFile.read((char*)&height, 4);
        pFile.seekg(54);

        GLint line_bytes;
        {
                line_bytes = width * 3;
                while(line_bytes % 4 != 0)
                        ++line_bytes;
                total_bytes = line_bytes * height;
        }
        pixels = new GLubyte[total_bytes];

        if(!pFile.read((char*)pixels, total_bytes))
        {
                my_print.error("bmp read failed!!!");
                delete[] pixels;
                pFile.close();
                return -1;
        }
        if(reshape==1)
        {
                if(height!=width)
                {
                        my_print.warning("the height and width of "+file_name+"is not equal!");
                }
                GLubyte* oripixels = new GLubyte[total_bytes];
                memcpy(oripixels,pixels,total_bytes);

                for (int i = height/2; i < height; ++i)
                {
                        int right = i + 1;
                        int w = height - i - 1;
                        while (w < right)
                        {
                                pixels[i*line_bytes + 3 * w] = oripixels[line_bytes*i + 3 * (width - w - 1)];
                                pixels[i*line_bytes + 3 * w + 1] = oripixels[line_bytes*i + 3 * (width - w - 1) + 1];
                                pixels[i*line_bytes + 3 * w + 2] = oripixels[line_bytes*i + 3 * (width - w - 1) + 2];
                                w++;
                        }
                }
                for (int i = 0; i < height/2; ++i)
                {
                        int right = height - i - 1;
                        int w = i;
                        while (w < right)
                        {
                                pixels[i*line_bytes + 3 * w] = oripixels[line_bytes*(height - i - 1) + 3 * w];
                                pixels[i*line_bytes + 3 * w+1] = oripixels[line_bytes*(height - i - 1) + 3 * w+1];
                                pixels[i*line_bytes + 3 * w+2] = oripixels[line_bytes*(height - i - 1) + 3 * w+2];
                                w++;
                        }
                }
                for (int i = 0; i < height; ++i)
                {
                        int right = height / 2 - abs(i - height / 2);
                        int w = 0;
                        while (w < right)
                        {
                                pixels[i*line_bytes + 3 * w] = oripixels[line_bytes*(height - w - 1) + 3 * i];
                                pixels[i*line_bytes + 3 * w + 1] = oripixels[line_bytes*(height - w - 1) + 3 * i + 1];
                                pixels[i*line_bytes + 3 * w + 2] = oripixels[line_bytes*(height - w - 1) + 3 * i + 2];
                                w++;
                        }
                }
                for (int i = 0; i < height; ++i)
                {
                        int w = width - (height / 2 - abs(i - height / 2)) - 1;
                        while (w < width)
                        {
                                pixels[i*line_bytes + 3 * w] = oripixels[line_bytes* w  + 3 * (width - i - 1)];
                                pixels[i*line_bytes + 3 * w+1] = oripixels[line_bytes* w  + 3 * (width - i - 1)+1];
                                pixels[i*line_bytes + 3 * w+2] = oripixels[line_bytes* w  + 3 * (width - i - 1)+2];
                                w++;
                        }
                }
                delete[] oripixels;
        }
        else if(reshape==2)
        {
                if(height!=width)
                {
                        my_print.warning("the height and width of "+file_name+"is not equal!");
                }
                GLubyte* oripixels = new GLubyte[total_bytes];
                memcpy(oripixels,pixels,total_bytes);

                for (int i = height / 2; i < height; ++i)
                {
                        int right = i + 1;
                        int w = height - i - 1;
                        while (w < right)
                        {
                                pixels[i*line_bytes + 3 * w] = oripixels[line_bytes*(height-i-1) + 3 * (w)];
                                pixels[i*line_bytes + 3 * w + 1] = oripixels[line_bytes*(height-i-1) + 3 * (w) + 1];
                                pixels[i*line_bytes + 3 * w + 2] = oripixels[line_bytes*(height-i-1) + 3 * (w) + 2];
                                w++;
                        }
                }
                for (int i = 0; i < height / 2; ++i)
                {
                        int right = height - i - 1;
                        int w = i;
                        while (w < right)
                        {
                                pixels[i*line_bytes + 3 * w] = oripixels[line_bytes*i + 3 * (width - w - 1)];
                                pixels[i*line_bytes + 3 * w + 1] = oripixels[line_bytes*i + 3 * (width - w - 1) + 1];
                                pixels[i*line_bytes + 3 * w + 2] = oripixels[line_bytes*i + 3 * (width - w - 1) + 2];
                                w++;
                        }
                }
                for (int i = 0; i < height; ++i)
                {
                        int right = height / 2 - abs(i - height / 2);
                        int w = 0;
                        while (w < right)
                        {
                                pixels[i*line_bytes + 3 * w] = oripixels[line_bytes*(w) + 3 *(width-i-1)];
                                pixels[i*line_bytes + 3 * w + 1] = oripixels[line_bytes*(w) + 3 *(width-i-1) + 1];
                                pixels[i*line_bytes + 3 * w + 2] = oripixels[line_bytes*(w) + 3 *(width-i-1) + 2];
                                w++;
                        }
                }
                for (int i = 0; i < height; ++i)
                {
                        int w = width - (height / 2 - abs(i - height / 2)) - 1;
                        while (w < width)
                        {
                                pixels[i*line_bytes + 3 * w] = oripixels[line_bytes*(height-w-1)+ 3 * (i)];
                                pixels[i*line_bytes + 3 * w + 1] = oripixels[line_bytes*(height-w-1)+ 3 * (i) + 1];
                                pixels[i*line_bytes + 3 * w + 2] = oripixels[line_bytes*(height-w-1)+ 3 * (i) + 2];
                                w++;
                        }
                }
                delete[] oripixels;
        }
        glGenTextures(1, &texture_ID);
        if(texture_ID == 0)
        {
                my_print.error("texture number not get!!!");
                delete[] pixels;
                pFile.close();
                return -2;
        }
        else
        {
                // glGetIntegerv(GL_TEXTURE_BINDING_2D, (GLint*)&last_texture_ID);
                glBindTexture(GL_TEXTURE_2D, texture_ID);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                // glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, pixels);
                // glBindTexture(GL_TEXTURE_2D, last_texture_ID);
        }

        delete[] pixels;
        pFile.close();
        return texture_ID;
}
