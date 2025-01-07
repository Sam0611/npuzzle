#include "Npuzzle.hpp"
#include <stdlib.h> // abs



int main(int argc, char *argv[])
{
    //  initialize puzzle class
    Npuzzle npuzzle;

    if (npuzzle_parsing(argc, argv, npuzzle))
        return(1);

    std::cout << "h = " << npuzzle.get_Manhattan_heuristic_value() << std::endl;

}