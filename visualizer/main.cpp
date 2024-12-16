#include "Visualizer.hpp"
#include <iostream>
#include <vector>

// void draw_puzzle_blocks(sf::RenderWindow window, std::vector<sf::RectangleShape> blocks, std::vector<sf::Text> numbers)
// {
//     for (std::vector<int>::size_type i = 0; i < blocks.size(); i++)
//     {
//         window.draw(blocks[i]);
//         window.draw(numbers[i]);
//     }
// }

// sf::Text init_text(sf::Font font, char c)
// {
//     sf::Text text;
//     text.setFont(font);
//     text.setCharacterSize(24);
//     text.setString(c);
//     text.setFillColor(sf::Color::Black);
//     return (text);
// }

int main()
{
    // get size from argv
    int size = 3;

    std::vector<std::string> data;
    data.push_back("2");
    data.push_back("3");
    data.push_back("1");
    data.push_back("8");
    data.push_back("4");
    data.push_back("5");
    data.push_back("0");
    data.push_back("6");
    data.push_back("7");

    Visualizer visualizer(1300, 900, "N-puzzle", size);
    visualizer.copy_vector(data);
    visualizer.init_window();
    
    return 0;
}