#ifndef __NPUZZLE__
#define __NPUZZLE__

#include <iostream>
#include <string>
#include <fstream>
#include <cmath>
#include <climits>

class Npuzzle
{
    public:
        Npuzzle();
        ~Npuzzle();

        void    set_size(int size);
        int     get_size(void);

    private:
        int _size;
    
};

//  parsing functions
int    npuzzle_parsing(int argc, char **argv, Npuzzle &npuzzle);

#endif