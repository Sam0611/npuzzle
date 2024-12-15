#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <vector>

void draw_puzzle_blocks(sf::RenderWindow window, std::vector<sf::RectangleShape> blocks, std::vector<sf::Text> numbers)
{
    for (std::vector<int>::size_type i = 0; i < blocks.size(); i++)
    {
        window.draw(blocks[i]);
        window.draw(numbers[i]);
    }
}

sf::Text init_text(sf::Font font, char c)
{
    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(24);
    text.setString(c);
    text.setFillColor(sf::Color::Black);
    return (text);
}

sf::RectangleShape init_shape(float size)
{
    sf::RectangleShape shape(sf::Vector2f(size, size));
    shape.setOutlineThickness(1.f);
    shape.setOutlineColor(sf::Color::Black);
    shape.setFillColor(sf::Color::White);
    return (shape);
}

int main()
{
    int size = 3;
    int screenWidth = 1300;
    int screenHeight = 900;
    float squareSize = 100.f;
    float startX = (screenWidth - size * squareSize) / 2;
    float startY = (screenHeight - size * squareSize) / 2;
    
    std::vector<char> data;
    data.push_back('9');
    data.push_back('2');
    data.push_back('3');
    data.push_back('1');
    data.push_back('8');
    data.push_back('4');
    data.push_back('5');
    data.push_back('0');
    data.push_back('6');
    data.push_back('7');

    // create window with size and title
    sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight), "N PUZZLE");

    // create rectangle with size
    sf::RectangleShape rectangle(sf::Vector2f(squareSize * size, squareSize * size));

    // set rectangle color, position, and border
    rectangle.setFillColor(sf::Color::White);
    rectangle.setPosition(startX, startY);
    rectangle.setOutlineThickness(5.f);
    rectangle.setOutlineColor(sf::Color::Black);

    // create text in a square
    sf::Font font;
    if (!font.loadFromFile("arial.ttf"))
    {
        std::cerr << "Error: Font cannot be loaded" << std::endl;
        return (1);
    }

    std::vector<sf::Text> numbers;
    std::vector<sf::RectangleShape> blocks;

    float posX = startX;
    float posY = startY;

    int i = -1;
    for (int row = 0; row < size; row++)
    {
        for (int col = 0; col < size; col++)
        {
            i++;
            if (data[i] == '0')
                continue ;

            sf::Text text;
            text.setFont(font);
            text.setCharacterSize(24);
            text.setString(data[i]);
            text.setFillColor(sf::Color::Black);

            sf::RectangleShape square = init_shape(squareSize);
            square.setPosition(posX + col * 100, posY + row * 100);
            text.setPosition(square.getPosition() + (square.getSize() / 2.f));
            numbers.push_back(text);
            blocks.push_back(square);
        }
    }

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                window.close();
            
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            {
                for (int i = 0; i < 1000; i++)
                {
                    blocks[0].move(0.1, 0);
                    numbers[0].move(0.1, 0);
                    window.draw(rectangle);
                    // draw_puzzle_blocks(window, blocks, numbers);
                    for (std::vector<int>::size_type i = 0; i < blocks.size(); i++)
                    {
                        window.draw(blocks[i]);
                        window.draw(numbers[i]);
                    }
                    window.display();
                }
            }
            
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            {
                for (int i = 0; i < 1000; i++)
                {
                    blocks[0].move(-0.1, 0);
                    numbers[0].move(-0.1, 0);
                    window.draw(rectangle);
                    // draw_puzzle_blocks(window, blocks, numbers);
                    for (std::vector<int>::size_type i = 0; i < blocks.size(); i++)
                    {
                        window.draw(blocks[i]);
                        window.draw(numbers[i]);
                    }
                    window.display();
                }
            }
        }

        window.clear(sf::Color::Red);
        window.draw(rectangle);
        // draw_puzzle_blocks(window, blocks, numbers);
        for (std::vector<int>::size_type i = 0; i < blocks.size(); i++)
        {
            window.draw(blocks[i]);
            window.draw(numbers[i]);
        }
        window.display();
    }

    return 0;
}