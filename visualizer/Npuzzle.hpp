#ifndef __NPUZZLE__
#define __NPUZZLE__

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <random>

//  square root of INT_MAX troncated
#define LIMIT_OF_PIECES 46340

typedef struct  s_piece
{
    int         nbr;
    std::string str;
}   t_piece;

class Npuzzle
{
    public:
        Npuzzle();
        ~Npuzzle();

        void    set_size(int size);
        int     get_size(void);
        int     get_max_piece(void);

        std::vector< std::vector<t_piece> > _map;

    private:
        int _size;
        int _max_piece;
    
};

//  parsing functions
int    npuzzle_parsing(char **argv, Npuzzle &npuzzle);

//  errors functions
int err_open_text_file(char **argv, std::ifstream &fs);
int err_empty_line(std::ifstream &fs, std::string &buffer);
int err_npuzzle_size_syntax(std::string &buffer, int i);
int err_size_to_big(int size);
int err_npuzzle_size_invalid(int size);
int err_missing_map(std::ifstream &fs, std::string &buffer);
int err_piece_to_big(std::ifstream &fs, std::string &buffer, int max_piece, int piece);
int err_piece_duplicate(std::ifstream &fs, Npuzzle &npuzzle, int i, int j, int piece);
int err_missing_piece(Npuzzle &npuzzle, std::ifstream &fs, int i, int j);
int err_wrong_syntax(std::ifstream &fs, std::string &buffer, int k);
int err_to_many_pieces(std::ifstream &fs, std::string &buffer, int k);
int err_to_many_lines(std::ifstream &fs);

#endif