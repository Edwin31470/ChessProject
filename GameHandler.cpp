
#include <string>;
#include <iostream>;

#include "MoveHandler.cpp";

using namespace std;

class GameHandler {

	public:
		Board mainBoard;
		MoveHandler moveHandler;
		Colour currentTurn;

		GameHandler() {	
			currentTurn = white;
			mainBoard.printBoard();

			/*int x1 = 2;
			int y1 = 7;
			int x2 = 1;
			int y2 = 6;

			mainBoard.movePieceLegacy(y1, x1, y2, x2);
			mainBoard.printBoard();*/
		}

		void humanMove() {
			string result;
			int letterOld, numberOld, letterNew, numberNew;

			cout << "Input must be in example format 'a1b2':" << endl;

			cin >> result;

			// +1 for padding(+1 instead of +2 as array is already 0 indexed)
			letterOld = letterToNum(result.substr(0, 1)) + 1;
			numberOld = atoi(result.substr(1, 1).c_str()) + 1;

			letterNew = letterToNum(result.substr(2, 1)) + 1;
			numberNew = atoi(result.substr(3, 1).c_str()) + 1;

			Move move = Move(letterOld, numberOld, letterNew, numberNew);
			vector<Move> validMoves = moveHandler.validMoves(currentTurn, mainBoard);

			//if move is valid (identical move in valid move set) move piece and print board
			for each (Move validMove in validMoves)
			{
				if (move.newLet == validMove.newLet && move.newNum == validMove.newNum &&
					move.oldLet == validMove.oldLet && move.oldNum == validMove.oldNum) {
						moveHandler.movePiece(move, mainBoard);
						mainBoard.printBoard();
						break;
				}
			}
		};

		//converts input letter to a coordinate the program can understand
		int letterToNum(string letter)
		{
			if (letter == "a") {
				return 1;
			}
			else if (letter == "b") {
				return 2;
			}
			else if (letter == "c") {
				return 3;
			}
			else if (letter == "d") {
				return 4;
			}
			else if (letter == "e") {
				return 5;
			}
			else if (letter == "f") {
				return 6;
			}
			else if (letter == "g") {
				return 7;
			}
			else if (letter == "h") {
				return 8;
			}
		}
};
