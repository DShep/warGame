/*
 *  BattleField.h
 *  mattsWarGame
 *
 *  Created by Catherine Creamer on 12/27/13.
 *  Copyright 2013 __MyCompanyName__. All rights reserved.
 *
 */

#include "Soldier.hpp"

class BattleField {
private:
	void setDefaultValues();
	void setCurrentValues(Soldier* unit,const int numunit);
	void resizeBattleField();
public:
    matrix<int> sIndex;
    matrix<int> sType;
    matrix<bool> occupied;
    matrix<int> sHealth;
    matrix<int> terrain;
	int sizeX;
	int sizeY;

	BattleField(const int fSizeX, const int fSizeY);
	void printField(const int whichField);
	void updateField(Soldier* unit, const int numunit);
};
