#include "Application.h"


// Constructor
Application::Application() {

}

// Constructor
Application::Application(int width, int length) {
		// Create window
		window = new sf::RenderWindow(sf::VideoMode(width, length), "Tetris");
		window->setPosition(sf::Vector2i(300, 75));
		this->windowLength = width;
		this->windowHeight = length;
		
		cell.setSize(sf::Vector2f(tileSize-1, tileSize-1));
		pieceFallTime = 500;

		clock.restart();
		pieceClock.restart();

		state = State::RUN;

		newBlock();
}

// Deconstructor
Application::~Application() {
		delete window;
}

// main run function to update, render, and poll events
void Application::run() {
		// check state of application
		switch(state) {
				case State::RUN:
						if(clock.getElapsedTime().asMilliseconds() > 10) {
								pollEvents();
								render();
								update();
								clock.restart();
						} else {
								return;
						}
						break;
				case State::PAUSE:
						pollEvents();
						break;
				default:
						break;
		}
}

// Draw all the boids and quadtree to the window
void Application::render() {
		window->clear(sf::Color(32, 32, 32));

		float xOffset = 50;
		float yOffset = 50;
		
		// render grid lines
		for(int i = 0; i < boardLength+1; i++) {
				sf::Vertex line[] = {
						sf::Vertex(sf::Vector2f(xOffset + tileSize*i, yOffset) ,sf::Color::White),
						sf::Vertex(sf::Vector2f(xOffset + tileSize*i, yOffset + boardHeight * tileSize) ,sf::Color::White)
				};
				window->draw(line, 2, sf::Lines);
		}
		for(int i = 0; i < boardHeight+1; i++) {
				sf::Vertex line[] = {
						sf::Vertex(sf::Vector2f(xOffset, yOffset + tileSize*i), sf::Color::White),
						sf::Vertex(sf::Vector2f(xOffset + boardLength * tileSize, 
								yOffset + tileSize*i ), sf::Color::White)
				};
				window->draw(line, 2, sf::Lines);
		}

		// render pieces
		for(int i = 0; i < boardLength; i++) {
				for(int j = 0; j < boardHeight; j++) {
						if(board[i][j] == 0) {
								continue;
						} else {
								cell.setPosition(sf::Vector2f(xOffset + (i*tileSize), yOffset + 
										(j*tileSize)));
								cell.setFillColor(piece.blockColor[board[i][j]-1]);
								window->draw(cell);
						}
				}
		}

		// render block
		for(int y = 0; y < 4; y++) {
				for(int x = 0; x < 4; x++) {
						if(piece.blocks[piece.blockType-1][y][x]) {
								cell.setPosition(sf::Vector2f(xOffset + (y*tileSize) + piece.position.x * tileSize, 
										yOffset + (x * tileSize) + piece.position.y * tileSize));
								cell.setFillColor(piece.blockColor[piece.blockIndex]);
								window->draw(cell);
						}
				}
		}

		window->display();
}

// poll events from the user
void Application::pollEvents() {
		// check event 
		while(window->pollEvent(event)) {
				switch(event.type) {
						
						// close window 
						case sf::Event::Closed:
								window->close();
								break;

						case sf::Event::KeyPressed:
								
								// close window when escape
								if(event.key.code == sf::Keyboard::Escape) {
										window->close();
								}

								// toggle Game state
								else if(event.key.code == sf::Keyboard::P) {
										if(state == State::PAUSE) {
												state = State::RUN;
										} 
										else {
												state = State::PAUSE;
										}
								}

								// Move piece
								if(event.key.code == sf::Keyboard::Right) {
										piece.moveRight();
										if(!checkPieceMovement()) {
												piece.moveLeft();
										}
								}
								if(event.key.code == sf::Keyboard::Left) {
										piece.moveLeft();
										if(!checkPieceMovement()) {
												piece.moveRight();
										}
								}
								if(event.key.code == sf::Keyboard::Down) {
										piece.moveDown();
										if(!checkPieceMovement()) {
												piece.moveUp();
										}
								}
								if(event.key.code == sf::Keyboard::Space) {
										while(checkPieceMovement()) {
												piece.moveDown();
										}
										piece.moveUp();

										for(int i = 0; i < 4; i++) {
												for(int j = 0; j < 4; j++) {
														if(piece.blocks[piece.blockIndex][i][j]) {
																board[piece.position.x + i][piece.position.y + j] = 
																		piece.blockType;
														}
												}
										}
										clearLines();
										newBlock();
								}

								// Rotate piece
								if(event.key.code == sf::Keyboard::Z) {
										piece.rotateLeft();
										if(!checkPieceMovement()) {
												piece.rotateRight();
										}
								}
								if(event.key.code == sf::Keyboard::X) {
										piece.rotateRight();
										if(!checkPieceMovement()) {
												piece.rotateLeft();
										}
								}
								
						default:
								break;
				}
		}
}

// update the application, quadtree & boids
void Application::update() {
		bool ready = false;
	
		/*
		for(int y = 0; y < boardHeight; y++) {
				for(int x = 0; x < boardLength; x++) {
						std::cout << board[x][y] << " ";
				}
				std::cout << "\n";
		}

		std::cout << "\n\n\n" << std::endl;
		*/


		if(pieceClock.getElapsedTime().asMilliseconds() > pieceFallTime) {
				pieceClock.restart();
				ready = true;
		}

		if(ready) {
				piece.moveDown();
				if(checkPieceMovement()) {
						pieceClock.restart();
				} 
				// Add piece to the board and create new piece
				else {
						piece.moveUp();
						for(int i = 0; i < 4; i++) {
								for(int j = 0; j < 4; j++) {
										if(piece.blocks[piece.blockIndex][i][j]) {
												board[piece.position.x + i][piece.position.y + j] = 
														piece.blockType;
										}
								}
						}
						clearLines();
						newBlock();
				}
		}
}

// return the state of the window
bool Application::isRunning() {
		return window->isOpen();
}

// Create a new piece
void Application::newBlock() {
		piece = Tetromino(rand() % 7);
}

// Check the piece boundaries
bool Application::checkPieceMovement() {
		for(int i = 0; i < 4; i++) {
				for(int j = 0; j < 4; j++) {
						if(piece.blocks[piece.blockIndex][i][j] == 0) {
								continue;
						}
						if(piece.position.x + i < 0 || piece.position.x + i >= 10 ||
								piece.position.y + j >= 20) {
								return false;
						}
						if(board[piece.position.x + i][piece.position.y + j]) {
								return false;
						}
				}
		}
		return true;
}

// Clear lines
void Application::clearLines() {
		/*
		for(int y = 0; y < boardHeight; y++) {
				for(int x = 0; x < boardLength; x++) {
						std::cout << board[x][y] << " ";
				}
				std::cout << "\n";
		}
		std::cout << "\n\n\n";
		*/

		std::vector<int> rows;

		for(int from = boardHeight-1; from >= 0; from--) {
				int count = 0;
				for(int x = 0; x < boardLength; x++) {
						if(board[x][from] != 0) {
								count ++;
						}
				}

				if(count == boardLength) {
						rows.push_back(from);
				}
		}

		for(size_t i = 0; i < rows.size(); i++) {
				for(int y = rows[i] + i; y >= 1; y--) {
						for(int x = 0; x < boardLength; x++) {
								board[x][y] = board[x][y-1];
						}
				}
		}
}
