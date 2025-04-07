#include "Npuzzle.hpp"
#include <chrono>

int main(int argc, char *argv[])
{
    //  initialize puzzle class
    Npuzzle npuzzle;

    if (npuzzle_parsing(argc, argv, npuzzle))
        return (1);

    // check if map is solvable
    if (!npuzzle.is_solvable())
    {
        std::cerr << "Error: map is unsolvable" << std::endl;
        return (1);
    }

    //  test a star algo
clock_t    start = std::clock();
    if (npuzzle.a_star_algorithm(&npuzzle.get_Manhattan_heuristic_and_linear_conflict_value))
    {
        std::cerr << "ERR a_star probleme" << std::endl;
        return (1);
    }
std::clock_t    end = std::clock();
std::cout << "time : " << end - start << " ms" << std::endl;
}