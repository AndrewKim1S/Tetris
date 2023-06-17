#pragma once

#include <iostream>
#include <algorithm>
#include "Tetromino.h"


const int boardLength = 10;
const int boardHeight = 20;

class AI {

		public:
				AI();
				~AI();
				void findBestPlay(int board[boardLength][boardHeight], Tetromino piece);
				int completedLines();
				int agregateHeight();
				int bumpiness();
				int holes();

				bool checkPiece(int board[boardLength][boardHeight], Tetromino &piece);

		private:


};
