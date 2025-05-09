#include "Npuzzle.hpp"

Npuzzle::Npuzzle() : _time_complexity(0), _map_generated(0), _map_finished(0)
{
    return;
}

Npuzzle::~Npuzzle()
{
    //  delete all movements
    for (std::unordered_set<t_movement*, hash_movement, cmp_movement>::iterator it = all_movements.begin(); it != all_movements.end(); it++)
        delete (*it);

    // delete all duplicates movements
    while (duplicates.size())
    {
        delete duplicates.front();
        duplicates.pop();
    }
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

int     Npuzzle::get_time_complexity(void)
{
    return (_time_complexity);
}

void    Npuzzle::incr_time_complexity(void)
{
    _time_complexity++;
}

bool    Npuzzle::get_map_generated(void)
{
    return(_map_generated);
}

void    Npuzzle::set_map_generated(void)
{
    _map_generated++;
}

bool    Npuzzle::get_map_finished(void)
{
    return(_map_finished);
}

void    Npuzzle::set_map_finished(void)
{
    _map_finished++;
}

bool    Npuzzle::is_solvable(void)
{
    // get map numbers in one vector
    std::vector<int> values;
    for (int i = 0; i < _size; i++)
    {
        for (int j = 0; j < _size; j++)
        {
            values.push_back(map[i][j]);
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
    // if N is odd,then puzzle instance is solvable if number of inversions is even in the input state
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
            if (map[i][j] == 0)
                return (0);
            j++;
        }
        i++;
    }
    std::cerr << "Map has no hole !" << std::endl;
    return (1);
}

int     Npuzzle::a_star_algorithm(void)
{
    //  initialize "movement" begining
    t_movement  *beginning;
    try
    {
        beginning = new t_movement;
    }
    catch(const std::bad_alloc& badd_alloc)
    {
        std::cerr << "bad_alloc caught : " << badd_alloc.what() << std::endl;
        return (1);
    }
    
    beginning->value = heuristic_func(map, *this);
    beginning->direction = BEGIN;
    beginning->cost = 0;
    if (get_map_blank(beginning->blank.i, beginning->blank.j))
        return (1);
    beginning->map = map;
    beginning->previous = NULL;


    possibilities.push(beginning);
    all_movements.insert(beginning);


    //  check if map already complete
    if (finished(heuristic_func(map, *this), beginning))
        return(0);

    while (!get_map_finished())
    {
        if (a_star_algorithm_recusrsive(possibilities.top()))
            return(0);
    }

    return (0);
}

int     Npuzzle::a_star_algorithm_recusrsive(t_movement *movement)
    {
    //remove front member before expanding
    possibilities.pop();
    incr_time_complexity();

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

    return(0);
}

int    Npuzzle::add_possibility(t_movement *parent_movement, int direction)
{
    //  create new movement base on parent and direction
    t_movement  *movement;
    try
    {
        movement = new t_movement;
    }
    catch(const std::bad_alloc& bad_alloc)
    {
        std::cerr << "bad_alloc caught : " << bad_alloc.what() << std::endl;
        return (1);
    }
    movement->cost = parent_movement->cost + 1;
    movement->direction = direction;
    movement_assign_map_and_blank(movement, parent_movement);
    movement->previous = parent_movement;

    int heuristic = heuristic_func(movement->map, *this);
    movement->value = heuristic + movement->cost;


    //  check if not already in all_movements to get points in the correction even if it slower to do
    auto match = all_movements.find(movement);
    if (match != all_movements.end())
    {
        if (movement->value >= (*match)->value)
        {
            delete movement;
            return (0);
        }
        else
        {
            duplicates.push(*match);
            all_movements.erase(*match);
        }
    }

    possibilities.push(movement);
    all_movements.insert(movement);


    //  check if npuzzle finished
    if (finished(heuristic, movement))
        return(1);

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
    std::swap(movement->map[parent_movement->blank.i][parent_movement->blank.j], movement->map[movement->blank.i][movement->blank.j]); 
}

int Npuzzle::finished(int heuristic, t_movement *movement)
{
    if (heuristic == 0)
    {
        
        std::cout << "complexity in time : " << get_time_complexity() << std::endl;
        std::cout << "complexity in size : " << possibilities.size() + all_movements.size() + duplicates.size() << std::endl;
        std::cout << "solution : ";
        print_solution_movement(movement);
        std::cout << std::endl;
        std::cout << "npuzzle finish in : " << movement->cost << " moves" << std::endl;
        set_map_finished();
        return (1);
    }
    return(0);
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

int Npuzzle::get_Manhattan_heuristic_value(std::vector< std::vector<int> > &map, Npuzzle &npuzzle)
{
    (void)npuzzle;
    int to_check = 0;
    int h_value = 0;
    int x, y;
    int size = static_cast<int>(map.size());
    for (int i = 0; i < size - 1; i++)
    {
        for (int j = 0; j < size; j++)
        {
            to_check++;
            set_coordinates(to_check, x, y, map);
            h_value += abs(i - x) + abs(j - y);
        }
    }

    //  check last line without checking the blank piece
    int i = size - 1;
    for (int j = 0; j < size - 1; j++)
    {
        to_check++;
        set_coordinates(to_check, x, y, map);
        h_value += abs(i - x) + abs(j - y);
    }
    return (h_value);
}

int Npuzzle::get_Manhattan_heuristic_and_linear_conflict_value(std::vector< std::vector<int> > &map, Npuzzle &npuzzle)
{
    return (get_Manhattan_heuristic_value(map, npuzzle) + (get_linear_conflicts_value(map) * 2));
}


int count_conflicts(std::vector<int> goals, std::vector<int> values)
{
    int conflicts = 0;
    std::vector<int> to_check;

    for (size_t i = 0; i < values.size(); i++)
    {
        if (std::find(goals.begin(), goals.end(), values[i]) != goals.end())
            to_check.push_back(values[i]);
    }

    for (size_t i = 0; i < to_check.size(); i++)
    {
        for (size_t j = i + 1; j < to_check.size(); j++)
        {
            if (to_check[i] > to_check[j])
                conflicts++;
        }
    }

    return (conflicts);
}

int Npuzzle::get_linear_conflicts_value(std::vector< std::vector<int> > &map)
{
    int conflicts = 0;
    int size = static_cast<int>(map.size());

    std::vector<int> goals;
    std::vector<int> values;

    // get conflicts for each row
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            values.push_back(map[i][j]);
            goals.push_back(size * i + j + 1);
        }
        conflicts += count_conflicts(goals, values);
        values.clear();
        goals.clear();
    }

    // get conflicts for each column
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            values.push_back(map[j][i]);
            goals.push_back(size * j + i + 1);
        }
        conflicts += count_conflicts(goals, values);
        values.clear();
        goals.clear();
    }

    return (conflicts);
}

int Npuzzle::get_pattern_database_heuristic_value(std::vector< std::vector<int> > &map, Npuzzle &npuzzle)
{
    int heuristic = 0;

    for (size_t i = 0; i < npuzzle.database.databases_map.size(); i++)
    {
        auto it = npuzzle.database.databases_map[i].find(npuzzle.database.map_to_string(map, i));
        if (it == npuzzle.database.databases_map[i].end())
        {
            std::cerr << "Error database altered" << std::endl;
            return(1);
        }

        heuristic += it->second;
    }

    return (heuristic);
}

void Npuzzle::set_coordinates(int to_check, int &x, int &y, std::vector< std::vector<int> > &map)
{
    int size = static_cast<int>(map.size());
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            if (to_check == map[i][j])
            {
                x = i;
                y = j;
            }
        }
    }
}

int Npuzzle::get_Misplaced_tiles_value(std::vector< std::vector<int> > &map, Npuzzle &npuzzle)
{
    (void)npuzzle;
    int h_value = 0;
    int number = 1;
    int size = static_cast<int>(map.size());
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            if (number != map[i][j] && number != size * size)
            {
                h_value++;
            }
            number++;
        }
    }
    return (h_value);
}
