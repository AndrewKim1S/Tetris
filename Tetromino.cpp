#include "Tetromino.h"


Tetromino::Tetromino() {
}

Tetromino::Tetromino(int type) {
		blockType = type+1;
		blockIndex = type;
		position.x = 4;
		position.y = 1;
}

Tetromino::~Tetromino() {

}

void Tetromino::moveDown() {
		position.y += 1;
}

void Tetromino::moveUp() {
		position.y -= 1;
}

void Tetromino::moveLeft() {
		if(position.x >= 0) {
				position.x -= 1;
		}
}

void Tetromino::moveRight() {
		if(position.x <= 8) {
				position.x += 1;
		}
}

// CCW rotation
void Tetromino::rotateLeft() {

		int rotated[4][4] = {0};
		int size = 0;
	
		// Finding the nessecary indexes to rotate
		for(int y = 0; y < 4; y ++) {
				for(int x = 0; x < 4; x++) {
						if(blocks[blockIndex][y][x]) {
								size = std::max(std::max(x, y) + 1, size);
						}
				}
		}

		for(int y = 0; y < size; y++) {
				for(int x = 0; x < size; x++) {
						if(blocks[blockIndex][y][x]) {
								rotated[size-1-x][y] = 1;
						}
				}
		}

		for(int y = 0; y < 4; y ++) {
				for(int x = 0; x < 4; x++) {
						blocks[blockIndex][y][x] = rotated[y][x];
				}
		}
		
}

// CW rotation
void Tetromino::rotateRight() {
		int rotated[4][4] = {0};
		int size = 0;
	
		// Finding the nessecary indexes to rotate
		for(int y = 0; y < 4; y ++) {
				for(int x = 0; x < 4; x++) {
						if(blocks[blockIndex][y][x]) {
								size = std::max(std::max(x, y) + 1, size);
						}
				}
		}

		for(int y = 0; y < size; y++) {
				for(int x = 0; x < size; x++) {
						if(blocks[blockIndex][y][x]) {
								rotated[x][size-1-y] = 1;
						}
				}
		}

		for(int y = 0; y < 4; y ++) {
				for(int x = 0; x < 4; x++) {
						blocks[blockIndex][y][x] = rotated[y][x];
				}
		}
}

bool Tetromino::hitBottom() {
		if(position.y >= 19) {
				return true;
		}
		return false;
}
