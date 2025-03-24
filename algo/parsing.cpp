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

static void  skip_white_space(std::string &buffer, int &i)
{
    while (isspace(buffer[i]))
        i++;
}

static int  is_comment(std::string buffer, int i)
{
    if (buffer[i] == '#')
        return(1);
    return(0);
}

static int  npuzzle_size_is_invalid(std::string buffer, Npuzzle &npuzzle)
{
    int size = 0;
    int i = 0;

    skip_white_space(buffer, i);

    if (err_npuzzle_size_syntax(buffer, i))
        return(1);

    //  take size
    while (isdigit(buffer[i]))
    {
        size *= 10;
        size += buffer[i] - 48;
        if (err_size_to_big(size))
            return(1);
        i++;
    }

    if (err_npuzzle_size_invalid(size))
        return(1);
    npuzzle.set_size(size);

    skip_white_space(buffer, i);
    if (is_comment(buffer, i))
        return(0);

    if (err_npuzzle_size_syntax(buffer, i))
        return(1);

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
        std::vector<int>    initialization;

        npuzzle._map.push_back(initialization);
        int j = 0;
        int k = 0;

        skip_white_space(buffer, k);

        while (j < npuzzle.get_size())
        {
            //  initialize piece
            npuzzle._map[i].push_back(0);

            //  add pieces to map
            while (isdigit(buffer[k]))
            {
                npuzzle._map[i][j] *= 10;
                npuzzle._map[i][j] += buffer[k] - 48;
                k++;

                if (err_piece_to_big(fs, buffer, npuzzle.get_max_piece(), npuzzle._map[i][j]))
                    return(1);
            }

            // if (err_missing_piece(npuzzle, fs, i, j))
            //     return(1);

            if (err_piece_missing_duplicate(fs, npuzzle, i, j, npuzzle._map[i][j]))
                return(1);

            skip_white_space(buffer, k);

            j++;

            if (is_comment(buffer, k))
                continue;

            if (err_wrong_syntax(fs, buffer, k))
                return(1);
        }

        i++;

        if (is_comment(buffer, k))
            continue;

        if (err_to_many_pieces(fs, buffer, k))
            return(1);
    }

    getline(fs, buffer);

    if (err_to_many_lines(fs))
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
    while (fs.good() && buffer[0] == '#')
        getline(fs, buffer);
    
    if (err_empty_line(fs, buffer))
        return(1);

    if (npuzzle_size_is_invalid(buffer, npuzzle))
    {
        fs.close();
        return(1);
    }
    
    if (npuzzle_map_is_invalid(fs, buffer, npuzzle))
        return(1);

    fs.close();
    return (0);
}

static void  shuffle_fisher_yates_algorithm(Npuzzle &npuzzle, int size)
{
    //  seed
    std::random_device  rd;
    //  pseudo-random number generator
    std::mt19937        rng(rd());


    //  Fisher Yates algorithm
    int random;
    int temp;
    int n = npuzzle.get_max_piece();

    for (int i = size - 1; i >= 0; i--)
    {
        for (int j = size - 1; j >= 0; j--)
        {
            //  for equal distribution on specific range
            std::uniform_int_distribution<std::mt19937::result_type>    dist(0, n);

            random = dist(rng);
            
            if (random == n)
                continue;

            temp = npuzzle._map[i][j];
            npuzzle._map[i][j] = npuzzle._map[random / size][random % size];
            npuzzle._map[random / size][random % size] = temp;
            n--;
        }
    }
}

static void  npuzzle_generate_map(Npuzzle &npuzzle)
{
    int size = npuzzle.get_size();
    int nbr = 0;

    //  create npuzzle map with default value
    for (int i = 0; i < size; i++)
    {
        std::vector<int>    initialization;
        npuzzle._map.push_back(initialization);
        for (int j = 0; j < size; j++)
        {
            npuzzle._map[i].push_back(0);
            nbr++;
        }
    }

    shuffle_fisher_yates_algorithm(npuzzle, size);
}

int    npuzzle_parsing(int argc, char **argv, Npuzzle &npuzzle)
{

    if (check_args_count(argc))
        return(1);

    std::string file_format = ".txt";
    std::string text_file = argv[1];

    //  1st option : check if arg is text file
    if ((text_file.size() >= file_format.size()) && (!text_file.compare(text_file.size() - file_format.size(), file_format.size(), file_format)))
        return (npuzzle_parsing_text_file(argv, npuzzle));

    //  2nd option : check if arg is npuzzle size
    if (npuzzle_size_is_invalid(argv[1], npuzzle))
        return(1);
    npuzzle_generate_map(npuzzle);

    return (0);
}