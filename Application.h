#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <cassert>

#include "Tetromino.h"
#include "AI.h"


const float tileSize = 40;

class Application {

		// application states
		enum State {
				RUN,
				PAUSE,
				AIPLAY,
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

				sf::Music soundtrack;
				sf::SoundBuffer lineclearSound;
				sf::SoundBuffer gameOverSound;
				sf::SoundBuffer tetrisSound;
				sf::Sound soundEffect;

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
				Tetromino nextPiece;
		
				AI tetrisAI;

				sf::RectangleShape cell;

				double pieceFallTime;
				int score;

				int currentLevel;
				int linesCleared;
};
