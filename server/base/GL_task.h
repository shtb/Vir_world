#pragma once

#include<iostream>
#include<string>

using namespace std;

class GL_task_part
{
public:
    float* contrast;
    int part_order;
    void init(int con_num, int order)
    {
        contrast = new float[con_num + 1];
        part_order = order;
    }
};

class GL_task
{
public:
    bool play_on = 0;
    double state = 0;

    int task_part_num = 0;
    GL_task_part* task_part;
    bool go_back = 0;
    double limit = 1;
    double step = 0;
    int real = 0;
    bool direction = 0;
    bool always_on = 0;
    bool limit_rand = 0;
    bool auto_run = 1;
    int part_postion = 0;
    void init(int num, bool back, int re, bool always, bool rand, double ste)
    {
        auto_run = true;
        go_back = back;
        task_part_num = num;
        task_part = new GL_task_part[num];
        real = re;
        always_on = always;
        limit_rand = rand;
        step = ste;
    }
    void init(int num)
    {
        auto_run = false;
        real = -1;
        task_part_num = num;
        task_part = new GL_task_part[num];
    }
    
    void move()
    {
        if (play_on&&auto_run)
        {
            if (!direction)
            {
                if (state < 1)
                {
                    state += step;
                }
                else
                {
                    if (go_back)
                    {
                        direction = 1;
                    }
                }
            }
            else
            {
                if (state > 0)
                {
                    state -= step;
                }
                else
                {
                    reset();
                    if (!always_on)
                    {
                        play_on = 0;
                    }
                    else
                    {
                        if (limit_rand)
                        {
                            int ran = rand() % 1000;
                            limit = (double)ran / 1000.0;
                        }
                    }
                }
            }
        }
    }

    void start()
    {
        reset();
        play_on = 1;
        move();
    }
    
    void reset()
    {
        direction = 0;
        state = 0;
    }
    
    void set_state(double sta, bool dir)
    {
        play_on = 1;
        state = sta;
        direction = dir;
    }
};

