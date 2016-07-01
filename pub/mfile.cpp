#include "mfile.h"
#include"../base/interfaces.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>

mfile::mfile(string pa,bool re)
{
      if(pa[pa.length()-1]=='/')
            pa=pa.substr(0,pa.length()-1);
      path=pa;
      recursion=re;
      open_dir(path);
}

bool mfile::open_dir(string pa)
{
      DIR *dp;
      struct dirent *entry;
      struct stat statbuf;

      if ((dp = opendir(pa.c_str())) == NULL)
      {
            my_print.error(pa+" open failed!");
            return false;
      }

      while ((entry = readdir(dp)) != NULL)
      {
            string dna=entry->d_name;
            string full_path=pa+"/"+dna;
            lstat(full_path.c_str(), &statbuf);
            mfi tfi;
            tfi.path=pa+"/";
            tfi.name=dna;
            if (S_ISDIR(statbuf.st_mode))
            {
                  if (dna!="."&&dna!="..")
                  {
                        tfi.isdir=true;
                        f_vec.push_back(tfi);
                        if(recursion)
                              open_dir(pa+"/"+dna);
                  }
            }
            else
            {
                  tfi.isdir=false;
                  f_vec.push_back(tfi);
            }
      }

      closedir(dp);
      return true;
}
