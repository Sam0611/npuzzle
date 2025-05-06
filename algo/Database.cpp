#include "Database.hpp"

Database::Database(void)
{
    return;
}

Database::~Database(void)
{
    for (size_t i = 0; i < patterns.size(); i++)
    {
        //delete nodes in queue if no match in closed_list (unique)
        while (queue[i].size() > 0)
        {
            t_node *node = queue[i].front();
    
            if (closed_list[i].find(node) == closed_list[i].end())
                delete node;
            queue[i].pop();
        }
    
        //free closed_list
        for (auto it = closed_list[i].begin(); it != closed_list[i].end(); ++it)
            delete (*it);
        return;
    }
}

void Database::set_npuzzle_size(int n)
{
    _npuzzle_size = n;
}

int Database::get_npuzzle_size(void)
{
    return(_npuzzle_size);
}

int Database::algo(int size)
{
    set_npuzzle_size(size);

    if (define_patterns())
        return(1);

    for (size_t i = 0; i < patterns.size(); i++)
    {
        std::queue<t_node *>                                                        queue_temp;
        std::unordered_set <t_node*, hash_closed_list, cmp_closed_list>             closed_list_temp;
        std::unordered_set <t_node*, hash_pattern_database, cmp_pattern_database>   pattern_database_temp;

        queue.push_back(queue_temp);
        closed_list.push_back(closed_list_temp);
        pattern_database.push_back(pattern_database_temp);
    }

    std::vector<std::thread>    threads;
    size_t                      loop = patterns.size();
    while (loop)
    {
        //make "max_threads" or "remaining databases" threads
        int i = 0;
        while (loop && i < _max_threads)
        {
            loop--;
            i++;
        }
        
        for (size_t j = i; j > 0; j--)
            threads.push_back(std::thread(lunch_create_pattern_database_in_threads, std::ref(*this), loop + j - 1));
        for (size_t j = i; j > 0; j--)
            threads[j - 1].join();
        threads.clear();
    }

    for (size_t i = 0; i < patterns.size(); i++)
    {
        if (fill_database_map(i))
            return(1);
    }

    return(0);
}

