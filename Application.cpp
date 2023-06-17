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
		pieceFallTime = 1000;

		clock.restart();
		pieceClock.restart();


		// Change this to State::AIPLAY 
		state = State::RUN;
		// state = State::AIPLAY;
	
		currentLevel = 0;
		linesCleared = 0;

		score = 0;
		scoreText = "Score " + std::to_string(score);

		// Music 
		int temp = rand() % 2;
		std::string songTitle = "util/TetrisMusic" + std::to_string(temp) + ".wav";
		assert(soundtrack.openFromFile(songTitle));
		if(temp == 1) {
				soundtrack.setVolume(100);
		} else {
				soundtrack.setVolume(30);
		}
		soundtrack.setLoop(true);
		soundtrack.play();

		// Sound Effects
		assert(lineclearSound.loadFromFile("util/clear.wav"));
		assert(gameOverSound.loadFromFile("util/success.wav"));
		assert(tetrisSound.loadFromFile("util/tetrisClear.wav"));

		// Text UI
		font = new sf::Font();
		assert(font->loadFromFile("util/RoseauSlabFree-Yz2yL.otf"));
		playMode = sf::Text();
		playMode.setFont(*font);
		playMode.setCharacterSize(35);
		playMode.setFillColor(sf::Color::White);
		playMode.setPosition(500, 50);
		playMode.setString("Player");
		scoreBoard = sf::Text();
		scoreBoard.setFont(*font);
		scoreBoard.setCharacterSize(30);
		scoreBoard.setFillColor(sf::Color::White);
		scoreBoard.setPosition(500, 100);
		scoreBoard.setString(scoreText);
		level.setFont(*font);
		level.setCharacterSize(30);
		level.setFillColor(sf::Color::White);
		level.setPosition(500, 150);
		level.setString("Level " + std::to_string(currentLevel));

		tetrisAI = AI();
		
		nextPiece = Tetromino(rand() % 7);

		newBlock();
}

// Deconstructor
Application::~Application() {
		delete window;
		delete font;
}

// main run function to update, render, and poll events
void Application::run() {
		// check state of application
		switch(state) {
				case State::AIPLAY:
						update();
						render();
						tetrisAI.findBestPlay(board, piece);
						state = State::PAUSE;
						while(window->pollEvent(event)) {
								if(event.type == sf::Event::Closed) {
										window->close();
								}
						}
						break;
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
				case State::GAMEOVER:
						playMode.setString("Game Over");
						// check event 
						while(window->pollEvent(event)) {
								if(event.type == sf::Event::Closed) {
										window->close();
								}
								if(event.type == sf::Event::KeyPressed) {
								
										// close window when escape
										if(event.key.code == sf::Keyboard::Escape) {
												window->close();
										}
								}

						}
						render();
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

		// render text
		window->draw(playMode);
		window->draw(scoreBoard);
		window->draw(level);
		
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
						if(piece.blocks[piece.blockIndex][y][x]) {
								cell.setPosition(sf::Vector2f(xOffset + (y*tileSize) + piece.position.x * tileSize, 
										yOffset + (x * tileSize) + piece.position.y * tileSize));
								cell.setFillColor(piece.blockColor[piece.blockIndex]);
								window->draw(cell);
						}
				}
		}

		// render next piece 
		for(int y = 0; y < 4; y++) {
				for(int x = 0; x < 4; x++) {
						if(nextPiece.blocks[nextPiece.blockIndex][y][x]) {
								cell.setPosition(sf::Vector2f(750 + (y * tileSize), 
										100 + (x*tileSize)));
								cell.setFillColor(nextPiece.blockColor[nextPiece.blockIndex]);
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
												state = State::AIPLAY;
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
		bool pushBlockUp = false;
		for(int x = 0; x < boardLength; x++) {
				if(board[x][1] != 0 && (x == 4 || x == 5)) {
						pushBlockUp = true;
				}

				// Game over
				if(board[x][0] != 0) {
						state = State::GAMEOVER;
						soundEffect.setBuffer(gameOverSound);
						soundEffect.play();
						soundtrack.stop();
				}
		}
		piece = nextPiece;
		nextPiece = Tetromino(rand() % 7);
		if(pushBlockUp) {
				piece.moveUp();
		}
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

		switch(rows.size()) {
				case 0:
						break;
				case 1:
						soundEffect.setBuffer(lineclearSound);
						soundEffect.play();
						score += 40 * (currentLevel + 1);
						break;
				case 2:
						score += 100 * (currentLevel + 1);
						soundEffect.setBuffer(lineclearSound);
						soundEffect.play();
						break;
				case 3:
						score += 300 * (currentLevel + 1);
						soundEffect.setBuffer(lineclearSound);
						soundEffect.play();
						break;
				case 4:
						score += 1200 * (currentLevel + 1);
						soundEffect.setBuffer(tetrisSound);
						soundEffect.play();
				default:
						break;
		}

		linesCleared += rows.size();
		manageLevel();
		scoreText = "Score " + std::to_string(score);
		scoreBoard.setString(scoreText);

		for(size_t i = 0; i < rows.size(); i++) {
				for(int y = rows[i] + i; y >= 1; y--) {
						for(int x = 0; x < boardLength; x++) {
								board[x][y] = board[x][y-1];
						}
				}
		}
}

void Application::manageLevel() {
		if(linesCleared >= 10) {
				linesCleared = 0;
				currentLevel++;
				pieceFallTime = pieceFallTime * 0.9;
				level.setString("Level " + std::to_string(currentLevel));
		}
}
