#ifndef DATABASE
#define DATABASE

#include "Npuzzle.hpp"
#include <string>
#include <iostream>
#include <unordered_set>

typedef struct  s_node
{
    std::vector< std::vector<int> > map;
    t_coor                          blank;
    int                             cost;
    int                             direction;
}   t_node;

struct hash_closed_list
{
    int  operator() (const t_node *node) const
    {
        int  temp = 0;
        int  hash_nbr = 0;
        for (size_t i = 0; i != node->map.size(); ++i)
        {
            for (size_t j = 0; j != node->map[i].size(); j++)
            {
                hash_nbr *= 10;
                hash_nbr += node->map[i][j];
                if (hash_nbr > 100000000)
                {
                    temp = temp ^ hash_nbr;
                    hash_nbr = 0;
                }
            }
        }
        hash_nbr *= 10;
        hash_nbr += node->blank.i;
        if (hash_nbr > 100000000)
        {
            temp = temp ^ hash_nbr;
            hash_nbr = 0;
        }
        hash_nbr *= 10;
        hash_nbr += node->blank.j;
        return (temp ^ hash_nbr);
    }
};

struct cmp_closed_list
{
    bool operator() (t_node *a, t_node *b) const
    {
        if (a->map != b->map)
            return (0);
        if (a->blank.i != b->blank.i)
            return (0);
        if (a->blank.j != b->blank.j)
            return(0);
        return(1);
    }
};

struct hash_pattern_database
{
    size_t  operator() (const t_node *node) const
    {
        int  temp = 0;
        int  hash_nbr = 0;
        for (size_t i = 0; i != node->map.size(); ++i)
        {
            for (size_t j = 0; j != node->map[i].size(); j++)
            {
                hash_nbr *= 10;
                hash_nbr += node->map[i][j];
                if (hash_nbr > 100000000)
                {
                    temp = temp ^ hash_nbr;
                    hash_nbr = 0;
                }
            }
        }
        return (temp ^ hash_nbr);
    }
};

struct cmp_pattern_database
{
    bool operator() (t_node *a, t_node *b) const
    {
        if (a->map == b->map)
            return (1);
        return(0);
    }
};

class   Database
{
public:
    Database();
    ~Database();

    int algo(Npuzzle npuzzle);

    //variables
    std::queue<t_node*>                                          queue;
    std::unordered_set<t_node*, hash_closed_list, cmp_closed_list>   closed_list;
    std::unordered_set<t_node*, hash_pattern_database, cmp_pattern_database>   pattern_database;
    std::vector< std::vector<int> > patterns;

private:
    int define_patterns(Npuzzle npuzzle);
    void    initialize_map(std::vector< std::vector<int> > &map, Npuzzle npuzzle, std::vector<int> &pattern);
    int algo_iterative(Npuzzle npuzzle);
    int add_bfs(t_node *node, int direction);
    int create_pattern_database(int number);
    std::string node_to_string(t_node *node, int number);


    //utils
    void                            print_map(std::vector< std::vector<int> > map);
};


#endif