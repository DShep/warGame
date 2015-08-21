/*
 *  BattleField.h
 *  mattsWarGame
 *
 *  Created by Catherine Creamer on 12/27/13.
 *  Copyright 2013 __MyCompanyName__. All rights reserved.
 *
 */

#include "Soldier.h"

class BattleField {
private:
	void makeBlankField(const int fSizeX, const int fSizeY);
public:
	int** sIndex;
	int** sType;
	int** occupied;
	int** sHealth;
	int** terrain;
	int sizeX;
	int sizeY;
	
	BattleField(const int fSizeX, const int fSizeY);
	~BattleField();
	void printField(const int whichField);
	void updateField(Soldier* unit, const int numUnits);
};