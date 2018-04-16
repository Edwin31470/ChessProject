
#include <iostream>;
#include <bitset>;

using namespace std;

class BitBoard
{
public:
	//8x8 arrays for each colour of piece
	bitset<64> whitePawns;
	unsigned int **whiteRooks = new unsigned int*[8];
	unsigned int **whiteBishops = new unsigned int*[8];
	unsigned int **whiteKnights = new unsigned int*[8];
	unsigned int **whiteQueens = new unsigned int*[8];
	unsigned int **whiteKings = new unsigned int*[8];

	unsigned int **blackPawns = new unsigned int*[8];
	unsigned int **blackRooks = new unsigned int*[8];
	unsigned int **blackBishops = new unsigned int*[8];
	unsigned int **blackKnights = new unsigned int*[8];
	unsigned int **blackQueens = new unsigned int*[8];
	unsigned int **blackKings = new unsigned int*[8];

	unsigned int **whiteAll = new unsigned int*[8];
	unsigned int **blackAll = new unsigned int*[8];

	BitBoard() {
		setupBoard();
	}

	void setupBoard() {
		//white pawns
		for (int i = 0; i < 8; ++i) {
			whitePawns.set[8 + i] = 1;
		}

		//black pawns
		for (int i = 0; i < 8; ++i) {
			whitePawns[i][6] = 1;
		}

		whiteRooks[0][0] = 1;
		whiteRooks[0][7] = 1;
		blackRooks[7][0] = 1;
		blackRooks[7][7] = 1;

		whiteBishops[0][1] = 1;
		whiteBishops[0][6] = 1;
		blackBishops[7][1] = 1;
		blackBishops[7][6] = 1;

		whiteKnights[0][2] = 1;
		whiteKnights[0][5] = 1;
		blackKnights[7][2] = 1;
		blackKnights[7][5] = 1;

		whiteKings[0][3] = 1;
		whiteQueens[0][4] = 1;
		blackKings[7][3] = 1;
		blackQueens[7][4] = 1;
	}

};