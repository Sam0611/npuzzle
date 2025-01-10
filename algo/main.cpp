#include "Npuzzle.hpp"
#include <stdlib.h> // abs



int main(int argc, char *argv[])
{
    //  initialize puzzle class
    Npuzzle npuzzle;

    if (npuzzle_parsing(argc, argv, npuzzle))
        return(1);

    //  test a star algo
    if (npuzzle.a_star_algorithm())
    {
        std::cerr << "ERR a_star probleme" << std::endl;
        return(1);
    }
}