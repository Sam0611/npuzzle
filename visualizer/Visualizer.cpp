#include "Visualizer.hpp"
#include <iostream>
#include <algorithm>

Visualizer::Visualizer() : _width(500), _height(500), _title("N PUZZLE"), _puzzleSize(3), _squareSize(99.f), _squareBorder(1.f), _indexZero(0)
{
}

Visualizer::Visualizer(int width, int height, std::string title, int puzzleSize) : _width(width), _height(height), _title(title), _puzzleSize(puzzleSize), _squareSize(99.f), _squareBorder(1.f), _indexZero(0)
{
}

Visualizer::Visualizer(const Visualizer &copy)
{
	*this = copy;
}

Visualizer &Visualizer::operator=(const Visualizer &visualizer)
{
	this->_width = visualizer._width;
	this->_height = visualizer._height;
	this->_title = visualizer._title;
	return (*this);
}

Visualizer::~Visualizer()
{
}

void Visualizer::copy_vector(std::vector<std::string> vec)
{
	_data = vec;
}

sf::RectangleShape init_shape(float size)
{
    sf::RectangleShape shape(sf::Vector2f(size, size));
    shape.setOutlineThickness(1.f);
    shape.setOutlineColor(sf::Color::Black);
    shape.setFillColor(sf::Color::White);
    return (shape);
}

void Visualizer::init_window()
{
    if (_data.empty())
    {
        std::cerr << "Error: no data" << std::endl;
        return ;
    }

	// create window
    sf::RenderWindow window(sf::VideoMode(_width, _height), _title);

    float startX = (_width - _puzzleSize * _squareSize) / 2;
    float startY = (_height - _puzzleSize * _squareSize) / 2;

	// create rectangle containing the puzzle
	float border = 5.f;
	sf::RectangleShape rectangle(sf::Vector2f((_squareSize + _squareBorder) * _puzzleSize, (_squareSize + _squareBorder) * _puzzleSize));
	rectangle.setFillColor(sf::Color::White);
    rectangle.setPosition(startX, startY);
    rectangle.setOutlineThickness(border);
    rectangle.setOutlineColor(sf::Color::Black);

	// create font
	sf::Font font;
    if (!font.loadFromFile("arial.ttf"))
    {
        std::cerr << "Error: Font cannot be loaded" << std::endl;
        return ;
    }

	// create blocks
	int n = -1;
    for (int row = 0; row < _puzzleSize; row++)
    {
        for (int col = 0; col < _puzzleSize; col++)
        {
            n++;
            if (_data[n] == "0")
			{
				_indexZero = n;
				continue ;
			}

            sf::Text text;
            text.setFont(font);
            text.setCharacterSize(24);
            text.setString(_data[n]);
            text.setFillColor(sf::Color::Black);

            sf::RectangleShape square = init_shape(_squareSize);
            square.setPosition(startX + col * (_squareSize + _squareBorder), startY + row * (_squareSize + _squareBorder));
            text.setPosition(square.getPosition() + (square.getSize() / 2.f));
            _numbers.push_back(text);
            _blocks.push_back(square);
        }
    }

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
				window.close();

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            {
                if (_indexZero % _puzzleSize == 0)
                    continue ;

				_indexZero--;
                for (int i = 0; i < 999; i++)
                {
                    _blocks[_indexZero].move(0.1, 0);
                    _numbers[_indexZero].move(0.1, 0);
                    window.draw(rectangle);
                    // draw_puzzle_blocks(window, blocks, numbers);
                    for (std::vector<int>::size_type i = 0; i < _blocks.size(); i++)
                    {
                        window.draw(_blocks[i]);
                        window.draw(_numbers[i]);
                    }
                    window.display();
                }
            }

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            {
                if ((_indexZero + 1) % _puzzleSize == 0)
                    continue ;

                for (int i = 0; i < 999; i++)
                {
                    _blocks[_indexZero].move(-0.1, 0);
                    _numbers[_indexZero].move(-0.1, 0);
                    window.draw(rectangle);
                    // draw_puzzle_blocks(window, blocks, numbers);
                    for (std::vector<int>::size_type i = 0; i < _blocks.size(); i++)
                    {
                        window.draw(_blocks[i]);
                        window.draw(_numbers[i]);
                    }
                    window.display();
                }
				_indexZero++;
            }

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            {
                if (_indexZero < _puzzleSize)
                    continue ;

				_indexZero -= _puzzleSize;
                for (int i = 0; i < 999; i++)
                {
                    _blocks[_indexZero].move(0, 0.1);
                    _numbers[_indexZero].move(0, 0.1);
                    window.draw(rectangle);
                    // draw_puzzle_blocks(window, blocks, numbers);
                    for (std::vector<int>::size_type i = 0; i < _blocks.size(); i++)
                    {
                        window.draw(_blocks[i]);
                        window.draw(_numbers[i]);
                    }
                    window.display();
                }
				for (int i = 0; i < _puzzleSize - 1; i++)
				{
        			std::iter_swap(_data.begin() + _indexZero + i, _data.begin() + _indexZero + 1 + i);
        			std::iter_swap(_blocks.begin() + _indexZero + i, _blocks.begin() + _indexZero + 1 + i);
        			std::iter_swap(_numbers.begin() + _indexZero + i, _numbers.begin() + _indexZero + 1 + i);
				}
            }

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            {
                if (_indexZero >= _puzzleSize * _puzzleSize - _puzzleSize)
                    continue ;

				_indexZero += _puzzleSize - 1;
                for (int i = 0; i < 999; i++)
                {
                    _blocks[_indexZero].move(0, -0.1);
                    _numbers[_indexZero].move(0, -0.1);
                    window.draw(rectangle);
                    // draw_puzzle_blocks(window, blocks, numbers);
                    for (std::vector<int>::size_type i = 0; i < _blocks.size(); i++)
                    {
                        window.draw(_blocks[i]);
                        window.draw(_numbers[i]);
                    }
                    window.display();
                }
				for (int i = 0; i < _puzzleSize - 1; i++)
				{
        			std::iter_swap(_data.begin() + _indexZero - i, _data.begin() + _indexZero - 1 - i);
        			std::iter_swap(_blocks.begin() + _indexZero - i, _blocks.begin() + _indexZero - 1 - i);
        			std::iter_swap(_numbers.begin() + _indexZero - i, _numbers.begin() + _indexZero - 1 - i);
				}
				_indexZero++;
            }
		}

        window.clear(sf::Color(150, 150, 150));
        window.draw(rectangle);
        for (std::vector<int>::size_type i = 0; i < _blocks.size(); i++)
        {
            window.draw(_blocks[i]);
            window.draw(_numbers[i]);
        }
		window.display();
	}
}