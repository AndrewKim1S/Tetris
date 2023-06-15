#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "Tetromino.h"


const int boardLength = 10;
const int boardHeight = 20;
const float tileSize = 40;

class Application {

		// application states
		enum State {
				RUN,
				PAUSE
		};

		public:
				Application();
				Application(int width, int length);
				~Application();
				
				void run();
				void render();
				void pollEvents();
				void update();

				bool isRunning();
				void newBlock();
				bool checkPieceMovement();
				void clearLines();

		private:
				sf::RenderWindow* window;
				sf::Event event;
				sf::Clock clock;
				sf::Clock pieceClock;

				int windowLength;
				int windowHeight;

				int board[boardLength][boardHeight] = {0};

				State state;
				Tetromino piece;

				sf::RectangleShape cell;

				int pieceFallTime;
};
