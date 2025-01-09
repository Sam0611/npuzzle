#include "Npuzzle.hpp"
#include <stdlib.h> // abs



int main(int argc, char *argv[])
{
    //  initialize puzzle class
    Npuzzle npuzzle;

    if (npuzzle_parsing(argc, argv, npuzzle))
        return(1);

    //  test a star algo
    if (npuzzle.a_star_algorithm())
    {
        std::cerr << "ERR a_star probleme" << std::endl;
        return(1);
    }


    //test a suppr
//     t_movement  test;

//     test.direction = UP;
//     test.value = 177;
//     test.previous = NULL;

//     std::cout << "direction = " << test.direction << std::endl;
//     std::cout << "value = " << test.value << std::endl;
//     std::cout << "previous = " << test.previous << std::endl;
// std::cout << std::endl << std::endl;

//     t_movement  test2;

//     test2.direction = DOWN;
//     test2.value = 777;
//     test2.previous = &test;

//     std::cout << "direction = " << test2.direction << std::endl;
//     std::cout << "value = " << test2.value << std::endl;
//     std::cout << "previous = " << test2.previous << std::endl;
// std::cout << std::endl << std::endl;


// std::cout << "test 3 :" << std::endl;

//     std::cout << "direction = " << test2.previous->direction << std::endl;
//     std::cout << "value = " << test2.previous->value << std::endl;
//     std::cout << "previous = " << test2.previous->previous << std::endl;



}