#include "Database.hpp"

Database::Database(void)
{
    return;
}

Database::~Database(void)
{
    //free the nodes
    for (auto it = closed_list.begin(); it != closed_list.end(); ++it)
        delete (*it);
    return;
}

int Database::algo(Npuzzle npuzzle)
{
    if (define_patterns(npuzzle))
        return(1);

    for (size_t i = 0; i < patterns.size(); i++)
    {
        //testsuppr
        //free the nodes
        if (!closed_list.empty())
        {
            pattern_database.clear();
            for (auto it = closed_list.begin(); it != closed_list.end(); ++it)
                delete (*it);
            closed_list.clear();
        }


        //  initialize node
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
        
        //testsuppr
        initialize_map(node->map, npuzzle, patterns[i]);
        print_map(node->map);
        node->blank.i = npuzzle.get_size() - 1;
        node->blank.j = npuzzle.get_size() - 1;
        node->cost = 0;
        queue.push(node);
        
        
        while(queue.size() > 0)
        {
            if (algo_iterative(npuzzle))
            return (1);
        }
        
        
        std::cout << "closed_list size (final) = " << closed_list.size() << std::endl;
        if (create_pattern_database(i))
            return (1);
    }
        
    return(0);
}
    
int Database::define_patterns(Npuzzle npuzzle)
{
    switch (npuzzle.get_size())
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
            std::vector<int>    pattern1 = {1, 2, 3, 4};
            std::vector<int>    pattern2 = {5, 6, 7, 8};
            patterns.push_back(pattern1);
            patterns.push_back(pattern2);
            break;
        }
        case 4:
        {
            // std::vector<int>    pattern1 = {1, 2, 3, 5, 6};
            // std::vector<int>    pattern2 = {4, 7, 8, 11, 12};
            // std::vector<int>    pattern3 = {9, 10, 13, 14, 15};
            // patterns.push_back(pattern1);
            // patterns.push_back(pattern2);
            // patterns.push_back(pattern3);
            std::vector<int>    pattern1 = {1, 2, 3, 5};
            std::vector<int>    pattern2 = {4, 7, 8, 12};
            std::vector<int>    pattern3 = {6, 10, 11, 15};
            std::vector<int>    pattern4 = {9, 13, 14};
            patterns.push_back(pattern1);
            patterns.push_back(pattern2);
            patterns.push_back(pattern3);
            patterns.push_back(pattern4);
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
        case 6:
        
        default:
        {
            std::cerr << "Pattern database heuristic not supported for puzzle of 6 or more" << std::endl;
            return (1);
        }
    }

    return(0);
}

void    Database::initialize_map(std::vector< std::vector<int> > &map, Npuzzle npuzzle, std::vector<int> &pattern)
{
    size_t  limit = npuzzle.get_size();
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

int Database::algo_iterative(Npuzzle npuzzle)
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
    if (node->blank.i > 0)
    {
        if (add_bfs(node, UP))
            return (1);
    }
    //  explore DOWN
    if (node->blank.i < npuzzle.get_size() - 1)
    {
        if (add_bfs(node, DOWN))
            return (1);
    }
    //  explore LEFT
    if (node->blank.j > 0)
    {
        if (add_bfs(node, LEFT))
            return (1);
    }
    //  explore RIGHT
    if (node->blank.j < npuzzle.get_size() - 1)
    {
        if (add_bfs(node, RIGHT))
            return (1);
    }
        
    return (0);
}
    
int Database::add_bfs(t_node *node, int direction)
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
    queue.push(new_node);
    
    return(0);
}

int Database::create_pattern_database(int number)
{
    // create patter_database from closed_list but without blank and with lower cost value
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


    std::cout << "pattern_database size = " << pattern_database.size() << std::endl;

    std::ofstream   fs;
    std::string     database_name = "database/database";
    database_name += std::to_string(number);
    database_name += ".txt";

    fs.open(database_name);
    if (!fs.good())
    {
        std::cerr << "error while creating database" << std::endl;
        return(1);
    }


    for (auto it = pattern_database.begin(); it != pattern_database.end(); ++it)
        fs << node_to_string(*it, number) << std::endl;

    fs.close();

    return(0);
}

std::string Database::node_to_string(t_node *node, int number)
{
    std::string         str;
    // std::vector<int>    targets = {1, 2, 3, 4, 5, 6};
    size_t              limit = node->map.size();

    //convert map
    for (size_t i = 0; i < patterns[number].size(); i++)
    {
        for (size_t y = 0; y < limit; y++)
        {
            for (size_t x = 0; x < limit; x++)
            {
                if (node->map[y][x] == patterns[number][i])
                {
                    str += std::to_string(y);
                    str += ',';
                    str += std::to_string(x);
                    str += ',';
                    x = limit;
                    y = limit;
                }
            }
        }
    }

    //convert score
    str += std::to_string(node->cost);
    
    return(str);
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
