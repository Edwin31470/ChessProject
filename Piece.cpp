
#include <string>;

using namespace std;

class Piece {

	public:
		string type = "none";
		int colour = 0; //none = 0, white = 1, black = 2

		Piece() {

		}

		void Clear() {
			type = "none";
			colour = 0;
		}

		string GetType() {
			return type;
		}

		void SetType(string s)
		{
			type = s;
		}

		int GetColour() {
			return colour;
		}

		void SetColour(int c)
		{
			colour = c;
		}

};