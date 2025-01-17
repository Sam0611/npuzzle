#ifndef __VISUALIZER__
#define __VISUALIZER__

#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <vector>

class Visualizer
{
	public:
		Visualizer();
		Visualizer(int width, int height, std::string title, int puzzleSize);
		Visualizer(const Visualizer &copy);
		Visualizer& operator=(const Visualizer &visualizer);
		~Visualizer();
		void init_window();
		void copy_vector(std::vector<std::string> vec);
		void set_instructions(std::string instructions);

	private:
		int _width;
        int _height;
        std::string _title;
        int _puzzleSize;
		float _squareSize;
		float _squareBorder;
		int _indexZero;
		std::string _instructions;
		std::vector<sf::RectangleShape> _blocks;
		std::vector<sf::Text> _numbers;
		std::vector<std::string> _data;

		void draw_puzzle_blocks(sf::RenderWindow &window);
		void move_up(sf::RenderWindow &window, sf::RectangleShape rectangle);
		void move_down(sf::RenderWindow &window, sf::RectangleShape rectangle);
		void move_left(sf::RenderWindow &window, sf::RectangleShape rectangle);
		void move_right(sf::RenderWindow &window, sf::RectangleShape rectangle);
};

#endif