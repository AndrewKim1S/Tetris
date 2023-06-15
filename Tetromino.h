#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>


class Tetromino {

		public:

				int blocks[7][4][4] = {
						1,0,0,0,
						1,0,0,0,
						1,0,0,0,
						1,0,0,0,

						1,0,0,0,
						1,1,0,0,
						0,1,0,0,
						0,0,0,0,

						0,1,0,0,
						1,1,0,0,
						1,0,0,0,
						0,0,0,0,

						1,1,0,0,
						1,1,0,0,
						0,0,0,0,
						0,0,0,0,

						1,0,0,0,
						1,1,0,0,
						1,0,0,0,
						0,0,0,0,

						1,0,0,0,
						1,0,0,0,
						1,1,0,0,
						0,0,0,0,

						0,1,0,0,
						0,1,0,0,
						1,1,0,0,
						0,0,0,0,
				};
				
				sf::Color blockColor[7] = {sf::Color::Cyan, sf::Color::Green, sf::Color::Red,
						sf::Color::Yellow, sf::Color::Magenta, sf::Color(255, 165, 0), sf::Color::Blue};


				int blockType;
				int blockIndex;
				
				Tetromino();
				Tetromino(int type);
				~Tetromino();

				void moveDown();
				void moveUp();
				void moveLeft();
				void moveRight();
				void rotateLeft();
				void rotateRight();

				bool hitBottom();

				sf::Vector2i position;

};
