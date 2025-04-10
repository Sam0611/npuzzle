#ifndef __NPUZZLE__
#define __NPUZZLE__

#include <iostream>
#include <string>
#include <fstream>
#include <cmath>
#include <climits>
#include <vector>
#include <list>
#include <forward_list>
#include <unordered_set>
#include <random>
#include <new>
#include <algorithm> // std::find
#include <ctime> //for test purposes
#include <functional> //to pass function as parameter

//  square root of INT_MAX troncated
#define LIMIT_OF_PIECES 46340
//  directions
#define BEGIN 0
#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4

typedef struct s_coor
{
    int i;
    int j;
}   t_coor;

typedef struct  s_movement
{
    int         value;
    int         direction;
    int         cost;
    t_coor      blank;
    std::vector< std::vector<int> > map;
    s_movement  *previous;
}   t_movement;

//  functor to overload comparaison function to all_movements 
struct cmp
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

        //mains functions
        int     a_star_algorithm(int (*heuristic_function)(std::vector< std::vector<int> >));
        bool    is_solvable(void);

        //heuristics functions
        static int     get_Misplaced_tiles_value(std::vector< std::vector<int> > map);
        static int     get_Manhattan_heuristic_value(std::vector< std::vector<int> > map);

        //heuristic associates functions
        static int     get_linear_conflicts_value(std::vector< std::vector<int> > map);
        static void    set_coordinates(int to_check, int &x, int &y, std::vector< std::vector<int> > map);


        std::vector< std::vector<int> > _map;
        std::list<t_movement*>               possibilities;
        std::unordered_set<t_movement*, std::hash<t_movement *>, cmp>  all_movements;

    private:
        int     get_map_blank(int &i, int &j);
        int     a_star_algorithm_recusrsive(int (*heuristic_function)(std::vector< std::vector<int> >), t_movement *movement);
        int     add_possibility(int (*heuristic_function)(std::vector< std::vector<int> >), t_movement *parent_movement, int direction);
        void    movement_assign_map_and_blank(t_movement *movement, t_movement *parent_movement);
        int     finished(int heuristic, t_movement *movement);
        void    print_solution_movement(t_movement *movement);

        int _size;
        int _max_piece;
        int _time_complexity;
};

//  parsing functions
int    npuzzle_parsing(int argc, char **argv, Npuzzle &npuzzle);

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