int Database::lunch_create_pattern_database_in_threads(Database &database, int index)
{
    if (database.create_pattern_database(index))
        return(1);
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
            _max_threads = patterns.size();
            break;
        }
        case 3:
        {
            std::vector<int>    pattern1 = {1, 2, 3, 4, 5, 6, 7, 8};
            patterns.push_back(pattern1);
            _max_threads = patterns.size();
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
            _max_threads = patterns.size();
            break;
        }
        case 5:
        {
            std::vector<int>    pattern1 = {1, 2, 3, 6};
            std::vector<int>    pattern2 = {4, 5, 10, 15};
            std::vector<int>    pattern3 = {7, 8, 11, 12};
            std::vector<int>    pattern4 = {9, 13, 14, 18};
            std::vector<int>    pattern5 = {16, 17, 21, 22};
            std::vector<int>    pattern6 = {19, 20, 23, 24};
            patterns.push_back(pattern1);
            patterns.push_back(pattern2);
            patterns.push_back(pattern3);
            patterns.push_back(pattern4);
            patterns.push_back(pattern5);
            patterns.push_back(pattern6);
            _max_threads = patterns.size();
            break;
        }
        case 6:
        {
            std::vector<int>    pattern1 = {1, 2, 7};
            std::vector<int>    pattern2 = {3, 8, 9};
            std::vector<int>    pattern3 = {4, 5, 10};
            std::vector<int>    pattern4 = {6, 11, 12};
            std::vector<int>    pattern5 = {13, 14, 19};
            std::vector<int>    pattern6 = {15, 16, 21};
            std::vector<int>    pattern7 = {17, 22, 23};
            std::vector<int>    pattern8 = {18, 24};
            std::vector<int>    pattern9 = {20, 26, 27};
            std::vector<int>    pattern10 = {25, 31, 32};
            std::vector<int>    pattern11 = {28, 33, 34};
            std::vector<int>    pattern12 = {29, 30, 35};
            patterns.push_back(pattern1);
            patterns.push_back(pattern2);
            patterns.push_back(pattern3);
            patterns.push_back(pattern4);
            patterns.push_back(pattern5);
            patterns.push_back(pattern6);
            patterns.push_back(pattern7);
            patterns.push_back(pattern8);
            patterns.push_back(pattern9);
            patterns.push_back(pattern10);
            patterns.push_back(pattern11);
            patterns.push_back(pattern12);
            _max_threads = patterns.size();
            break;
        }
        case 7:
        {
            std::vector<int>    pattern1 = {1, 2, 8};
            std::vector<int>    pattern2 = {3, 4, 10};
            std::vector<int>    pattern3 = {5, 6, 12};
            std::vector<int>    pattern4 = {7, 13, 14};
            std::vector<int>    pattern5 = {9, 15, 16};
            std::vector<int>    pattern6 = {11, 17, 18};
            std::vector<int>    pattern7 = {19, 20, 26};
            std::vector<int>    pattern8 = {21, 27, 28};
            std::vector<int>    pattern9 = {22, 23, 29};
            std::vector<int>    pattern10 = {24, 30, 31};
            std::vector<int>    pattern11 = {25, 32, 33};
            std::vector<int>    pattern12 = {34, 35, 42};
            std::vector<int>    pattern13 = {36, 43, 44};
            std::vector<int>    pattern14 = {37, 38, 45};
            std::vector<int>    pattern15 = {39, 40, 46};
            std::vector<int>    pattern16 = {41, 47, 48};
            patterns.push_back(pattern1);
            patterns.push_back(pattern2);
            patterns.push_back(pattern3);
            patterns.push_back(pattern4);
            patterns.push_back(pattern5);
            patterns.push_back(pattern6);
            patterns.push_back(pattern7);
            patterns.push_back(pattern8);
            patterns.push_back(pattern9);
            patterns.push_back(pattern10);
            patterns.push_back(pattern11);
            patterns.push_back(pattern12);
            patterns.push_back(pattern13);
            patterns.push_back(pattern14);
            patterns.push_back(pattern15);
            patterns.push_back(pattern16);
            _max_threads = 4;
            break;
        }
        case 8:
        {
            std::vector<int>    pattern1 = {1, 2, 9};
            std::vector<int>    pattern2 = {3, 11, 12};
            std::vector<int>    pattern3 = {4, 5, 13};
            std::vector<int>    pattern4 = {6, 14, 15};
            std::vector<int>    pattern5 = {7, 8, 16};
            std::vector<int>    pattern6 = {10, 17, 18};
            std::vector<int>    pattern7 = {19, 26, 27};
            std::vector<int>    pattern8 = {20, 28, 29};
            std::vector<int>    pattern9 = {21, 22, 30};
            std::vector<int>    pattern10 = {23, 24, 32};
            std::vector<int>    pattern11 = {25, 33, 34};
            std::vector<int>    pattern12 = {31, 39, 40};
            std::vector<int>    pattern13 = {35, 43, 44};
            std::vector<int>    pattern14 = {36, 37, 45};
            std::vector<int>    pattern15 = {38, 46, 47};
            std::vector<int>    pattern16 = {41, 42, 49};
            std::vector<int>    pattern17 = {48, 55, 56};
            std::vector<int>    pattern18 = {50, 57, 58};
            std::vector<int>    pattern19 = {51, 59, 60};
            std::vector<int>    pattern20 = {52, 53, 61};
            std::vector<int>    pattern21 = {54, 62, 63};
            patterns.push_back(pattern1);
            patterns.push_back(pattern2);
            patterns.push_back(pattern3);
            patterns.push_back(pattern4);
            patterns.push_back(pattern5);
            patterns.push_back(pattern6);
            patterns.push_back(pattern7);
            patterns.push_back(pattern8);
            patterns.push_back(pattern9);
            patterns.push_back(pattern10);
            patterns.push_back(pattern11);
            patterns.push_back(pattern12);
            patterns.push_back(pattern13);
            patterns.push_back(pattern14);
            patterns.push_back(pattern15);
            patterns.push_back(pattern16);
            patterns.push_back(pattern17);
            patterns.push_back(pattern18);
            patterns.push_back(pattern19);
            patterns.push_back(pattern20);
            patterns.push_back(pattern21);
            _max_threads = 1;
            break;
        }
        default:
        {
            std::cerr << "Pattern database heuristic not supported for puzzle of 7 or more" << std::endl;
            return (1);
        }
    }

    return(0);
}

