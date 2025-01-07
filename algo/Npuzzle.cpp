#include "Npuzzle.hpp"

Npuzzle::Npuzzle()
{

}

Npuzzle::~Npuzzle()
{

}

void    Npuzzle::set_size(int size)
{
    _size = size;
    _max_piece = _size * _size - 1;
}


int     Npuzzle::get_size(void)
{
    return(_size);
}

int     Npuzzle::get_max_piece(void)
{
    return(_max_piece);
}

int Npuzzle::get_Manhattan_heuristic_value(void)
{
    int to_check = 0;
    int h_value = 0;
    int x, y;
    for (int i = 0; i < _size; i++)
    {
        for (int j = 0; j < _size; j++)
        {
            to_check++;
            if (to_check >= _max_piece)
                return (h_value);
            set_coordinates(to_check, x, y);
            h_value += abs(static_cast<int>(i) - x) + abs(static_cast<int>(j) - y);
        }
    }
    return (h_value);
}

void Npuzzle::set_coordinates(int to_check, int &x, int &y)
{
    for (int i = 0; i < _size; i++)
    {
        for (int j = 0; j < _size; j++)
        {
            if (to_check == _map[i][j].nbr)
            {
                x = i;
                y = j;
            }
        }
    }
}
