#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>

int main()
{
    // create window with size and title
    sf::RenderWindow window(sf::VideoMode(1300, 900), "N PUZZLE");

    // create rectangle with size
    sf::RectangleShape rectangle(sf::Vector2f(800.f, 800.f));

    // set rectangle color, position, and border
    rectangle.setFillColor(sf::Color::White);
    rectangle.setPosition(250.f, 50.f);
    rectangle.setOutlineThickness(10.f);
    rectangle.setOutlineColor(sf::Color(250, 150, 100));

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::Red);
        window.draw(rectangle);
        window.display();
    }

    return 0;
}