
#include <string>;
#include <iostream>;

#include "BoardHandler.cpp";

using namespace std;

class GameHandler {

	public:
		MoveHandler moveHandler;
		BoardHandler boardHandler;
		AI ai;

		Board realBoard;
		Colour currentTurn = Colour::white;

		//bool humanCheck = false;
		//bool aiCheck = false;
		//bool humanCheckMate = false;
		//bool aiCheckMate = false;

		bool humanHasKing = true;
		bool aiHasKing = true;

		GameHandler() {	
			boardHandler.setupNormalBoard(realBoard);
			boardHandler.printBoard(realBoard);

			//while both kings are in play, play the game
			while (humanHasKing && aiHasKing)
			{
				if (currentTurn == white)
				{
					int valid = 0;
					while (!valid)
					{
						valid = humanMove();
					}
					aiHasKing = checkKing(black, realBoard);
					
				}
				else if (currentTurn == black)
				{
					aiMove();
					boardHandler.printBoard(realBoard);
					humanHasKing = checkKing(white, realBoard);
				}
				currentTurn = (currentTurn == white) ? black : white;
			}

			if (!aiHasKing) {
				cout << endl << "Congratulations, you win!" << endl;
			}
			else if (!humanHasKing) {
				cout << endl << "Unfortunately, the ai has beaten you!" << endl;
			}
			cout << endl << "Thank you for playing!" << endl;
			cin;
		}

		//return of 1 means a valid move has been performed. 0 is an invalid move
		int humanMove() {
			string result;
			int letterOld, numberOld, letterNew, numberNew;

			cout << "Input must be in example format 'a1b2':" << endl;

			cin >> result;

			// +1 for padding(+1 instead of +2 as array is already 0 indexed)
			letterOld = letterToNum(result.substr(0, 1)) + 1;
			numberOld = atoi(result.substr(1, 1).c_str()) + 1;

			letterNew = letterToNum(result.substr(2, 1)) + 1;
			numberNew = atoi(result.substr(3, 1).c_str()) + 1;

			Move move = Move(numberOld, letterOld, numberNew, letterNew);
			vector<Move> validMoves = moveHandler.validMoves(currentTurn, realBoard);

			//if move is valid (identical move in valid move set) move piece and print board
			for each (Move validMove in validMoves)
			{
				if (move.newLet == validMove.newLet && move.newNum == validMove.newNum &&
					move.oldLet == validMove.oldLet && move.oldNum == validMove.oldNum) {
						moveHandler.movePiece(move, white, realBoard);
						boardHandler.printBoard(realBoard);
						return 1;
				}
			}

			cout << "That move is invalid." << endl;
			return 0;
		}

		void aiMove() {
			ai.AIMove(currentTurn, realBoard);
		}

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

		//checks if a king is taken
		bool checkKing(Colour colour, Board& board) {
			for (int numberCoord = 2; numberCoord < 10; numberCoord++) {
				for (int letterCoord = 2; letterCoord < 10; letterCoord++) {
					if ((board.GetSquare(numberCoord, letterCoord).type == king || board.GetSquare(numberCoord, letterCoord).type == kingCastle)
						&& board.GetSquare(numberCoord, letterCoord).colour == colour)
					{
						cout << colour << " King detected!" << endl;
						return true;
					}		
				}
			}
			cout << "No " << colour << " King detected!" << endl;
			return false;
		}
};
