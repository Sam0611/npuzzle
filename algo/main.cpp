#include "Npuzzle.hpp"
#include <stdlib.h> // abs

void set_coordinates(std::vector< std::vector<t_piece> > map, int value, int &row, int &col)
{
    for (std::vector<int>::size_type i = 0; i < map.size(); i++)
    {
        for (std::vector<int>::size_type j = 0; j < map[i].size(); j++)
        {
            if (value == map[i][j].nbr)
            {
                row = i;
                col = j;
            }
        }
    }
}

int get_heuristic_value(std::vector< std::vector<t_piece> > map)
{
    int to_check = 0;
    int h_value = 0;
    int x, y;
    for (std::vector<int>::size_type i = 0; i < map.size(); i++)
    {
        for (std::vector<int>::size_type j = 0; j < map[i].size(); j++)
        {
            to_check++;
            if (to_check >= map.size() * map.size())
                return (h_value);
            set_coordinates(map, to_check, x, y);
            h_value += abs(static_cast<int>(i) - x) + abs(static_cast<int>(j) - y);
        }
    }
    return (h_value);
}

int main(int argc, char *argv[])
{
    //  initialize puzzle class
    Npuzzle npuzzle;

    if (npuzzle_parsing(argc, argv, npuzzle))
        return(1);

    std::cout << "h = " << get_heuristic_value(npuzzle._map) << std::endl;

}