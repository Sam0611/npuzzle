#include "Npuzzle.hpp"

int  err_open_text_file(char **argv, std::ifstream &fs)
{
    fs.open(argv[1]);
    if (!fs.good())
    {
        std::cerr << "Error while opening \"" << argv[1] << "\" file" << std::endl;
        fs.close();
        return (1);
    }
    return(0);
}

int err_empty_line(std::ifstream &fs, std::string &buffer)
{
    if (buffer.empty())
    {
        std::cerr << "Error empty line" << std::endl;
        fs.close();
        return(1);
    }
    return(0);
}

int err_npuzzle_size_syntax(std::ifstream &fs, std::string &buffer, int i)
{
    if (buffer[i] && !isdigit(buffer[i]))
    {
        std::cerr << "Npuzzle size syntax invalid" << std::endl;
        fs.close();
        return(1);
    }
    return(0);
}

int err_size_to_big(std::ifstream &fs, int size)
{
    if (size >= LIMIT_OF_PIECES)
    {
        std::cerr << "Npuzzle size to big" << std::endl;
        fs.close();
        return(1);
    }
    return (0);
}

int err_missing_map(std::ifstream &fs, std::string &buffer)
{
    if (buffer.empty())
    {
        std::cerr << "Line map is empty" << std::endl;
        fs.close();
        return(1);
    }
    return(0);
}

int err_piece_to_big(std::ifstream &fs, std::string &buffer, int max_piece, int piece)
{
    if (piece > max_piece)
    {
        std::cerr << "A piece number exceed the maximum in line :" << std::endl << "\"" << buffer << "\"" << std::endl;
        fs.close();
        return(1);
    }
    return(0);
}

int err_piece_duplicate(std::ifstream &fs, std::string &buffer, Npuzzle &npuzzle, int i, int j, int piece)
{
    j--;
    while (j >= 0)
    {
        if (piece == npuzzle._map[i][j].nbr)
        {
            std::cerr << "A piece number is duplicate in line :" << std::endl << "\"" << buffer << "\"" << std::endl;
            fs.close();
            return(1);
        }
        j--;
    }

    i--;
    while (i >= 0)
    {
        j = npuzzle.get_size();
        while (j >= 0)
        {
            if (piece == npuzzle._map[i][j].nbr)
            {
                std::cerr << "A piece number is duplicate in line :" << std::endl << "\"" << buffer << "\"" << std::endl;
                fs.close();
                return(1);
            }
            j--;
        }
        i--;
    }
    return(0);
}

int err_missing_piece(Npuzzle &npuzzle, std::ifstream &fs, int i, int j)
{
    if (npuzzle._map[i][j].str.empty())
    {
        std::cerr << "Missing piece on map" << std::endl;
        fs.close();
        return (1);
    }
    return(0);
}

int err_wrong_syntax(std::ifstream &fs, std::string &buffer, int k)
{
    if (buffer[k] && !isdigit(buffer[k]))
    {
        std::cerr << "Wrong syntax in line : " << buffer << std::endl;
        fs.close();
        return(1);
    }
    return(0);
}

int err_to_many_pieces(std::ifstream &fs, std::string &buffer, int k)
{
    if (buffer[k])
    {
        std::cerr << "To many pieces in line :" << std::endl << "\"" << buffer << "\"" << std::endl;
        fs.close();
        return(1);
    }
    return(0);
}