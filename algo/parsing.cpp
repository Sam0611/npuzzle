#include "Npuzzle.hpp"

static int  check_args_count(int argc)
{
    //  if no puzzle or number provided
    if (argc != 2)
    {
        if (argc < 2) 
            std::cerr << "Not enough arguments" << std::endl;
        if (argc > 2) 
            std::cerr << "Too many arguments" << std::endl;
        return (1);
    }
    return (0);
}


static int  npuzzle_size_is_invalid(std::ifstream &fs, std::string buffer, Npuzzle &npuzzle)
{
    int size = 0;
    int i = 0;

    if (err_npuzzle_size_syntax(fs, buffer, i))
        return(1);

    //  take size
    while (isdigit(buffer[i]))
    {
        size *= 10;
        size += buffer[i] - 48;
        if (err_size_to_big(fs, size))
            return(1);
        i++;
    }

    if (err_npuzzle_size_syntax(fs, buffer, i))
        return(1);

    npuzzle.set_size(size);
    return(0);
}

static int  npuzzle_map_is_invalid(std::ifstream &fs, std::string buffer, Npuzzle &npuzzle)
{
    int i = 0;

    while (i < npuzzle.get_size())
    {
        getline(fs, buffer);
        if (err_missing_map(fs, buffer))
            return(1);

        //  initializations
        std::vector<t_piece>    initializion;
        npuzzle._map.push_back(initializion);
        int j = 0;
        int k = 0;
        while (isspace(buffer[k]))
            k++;

        while (j < npuzzle.get_size())
        {
            //  initialize piece
            t_piece piece;
            npuzzle._map[i].push_back(piece);
            npuzzle._map[i][j].str = "";
            npuzzle._map[i][j].nbr = 0;

            //  add pieces to map
            while (isdigit(buffer[k]))
            {
                (npuzzle._map[i][j].str) += buffer[k];
                npuzzle._map[i][j].nbr *= 10;
                npuzzle._map[i][j].nbr += buffer[k] - 48;
                k++;

                if (err_piece_to_big(fs, buffer, npuzzle.get_max_piece(), npuzzle._map[i][j].nbr))
                    return(1);
            }

            if (err_piece_duplicate(fs, npuzzle, i, j, npuzzle._map[i][j].nbr))
                return(1);

            while (isspace(buffer[k]))
                k++;

            if (err_missing_piece(npuzzle, fs, i, j))
                return(1);

            if (err_wrong_syntax(fs, buffer, k))
                return(1);
            j++;
        }

        if (err_to_many_pieces(fs, buffer, k))
            return(1);
        i++;
    }

    getline(fs, buffer);
    if (err_to_many_pieces(fs, buffer, 0))
        return(1);

    return(0);
}

static int  npuzzle_parsing_text_file(char ** argv, Npuzzle &npuzzle)
{
    std::ifstream   fs;
    if (err_open_text_file(argv, fs))
        return(1);

    std::string buffer;
    getline(fs, buffer);

    //  skip commentaries
    while (fs.good() && buffer.size() && buffer[0] == '#')
        getline(fs, buffer);
    
    if (err_empty_line(fs, buffer))
        return(1);

    if (npuzzle_size_is_invalid(fs, buffer, npuzzle))
        return(1);
    
    if (npuzzle_map_is_invalid(fs, buffer, npuzzle))
        return(1);




    //  print rest of document
    while (getline(fs, buffer) && fs.good())
        std::cout << buffer << std::endl;





    fs.close();
    //  parsing good 👌
    return (0);
}

int    npuzzle_parsing(int argc, char **argv, Npuzzle &npuzzle)
{

    if (check_args_count(argc))
        return(1);

    std::string file_format = ".txt";
    std::string text_file = argv[1];

    //  check if arg is text file
    if ((text_file.size() >= file_format.size()) && (!text_file.compare(text_file.size() - file_format.size(), file_format.size(), file_format)))
        return (npuzzle_parsing_text_file(argv, npuzzle));



    //  check if number conform
    //  npuzzle_parsing_size(argv, npuzzle);

    //  something wrong
    std::cerr << "something wrong" << std::endl;
    return (1);
}