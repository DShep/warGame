#include "BattleField.hpp"

int main() {
    // f before a variable refers to field
    // s before a variable refers to soldier
	int fSizeX = 10;
	int fSizeY = 10;
	int numUnit = 8;
	int numPlayer = 2;
	int windowSizeX = 1000;
	int windowSizeY = 1000;

	// Soldier is a class that contains all the information pertaining to a given unit
	// make a vector of vectors which contains a list of soldiers for each player;
	vector< vector< Soldier > > unit(numPlayer);

    // resize the second dimension of this vector for every unit
    // in each player's army
    // each player can have a different number of unit
    // which is why its a 2d vector and not a matrix
    for(int pp=0; pp<numPlayer; pp++) {
        unit[pp].reserve(numUnit);
    }

    // create the window for the game
    sf::RenderWindow window(sf::VideoMode(windowSizeY, windowSizeX), "War Game");

	// battlefield is a class that contains all the information for the current game board
	BattleField bField(fSizeX,fSizeY);

	// initialize unit type and location for testing purposes
    for(auto& pp: unit) {
        for(auto& uu: player) {
            unit[pp].emplace_back(unitType[pp][uu],unitLocationX[pp][uu],unitLocationY[pp][uu],uu);
        }
	}

	// update the battlefield to show the current locations of unit
	bField.updateField(unit);

    return 0;
}
