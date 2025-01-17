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
