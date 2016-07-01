#ifndef MFILE_H
#define MFILE_H

#include<string>
#include<iostream>
#include<vector>

using namespace std;

struct mfi
{
      string name;
      string path;
      bool isdir;
};

class mfile
{
public:
      vector<mfi>f_vec;
      string path;
      bool recursion;
      mfile(string pa,bool re);
private:
      bool open_dir(string pa);
};

#endif // MFILE_H
