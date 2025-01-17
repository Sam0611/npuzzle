#include "Visualizer.hpp"
#include <iostream>
#include <vector>
#include "Npuzzle.hpp"

int main(int ac, char **av)
{
    if (ac == 1)
        return (1);

    //  initialize puzzle class
    Npuzzle npuzzle;

    if (npuzzle_parsing(av, npuzzle))
        return (1);

    std::vector<std::string> data;
    for (unsigned int i = 0; i < npuzzle._map.size(); i++)
    {
        for (unsigned int j = 0; j < npuzzle._map.size(); j++)
        {
            data.push_back(npuzzle._map[i][j].str);
        }
    }

    std::string tmp, instructions;
    for (int i = 2; i < ac; i++)
        tmp += av[i];

    for (std::size_t i = 0; i + 2 < tmp.size(); i++)
    {
        if (int(tmp[i]) == -30 && int(tmp[i + 1]) == -122)
        {
            i += 2;
            int x = int(tmp[i]);
            switch (x)
            {
                case -109:
                    instructions += "d";
                    break ;
                case -110:
                    instructions += "r";
                    break ;
                case -111:
                    instructions += "u";
                    break ;
                case -112:
                    instructions += "l";
                    break ;
            }
        }
    }

    Visualizer visualizer(1300, 900, "N-puzzle", npuzzle.get_size());
    visualizer.copy_vector(data);
    visualizer.set_instructions(instructions);
    visualizer.init_window();
    
    return (0);
}