int Database::create_pattern_database(int index)
{
    if (database_already_made(index))
        return(0);

    t_node  *node = initialize_node(index);
    if (!node)
        return(1);
    queue[index].push(node);


    while(queue[index].size() > 0)
    {
        if (bfs(index))
            return (1);
    }
    
    if (create_pattern_database_no_blank_tile(index))
        return (1);

    //indic user about the creation of the database
    std::cout << "Database " << index << " finished (" << closed_list[index].size() << " cases explored)" << std::endl;

    //free the nodes
    pattern_database[index].clear();
    for (auto it = closed_list[index].begin(); it != closed_list[index].end(); ++it)
        delete (*it);
    closed_list[index].clear();

    return(0);
}

int Database::database_already_made(int index)
{
    std::ifstream   fs;
    fs.open(get_database_name(index));
    if (fs.good())
    {
        fs.close();
        return(1);
    }
    fs.close();
    return(0);
}

t_node *Database::initialize_node(int index)
{
    t_node *node;
    try
    {
        node = new t_node;
    }
    catch(const std::bad_alloc& bad_alloc)
    {
        std::cerr << "bad_alloc caught " << bad_alloc.what() << std::endl;
        return(NULL);
    }

    //  initialize node
    initialize_map(node->map, patterns[index]);
    node->blank.i = get_npuzzle_size() - 1;
    node->blank.j = get_npuzzle_size() - 1;
    node->cost = 0;
    node->direction = BEGIN;

    return(node);
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

int Database::bfs(int index)
{
    t_node *node = queue[index].front();
    queue[index].pop();
    
    //search if map already in closed_list
    if (closed_list[index].find(node) != closed_list[index].end())
    {
        delete node;
        return(0);
    }
    closed_list[index].insert(node);
    
    
    //  explore UP
    if (node->blank.i > 0 && node->direction != DOWN)
    {
        if (bfs_add_node(node, UP, index))
            return (1);
    }
    //  explore DOWN
    if (node->blank.i < get_npuzzle_size() - 1 && node->direction != UP)
    {
        if (bfs_add_node(node, DOWN, index))
            return (1);
    }
    //  explore LEFT
    if (node->blank.j > 0 && node->direction != RIGHT)
    {
        if (bfs_add_node(node, LEFT, index))
            return (1);
    }
    //  explore RIGHT
    if (node->blank.j < get_npuzzle_size() - 1 && node->direction != LEFT)
    {
        if (bfs_add_node(node, RIGHT, index))
            return (1);
    }
        
    return (0);
}
    
int Database::bfs_add_node(t_node *node, int direction, int index)
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
    queue[index].push(new_node);
    
    return(0);
}

int Database::create_pattern_database_no_blank_tile(int index)
{
    // create patter_database from closed_list without blank for space optimisation
    t_node  *node; 
    for (auto it = closed_list[index].begin(); it != closed_list[index].end(); ++it)
    {
        node = *it;
        auto    match = pattern_database[index].find(node);
        
        if (match != pattern_database[index].end())
        {
            if ((*it)->cost < (*match)->cost)
                (*match)->cost = (*it)->cost;
        }
        else
            pattern_database[index].insert(node);
    }


    std::ofstream   fs;

    fs.open(get_database_name(index));
    if (!fs.good())
    {
        std::cerr << "error while creating database" << std::endl;
        fs.close();
        return(1);
    }


    for (auto it = pattern_database[index].begin(); it != pattern_database[index].end(); ++it)
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
        int buffer_size = static_cast<int>(buffer.size());

        if (buffer_size <= number_of_tiles)
        {
            std::cerr << "Err databases has been altered" << std::endl;
            return(1);
        }

        std::string str_pattern = buffer.substr(0, number_of_tiles);
        int         cost        = std::stoi(buffer.substr(number_of_tiles, buffer_size - number_of_tiles));

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
