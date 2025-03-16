#include "Npuzzle.hpp"

Npuzzle::Npuzzle()
{

}

Npuzzle::~Npuzzle()
{
    //  delete all movements
    for (std::forward_list<t_movement*>::iterator it = all_movements.begin(); it != all_movements.end(); it++)
        delete (*it);
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

bool    Npuzzle::is_solvable(void)
{
    // get map numbers in one vector
    std::vector<int> values;
    for (int i = 0; i < _size; i++)
    {
        for (int j = 0; j < _size; j++)
        {
            values.push_back(_map[i][j].nbr);
        }
    }

    // get number of inversions
    // a pair of tiles (a, b) form an inversion if a appears before b but a > b
    int inversions = 0;
    for (std::vector<int>::size_type i = 0; i < values.size(); i++)
    {
        for (std::vector<int>::size_type j = i + 1; j < values.size(); j++)
        {
            if (values[j] != 0 && values[i] > values[j])
                inversions++;
        }
    }

    // if N is even, puzzle instance is solvable if
    //  the blank is on odd row and number of inversions is odd
    //  the blank is on even row and number of inversions is even
    // if N is odd,then puzzle instance is solvable if number of inversions is odd in the input state
    if (_size % 2 == 0) // N is even
    {
        int row, col;
        get_map_blank(row, col);
        if (row % 2 == 0 && inversions % 2 != 0)
            return (true);
        if (row % 2 != 0 && inversions % 2 == 0)
            return (true);
    }
    else // N is odd
    {
        if (inversions % 2 == 0)
            return (true);
    }
    return (false);
}

int    Npuzzle::get_map_blank(int &i, int &j)
{
    i = 0;

    while (i < _size)
    {
        j = 0;
        while (j < _size)
        {
            if (_map[i][j].nbr == 0)
                return  (0);
            j++;
        }
        i++;
    }
    std::cerr << "Map has no hole !" << std::endl;
    return(1);
}

int     Npuzzle::a_star_algorithm(void)
{
    //  check if map already complete
    if (get_Manhattan_heuristic_value(_map) == 0)
    {
        std::cout << "npuzzle finish in : 0" << std::endl;
        return (0);
    }

    //  initialize "movement" begining
    t_movement  *beginning;
    try
    {
        beginning = new t_movement;
    }
    catch(const std::bad_alloc& ba)
    {
        std::cerr << "bad_alloc caught : " << ba.what() << std::endl;
        return (1);
    }
    
    beginning->value = 0;
    beginning->direction = BEGIN;
    beginning->cost = 0;
    if (get_map_blank(beginning->blank.i, beginning->blank.j))
        return (1);
    beginning->map = _map;
    beginning->previous = NULL;

    //  add in lists
    possibilities.push_front(beginning);
    all_movements.push_front(beginning);


    a_star_algorithm_recusrsive(beginning);

    return (0);
}

int     Npuzzle::a_star_algorithm_recusrsive(t_movement *movement)
{
//test
// auto it = possibilities.begin();
// while (it != possibilities.end())
// {
//     std::cout << (*it)->value << " ";
//     it++;
// }
// std::cout << std::endl;
// std::cout << std::endl;
// std::cout << std::endl;


    //remove front member before expanding
    possibilities.pop_front();

    //  can go UP
    if (movement->direction != DOWN && movement->blank.i > 0)
    {
        if (add_possibility(movement, UP))
            return(1);
    }

    //  can go DOWN
    if (movement->direction != UP && movement->blank.i < _size - 1)
    {
        if (add_possibility(movement, DOWN))
            return (1);
    }

    //  can go LEFT
    if (movement->direction != RIGHT && movement->blank.j > 0)
    {
        if (add_possibility(movement, LEFT))
            return(1);
    }

    //  can go RIGHT
    if (movement->direction != LEFT && movement->blank.j < _size - 1)
    {
        if (add_possibility(movement, RIGHT))
            return(1);
    }

    //  launch with the smaller value
    return(a_star_algorithm_recusrsive(possibilities.front()));
}

int    Npuzzle::add_possibility(t_movement *parent_movement, int direction)
{
    //  create new movement base on parent and direction
    t_movement  *movement;
    try
    {
        movement = new t_movement;
    }
    catch(const std::bad_alloc& ba)
    {
        std::cerr << "bad_alloc caught : " << ba.what() << std::endl;
        return (1);
    }

    movement->cost = parent_movement->cost + 1;
    movement->direction = direction;
    movement_assign_map_and_blank(movement, parent_movement);
    movement->previous = parent_movement;

    int heuristic = get_Manhattan_heuristic_value(movement->map);
    movement->value = heuristic + movement->cost;


    //  add in lists
    std::list<t_movement*>::iterator it = possibilities.begin();
    while (it != possibilities.end() && (movement->value > (*it)->value || (movement->value == (*it)->value && movement->cost < (*it)->cost)))
        it++;
    possibilities.insert(it, movement);
    all_movements.push_front(movement);

    //  check if npuzzle finished
    if (heuristic == 0)
    {
        std::cout << "solution : ";
        print_solution_movement(movement);
        std::cout << std::endl;
        std::cout << "npuzzle finish in : " << movement->cost << std::endl;
        return (1);
    }

    return(0);
}

void    Npuzzle::movement_assign_map_and_blank(t_movement *movement, t_movement *parent_movement)
{
    if (movement->direction == UP)
    {
        movement->blank.i = parent_movement->blank.i - 1;
        movement->blank.j = parent_movement->blank.j;
    }
    else if (movement->direction == DOWN)
    {
        movement->blank.i = parent_movement->blank.i + 1;
        movement->blank.j = parent_movement->blank.j;
    }
    else if (movement->direction == LEFT)
    {
        movement->blank.i = parent_movement->blank.i;
        movement->blank.j = parent_movement->blank.j - 1;
    }
    else if (movement->direction == RIGHT)
    {
        movement->blank.i = parent_movement->blank.i;
        movement->blank.j = parent_movement->blank.j + 1;
    }
    movement->map = parent_movement->map;
    std::swap(movement->map[parent_movement->blank.i][parent_movement->blank.j].nbr, movement->map[movement->blank.i][movement->blank.j].nbr); 
}

void    Npuzzle::print_solution_movement(t_movement *movement)
{
    if (movement->previous)
        print_solution_movement(movement->previous);

    if (movement->direction == UP)
        std::cout << "↑ ";
    if (movement->direction == DOWN)
        std::cout << "↓ ";
    if (movement->direction == LEFT)
        std::cout << "← ";
    if (movement->direction == RIGHT)
        std::cout << "→ ";
}

int Npuzzle::get_Manhattan_heuristic_value(std::vector< std::vector<t_piece> > map)
{
    int to_check = 0;
    int h_value = 0;
    int x, y;
    for (int i = 0; i < _size - 1; i++)
    {
        for (int j = 0; j < _size; j++)
        {
            to_check++;
            set_coordinates(to_check, x, y, map);
            h_value += abs(i - x) + abs(j - y);
        }
    }

    //  check last line without checking the blank piece
    int i = _size - 1;
    for (int j = 0; j < _size - 1; j++)
    {
        to_check++;
        set_coordinates(to_check, x, y, map);
        h_value += abs(i - x) + abs(j - y);
    }
    return (h_value);
}

int Npuzzle::get_linear_conflicts_value(std::vector< std::vector<t_piece> > map)
{
    int conflicts = 0;

    // get conflict for each row
    for (int i = 0; i < _size; i++)
    {
        for (int j = 0; j < _size; j++)
        {
            if (map[i][j].nbr > map[i][j + 1])
                conflicts++;
        }
    }

    // get conflict for each column
    for (int i = 0; i < _size; i++)
    {
        for (int j = 0; j < _size; j++)
        {
            if (map[j][i].nbr > map[j][i + 1])
                conflicts++;
        }
    }

    return (conflicts);
}

void Npuzzle::set_coordinates(int to_check, int &x, int &y, std::vector< std::vector<t_piece> > map)
{
    for (int i = 0; i < _size; i++)
    {
        for (int j = 0; j < _size; j++)
        {
            if (to_check == map[i][j].nbr)
            {
                x = i;
                y = j;
            }
        }
    }
}

int Npuzzle::get_Misplaced_tiles_value(std::vector< std::vector<t_piece> > map)
{
    int h_value = 0;
    int number = 1;
    for (int i = 0; i < _size; i++)
    {
        for (int j = 0; j < _size; j++)
        {
            if (number != map[i][j].nbr && number != _size * _size)
            {
                h_value++;
            }
            number++;
        }
    }
    return (h_value);
}
