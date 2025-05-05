#include "Npuzzle.hpp"

int main(int argc, char *argv[])
{
    auto    start = std::chrono::high_resolution_clock::now();

    //  initialize puzzle class
    Npuzzle npuzzle;

    if (npuzzle.npuzzle_parsing(argc, argv, npuzzle))
        return (1);

    // check if map is solvable
    if (!npuzzle.get_map_generated() && !npuzzle.is_solvable())
    {
        std::cerr << "Error: map is unsolvable" << std::endl;
        return (1);
    }

    if (npuzzle._heuristic_func == npuzzle.get_pattern_database_heuristic_value && npuzzle.database.algo(npuzzle.get_size()))
    {
            std::cerr << "ERR create pattern database problem" << std::endl;
            return (1);
    }

    // if (npuzzle.a_star_algorithm(npuzzle.get_pattern_database_heuristic_value))
    if (npuzzle.a_star_algorithm())
    {
        std::cerr << "ERR a_star problem" << std::endl;
        return (1);
    }

    auto    end = std::chrono::high_resolution_clock::now();
    std::cout << "time : " << static_cast<std::chrono::duration<double>>(end - start).count() << " seconds" << std::endl;
}