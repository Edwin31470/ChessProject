
#include <string>;
#include <iostream>;

#include "Board.cpp";

using namespace std;
int main(int argc, char * argv[]) {

	Board mainBoard;
	mainBoard.printBoard();

	cout << mainBoard.GetSquare(1, 1).GetType();
	cout << mainBoard.GetSquare(1, 2).GetType() << endl;

	mainBoard.movePiece(1, 2, 1, 1);
	mainBoard.printBoard();

	cout << mainBoard.GetSquare(1, 1).GetType();
	cout << mainBoard.GetSquare(1, 2).GetType() << endl;
	
	return 0;
};