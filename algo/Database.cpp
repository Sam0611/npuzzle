#include "Database.hpp"

Database::Database(void)
{
    return;
}

Database::~Database(void)
{
    //delete nodes in queue if no match in closed_list (unique)
    while (queue.size() > 0)
    {
        t_node *node = queue.front();

        if (closed_list.find(node) == closed_list.end())
            delete node;
        queue.pop();
    }

    //free closed_list
    for (auto it = closed_list.begin(); it != closed_list.end(); ++it)
        delete (*it);
    return;
}

void Database::set_npuzzle_size(int n)
{
    npuzzle_size = n;
}

int Database::get_npuzzle_size(void)
{
    return(npuzzle_size);
}

int Database::algo(int size)
{
    set_npuzzle_size(size);

    if (define_patterns())
        return(1);

        
    for (size_t i = 0; i < patterns.size(); i++)
    {
        if (create_pattern_database(i))
            return(1);
    }

    for (size_t i = 0; i < patterns.size(); i++)
    {
        if (fill_database_map(i))
            return(1);
    }
        
    return(0);
}
    
int Database::define_patterns(void)
{
    switch (get_npuzzle_size())
    {
        case 1:
        {
            std::cerr << "Not enough number to make a pattern databse" << std::endl;
            break;
        }
        case 2:
        {
            std::vector<int> pattern1 = {1, 2, 3};
            patterns.push_back(pattern1);
            break;
        }
        case 3:
        {
            std::vector<int>    pattern1 = {1, 2, 3, 4, 5, 6, 7, 8};
            patterns.push_back(pattern1);
            break;
        }
        case 4:
        {
            std::vector<int>    pattern1 = {1, 2, 3, 5, 6};
            std::vector<int>    pattern2 = {4, 7, 8, 11, 12};
            std::vector<int>    pattern3 = {9, 10, 13, 14, 15};
            patterns.push_back(pattern1);
            patterns.push_back(pattern2);
            patterns.push_back(pattern3);
            break;
        }
        case 5:
        {
            std::vector<int>    pattern1 = {1, 2, 3, 6, 7};
            std::vector<int>    pattern2 = {4, 5, 9, 10, 15};
            std::vector<int>    pattern3 = {8, 11, 12, 13};
            std::vector<int>    pattern4 = {16, 17, 21, 22, 23};
            std::vector<int>    pattern5 = {14, 18, 19, 20, 24};
            patterns.push_back(pattern1);
            patterns.push_back(pattern2);
            patterns.push_back(pattern3);
            patterns.push_back(pattern4);
            patterns.push_back(pattern5);
            break;
        }
        
        default:
        {
            std::cerr << "Pattern database heuristic not supported for puzzle of 6 or more" << std::endl;
            return (1);
        }
    }

    return(0);
}

int Database::create_pattern_database(int index)
{
    //check if database already made
    std::ifstream   fs;
    fs.open(get_database_name(index));
    if (fs.good())
    {
        fs.close();
        return(0);
    }
    fs.close();

    //  allocate node
    t_node  *node;
    try
    {
        node = new t_node;
    }
    catch(const std::bad_alloc& bad_alloc)
    {
        std::cerr << "bad_alloc caught " << bad_alloc.what() << std::endl;
        return(1);
    }
    
    //  initialize node
    initialize_map(node->map, patterns[index]);
    node->blank.i = get_npuzzle_size() - 1;
    node->blank.j = get_npuzzle_size() - 1;
    node->cost = 0;
    node->direction = BEGIN;
    queue.push(node);

    //testsuppr
    // std::cout << "sizeof(t_node) = " << sizeof(t_node) << std::endl;
    // std::cout << "sizeof(t_node *) = " << sizeof(t_node *) << std::endl;
    // std::cout << "sizeof(node) = " << sizeof(node) << std::endl;
    // std::cout << "sizeof(*node) = " << sizeof(*node) << std::endl;
    // std::cout << "sizeof(node->map) = " << sizeof(node->map) << std::endl;
    // std::cout << "sizeof(node->map[0]) = " << sizeof(node->map[0]) << std::endl;
    // std::cout << "sizeof(node->map[0][0]) = " << sizeof(node->map[0][0]) << std::endl;
    // std::cout << "sizeof(node->cost) = " << sizeof(node->cost) << std::endl;
    // std::cout << "sizeof(node->direction) = " << sizeof(node->direction) << std::endl;
    // std::cout << "sizeof(node->blank) = " << sizeof(node->blank) << std::endl;
    // std::cout << "sizeof(node->blank.i) = " << sizeof(node->blank.i) << std::endl;
    // std::cout << "sizeof(node->blank.j) = " << sizeof(node->blank.j) << std::endl;
    // return (1);
    
    
    while(queue.size() > 0)
    {
        if (bfs())
        return (1);
    }
    
    std::cout << "closed_list size (final) = " << closed_list.size() << std::endl;

    if (create_pattern_database_no_blank_tile(index))
        return (1);

    //free the nodes
    pattern_database.clear();
    for (auto it = closed_list.begin(); it != closed_list.end(); ++it)
        delete (*it);
    closed_list.clear();

    return(0);
}

void    Database::initialize_map(std::vector< std::vector<int> > &map, std::vector<int> &pattern)
{
    size_t  limit = get_npuzzle_size();
    size_t  k = 0;
    int     nbr = 1;

    for (size_t i = 0; i < limit; i++)
    {
        std::vector<int> line;
        for (size_t j = 0; j < limit; j++)
        {
            if (k < pattern.size() && pattern[k] == nbr)
            {
                line.push_back(pattern[k]);
                k++;
            }
            else
                line.push_back(-1);
            nbr++;
        }
        map.push_back(line);
    }
}

