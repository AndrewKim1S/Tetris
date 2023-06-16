#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <cassert>

#include "Tetromino.h"


const int boardLength = 10;
const int boardHeight = 20;
const float tileSize = 40;

class Application {

		// application states
		enum State {
				RUN,
				PAUSE,
				GAMEOVER
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
				void manageLevel();

		private:
				sf::RenderWindow* window;
				sf::Event event;
				sf::Clock clock;
				sf::Clock pieceClock;

				sf::Font* font;
				sf::Text playMode;
				sf::Text scoreBoard;
				sf::Text level;
				std::string scoreText;

				int windowLength;
				int windowHeight;

				int board[boardLength][boardHeight] = {0};

				State state;
				Tetromino piece;

				sf::RectangleShape cell;

				double pieceFallTime;
				int score;

				int currentLevel;
				int linesCleared;
};
