#include "Npuzzle.hpp"
#include <chrono>

int main(int argc, char *argv[])
{
    //  initialize puzzle class
    Npuzzle npuzzle;

    if (npuzzle_parsing(argc, argv, npuzzle))
        return(1);

    //  test a star algo
clock_t    start = std::clock();
    if (npuzzle.a_star_algorithm())
    {
        std::cerr << "ERR a_star probleme" << std::endl;
        return(1);
    }
std::clock_t    end = std::clock();
std::cout << "start = " << start << std::endl;
std::cout << "end = " << end << std::endl;
std::cout << "end - start = " << end - start << std::endl;
}