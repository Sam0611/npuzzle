#ifndef __NPUZZLE__
#define __NPUZZLE__

#include <iostream>
#include <string>
#include <cmath>
#include <climits>
#include <list>
#include <forward_list>
#include <random>
#include <new>
#include <algorithm>    // std::find
#include <chrono>       // timer
#include <ctime>        // for test purposes
#include "Database.hpp"
#include <stdio.h>

//  square root of INT_MAX troncated
#define LIMIT_OF_PIECES 46340

class Database;
class Npuzzle;

typedef int (*heuristic_func)(std::vector< std::vector<int> > &, Npuzzle &npuzzle);

//  heuristic function
#define MISPLACED_TILE "1"
#define MANHATTAN "2"
#define MANHATTAN_LINEAR_CONFLICT "3"
#define PATTERN_DATABASE "4"

typedef struct  s_movement
{
    int         value;
    int         direction;
    int         cost;
    t_coor      blank;
    std::vector< std::vector<int> > map;
    s_movement  *previous;
}   t_movement;

struct hash_movement
{
    int  operator() (const t_movement *movement) const
    {
        int  temp = 0;
        int  hash_nbr = 0;
        for (size_t i = 0; i != movement->map.size(); i++)
        {
            for (size_t j = 0; j != movement->map[i].size(); j++)
            {
                hash_nbr *= 10;
                hash_nbr += movement->map[i][j];
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

//  functor to overload comparaison function to all_movements 
struct cmp_movement
{
    bool operator() (t_movement *a, t_movement *b) const
    {
        return (a->map == b->map);
    }
};

class Npuzzle
{
    public:
        Npuzzle();
        ~Npuzzle();

        //  accessors functions
        void    set_size(int size);
        int     get_size(void);
        int     get_max_piece(void);
        int     get_time_complexity(void);
        void    incr_time_complexity(void);
        void    set_map_generated(void);
        bool    get_map_generated(void);

        //mains functions
        int     npuzzle_parsing(int argc, char **argv, Npuzzle &npuzzle);
        int     a_star_algorithm();
        bool    is_solvable(void);

        //heuristics functions
        static int  get_Misplaced_tiles_value(std::vector< std::vector<int> > &map, Npuzzle &npuzzle);
        static int  get_Manhattan_heuristic_value(std::vector< std::vector<int> > &map, Npuzzle &npuzzle);
        static int  get_Manhattan_heuristic_and_linear_conflict_value(std::vector< std::vector<int> > &map, Npuzzle &npuzzle);
        static int  get_pattern_database_heuristic_value(std::vector< std::vector<int> > &map, Npuzzle &npuzzle);


        //heuristic associates functions
        static int     get_linear_conflicts_value(std::vector< std::vector<int> > &map);
        static void    set_coordinates(int to_check, int &x, int &y, std::vector< std::vector<int> > &map);


        std::vector< std::vector<int> > _map;
        std::list<t_movement*>               possibilities;
        std::unordered_set<t_movement*, hash_movement, cmp_movement>  all_movements;
        // std::unordered_set<t_movement*, std::hash<t_movement *>, cmp_movement>  all_movements;
        Database    database;
        heuristic_func  _heuristic_func;

    private:
        int     get_map_blank(int &i, int &j);
        int     a_star_algorithm_recusrsive(t_movement *movement);
        int     add_possibility(t_movement *parent_movement, int direction);
        void    movement_assign_map_and_blank(t_movement *movement, t_movement *parent_movement);
        int     finished(int heuristic, t_movement *movement);
        void    print_solution_movement(t_movement *movement);

        int _size;
        int _max_piece;
        int _time_complexity;
        int _map_generated;
};

//  errors functions
int err_open_text_file(char **argv, std::ifstream &fs);
int err_empty_line(std::ifstream &fs, std::string &buffer);
int err_npuzzle_size_syntax(std::string &buffer, int i);
int err_size_to_big(int size);
int err_npuzzle_size_invalid(int size);
int err_missing_map(std::ifstream &fs, std::string &buffer);
int err_piece_to_big(std::ifstream &fs, std::string &buffer, int max_piece, int piece);
int err_piece_missing_duplicate(std::ifstream &fs, Npuzzle &npuzzle, int i, int j, int piece);
int err_wrong_syntax(std::ifstream &fs, std::string &buffer, int k);
int err_to_many_pieces(std::ifstream &fs, std::string &buffer, int k);
int err_to_many_lines(std::ifstream &fs);

#endif