#include "Npuzzle.hpp"

static int  check_args_count(int argc)
{
    //  if no puzzle or number provided
    if (argc != 2)
    {
        if (argc < 2) 
            std::cerr << "Not enough arguments" << std::endl;
        if (argc > 2) 
            std::cerr << "To many arguments" << std::endl;
        return (1);
    }
    return (0);
}

static int  open_text_file(char **argv, std::ifstream &fs)
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

static int  text_file_is_empty(std::ifstream &fs, std::string &buffer)
{
    //  check if text_file empty
    if (buffer.empty())
    {
        std::cerr << "text file is empty" << std::endl;
        fs.close();
        return(1);
    }
    return(0);
}

static int  npuzzle_size_is_not_valid(std::ifstream &fs, std::string buffer, Npuzzle &npuzzle)
{
    //  try convert string to int
    try
    {
        npuzzle.set_size(std::stoi(buffer));
    }
    catch (std::invalid_argument const &ex)
    {
        std::cerr << "Size of n_puzzle is not a number or is invalid" << std::endl;
        fs.close();
        return (1);
    }
    catch (std::out_of_range const &ex)
    {
        std::cerr << "Size of n_puzzle is out of range" << std::endl;
        fs.close();
        return (1);
    }

    //  check if n-puzzle size to big or to low
    if (npuzzle.get_size() < 2)
    {
        std::cerr << "N-puzzle size is to low" << std::endl;
        fs.close();
        return (1);
    }
    else if (npuzzle.get_size() > sqrt(INT_MAX) - 1)
    {
        std::cerr << "N-puzzle size is to big" << std::endl;
        fs.close();
        return (1);
    }
    return(0);
}

static int  npuzzle_parsing_text_file(char ** argv, Npuzzle &npuzzle)
{
    std::ifstream   fs;
    if (open_text_file(argv, fs))
        return(1);

    std::string buffer;
    getline(fs, buffer);

    //  skip commentaries
    while (fs.good() && buffer.size() && buffer[0] == '#')
        getline(fs, buffer);
    
    if (text_file_is_empty(fs, buffer))
        return(1);

    if (npuzzle_size_is_not_valid(fs, buffer, npuzzle))
        return(1);
    
    //  test debug
    std::cout << "n-puzzle size is " << npuzzle.get_size() << std::endl;



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