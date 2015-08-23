#include "BattleField.hpp"

int main() {
    // f before a variable refers to field
    // s before a variable refers to soldier
	int fSizeX = 10;
	int fSizeY = 10;
	int numUnit = 8;
	int numPlayer = 2;

	// Soldier is a class that contains all the information pertaining to a given unit
	// make a vector of vectors which contains a list of soldiers for each player;
	vector< vector< Soldier > > unit(numPlayer);

	// create a 2d vector that has all the unit types
	vector< vector< int > > unitType(numPlayer);

	// create two 2d vectors that have all unit X and Y locations
    vector< vector< int > > unitLocationX(numPlayer);
    vector< vector< int > > unitLocationY(numPlayer);

    // resize the second dimension of this vector for every unit
    // in each player's army
    // each player can have a different number of units
    // which is why its a 2d vector and not a matrix
    for(int pp=0; pp<numPlayer; pp++) {
        unit[pp].resize(numUnit);
        unitType[pp].resize(numUnit);
        unitLocationX[pp].resize(numUnit);
        unitLocationY[pp].resize(numUnit);
    }

	// battlefield is a class that contains all the information for the current game board
	BattleField bField(fSizeX,fSizeY);

	// initialize unit type and location for testing purposes
	for(int pp=0; pp<numPlayer; pp++) {
        for(int uu=0; uu<numUnit; uu++) {
            unitType[pp][uu] = uu+1;
            unitLocationX[pp][uu] = 2*ii;
            unitLocationY[pp][uu] = 0;

            unit[pp][uu].initSoldier(unitType[pp][uu],unitLocationX[pp][uu],unitLocationY[pp][uu],uu);
        }
	}

	// update the battlefield to show the current locations of units
	bField.updateField(&unit);

	// display the field to the user
	bField.printField(2);

    return 0;
}
