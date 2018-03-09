
#include <string>;
#include <iostream>;

#include "Board.cpp";

using namespace std;
int main(int argc, char * argv[]) {

	Board mainBoard;
	mainBoard.printBoard();

	int x1 = 2;
	int y1 = 7;
	int x2 = 1;
	int y2 = 6;

	cout << mainBoard.GetSquare(y1, x1).GetType() << " to ";
	cout << mainBoard.GetSquare(y2, x2).GetType() << endl;

	mainBoard.movePiece(y1, x1, y2, x2);
	mainBoard.printBoard();

	cout << mainBoard.GetSquare(y1, x1).GetType() << " and ";
	cout << mainBoard.GetSquare(y2, x2).GetType() << endl;
	
	return 0;
};