int Database::bfs()
{
    t_node *node = queue.front();
    queue.pop();
    
    //search if map already in closed_list
    if (closed_list.find(node) != closed_list.end())
    {
        delete node;
        return(0);
    }
    closed_list.insert(node);
    
    
    //  explore UP
    if (node->blank.i > 0 && node->direction != DOWN)
    {
        if (bfs_add_node(node, UP))
            return (1);
    }
    //  explore DOWN
    if (node->blank.i < get_npuzzle_size() - 1 && node->direction != UP)
    {
        if (bfs_add_node(node, DOWN))
            return (1);
    }
    //  explore LEFT
    if (node->blank.j > 0 && node->direction != RIGHT)
    {
        if (bfs_add_node(node, LEFT))
            return (1);
    }
    //  explore RIGHT
    if (node->blank.j < get_npuzzle_size() - 1 && node->direction != LEFT)
    {
        if (bfs_add_node(node, RIGHT))
            return (1);
    }
        
    return (0);
}
    
int Database::bfs_add_node(t_node *node, int direction)
{
    t_node *new_node;
    try
    {
        new_node = new t_node;
    }
    catch(const std::exception& bad_alloc)
    {
        std::cerr << "bad_alloc caught " << bad_alloc.what() << std::endl;
        return(1);
    }

    switch (direction)
    {
    case UP:
        new_node->blank.i = node->blank.i - 1;
        new_node->blank.j = node->blank.j;
        break;
    case DOWN:
        new_node->blank.i = node->blank.i + 1;
        new_node->blank.j = node->blank.j;
        break;
    case LEFT:
        new_node->blank.i = node->blank.i;
        new_node->blank.j = node->blank.j - 1;
        break;
    case RIGHT:
        new_node->blank.i = node->blank.i;
        new_node->blank.j = node->blank.j + 1;
        break;
    }
    new_node->map = node->map;
    std::swap(new_node->map[node->blank.i][node->blank.j], new_node->map[new_node->blank.i][new_node->blank.j]);
    new_node->cost = node->cost + 1;
    new_node->direction = direction;
    queue.push(new_node);
    
    return(0);
}

int Database::create_pattern_database_no_blank_tile(int index)
{
    // create patter_database from closed_list without blank for space optimisation
    t_node  *node; 
    for (auto it = closed_list.begin(); it != closed_list.end(); ++it)
    {
        node = *it;
        auto    match = pattern_database.find(node);
        
        if (match != pattern_database.end())
        {
            if ((*it)->cost < (*match)->cost)
                (*match)->cost = (*it)->cost;
        }
        else
            pattern_database.insert(node);
    }


    std::cout << "pattern_database no blank size = " << pattern_database.size() << std::endl;

    std::ofstream   fs;

    fs.open(get_database_name(index));
    if (!fs.good())
    {
        std::cerr << "error while creating database" << std::endl;
        fs.close();
        return(1);
    }


    for (auto it = pattern_database.begin(); it != pattern_database.end(); ++it)
        fs << node_to_string(*it, index) << std::endl;

    fs.close();

    return(0);
}

std::string Database::node_to_string(t_node *node, int index)
{
    std::string str;

    //convert map
    str += map_to_string(node->map, index);

    //convert score
    str += std::to_string(node->cost);
    
    return(str);
}

std::string Database::map_to_string(std::vector< std::vector<int> > &map, int index)
{
    std::string         str;
    size_t              limit = map.size();

    for (size_t i = 0; i < patterns[index].size(); i++)
    {
        for (size_t y = 0; y < limit; y++)
        {
            for (size_t x = 0; x < limit; x++)
            {
                if (map[y][x] == patterns[index][i])
                {
                    str += std::to_string(y);
                    str += std::to_string(x);
                    x = limit;
                    y = limit;
                }
            }
        }
    }
    return(str);
}

int Database::fill_database_map(int index)
{
    std::ifstream   fs;

    fs.open(get_database_name(index));
    if (!fs.good())
    {
        std::cerr << "error while filing databases map" << std::endl;
        fs.close();
        return(1);
    }

    //add current map pattern database to vector "database"
    std::unordered_map<std::string, int>    database_map;
    databases_map.push_back(database_map);

    std::string buffer;
    int         number_of_tiles = patterns[index].size() * 2;

    getline(fs, buffer);
    while(fs.good())
    {
        std::string str_pattern = buffer.substr(0, number_of_tiles);
        int         cost        = std::stoi(buffer.substr(number_of_tiles, buffer.size() - number_of_tiles));

        databases_map[index].emplace(str_pattern, cost);

        getline(fs, buffer);
    }
    fs.close();
    return (0);
}

std::string Database::get_database_name(int index)
{
    std::string     database_name = "database/database";

    database_name += std::to_string(get_npuzzle_size());
    database_name += "-";
    database_name += std::to_string(index);
    database_name += ".txt";
    return(database_name);
}

void    Database::print_map(std::vector< std::vector<int> > map)
{
    for (size_t i = 0; i < map.size(); i++)
    {
        for (size_t j = 0; j < map[i].size(); j++)
            std::cout << map[i][j] << " ";
        std::cout << std::endl;
    }
}
