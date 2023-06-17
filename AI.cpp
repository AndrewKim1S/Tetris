#include "AI.h"


AI::AI() {

}

AI::~AI() {

}

// Does not look at T-spin or tucking pieces into the board
void AI::findBestPlay(int board[boardLength][boardHeight], Tetromino piece) {

		std::cout << "Find ALL Permutations -----------" << std::endl;
		// First find all of the possible locations & orientations the piece
		// can be added into the board
		
		// Move the piece all the way to the left
		while(checkPiece(board, piece)) {
				piece.moveLeft();
		}
		piece.moveRight();

		int rotationlimit;
		switch(piece.blockType) {
				case 0 || 1 || 2:
						rotationlimit = 2;
						break;
				case 3:
						rotationlimit = 1;
				default:
						rotationlimit = 4;
						break;
		}

		// Find all positions across the board
		for(int i = 0; i < boardLength; i++) {
				sf::Vector2i defaultPosition = piece.position;
				// Find all rotations across the board
				for(int j = 0; j < rotationlimit; j++) {
						
						// Create temporary copy to analyze future moves
						int simulatedBoard[boardLength][boardHeight];
						std::copy(&board[0][0], &board[0][0] + boardLength * boardHeight,
								&simulatedBoard[0][0]);				

						piece.rotateRight();
						/*if(!checkPiece(board, piece)) {
								piece.rotateLeft();
								continue;
						}*/
				
						// Move piece to the bottom
						while(checkPiece(board, piece)) {
								piece.moveDown();
						}
						piece.moveUp();
						
						// std::cout << piece.position.x << ", " << piece.position.y << std::endl;

						// Add this simulated piece position to the board
						for(int i = 0; i < 4; i++) {
								for(int j = 0; j < 4; j++) {
										if(piece.blocks[piece.blockIndex][i][j]) {
												simulatedBoard[piece.position.x + i][piece.position.y + j] = piece.blockType;
										}
								}
						}

						// Print for debugging
						for(int y = 10; y < boardHeight; y++) {
								for(int x = 0; x < boardLength; x++) {
										std::cout << simulatedBoard[x][y] << " ";
								}
								std::cout << "\n";
						}
						std::cout << "\n\n\n";
				}
				
				// Move the piece to the next position over
				piece.position = defaultPosition;
				piece.moveRight();
				if(!checkPiece(board, piece)) {
						piece.moveLeft();
						break;
				}
		}
}

int AI::completedLines() {
		return 0;
}

int AI::agregateHeight() {
		return 0;

}

int AI::bumpiness() {
		return 0;

}

int AI::holes() {
		return 0;

}


bool AI::checkPiece(int board[boardLength][boardHeight], Tetromino &piece){
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
