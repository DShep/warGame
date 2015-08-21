/*
 *  BattleField.cpp
 *  mattsWarGame
 *
 *  Created by Catherine Creamer on 12/27/13.
 *  Copyright 2013 __MyCompanyName__. All rights reserved.
 *
 */

#include "BattleField.h"

BattleField::BattleField(const int fSizeX, const int fSizeY) {
	sizeX = fSizeX;
	sizeY = fSizeY;
	
	sIndex = new int*[sizeY];
	sType = new int*[sizeY];
	occupied = new int*[sizeY];
	sHealth = new int*[sizeY];
	terrain = new int*[sizeY];
	
	
	for(int jj = 0; jj < sizeY; jj++) {
		sIndex[jj] = new int[sizeX];
		sType[jj] = new int[sizeX];
		occupied[jj] = new int[sizeX];
		sHealth[jj] = new int[sizeX];
		terrain[jj] = new int[sizeX];
		
		for(int ii = 0; ii < sizeX; ii++) {
			sIndex[jj][ii] = -1;
			sType[jj][ii] = 0;
			occupied[jj][ii] = false;
			sHealth[jj][ii] = 0;
			terrain[jj][ii] = 0;
		}
	}
}

void BattleField::updateField(Soldier* unit, const int numUnits) {
	
    int fieldCreateDistance = 10;
    
    // resize field properly
	for(int ii=0; ii<numUnits; ii++) {
        // check if unit is too close to left X boundary
        if (unit[ii].position[0] < fieldCreateDistance) {
            int addSizeX = fieldCreateDistance - unit[ii].position[0];
            sizeX += addSizeX;
            
            // move unit positions over
            for(int uu=0; uu<numUnits; uu++) {
                unit[uu].position[0] += addSizeX;
            }
        }
        
        // check if unit is too close to top Y boundary
        if (unit[ii].position[1] < fieldCreateDistance) {
            int addSizeY = fieldCreateDistance - unit[ii].position[1];
            sizeY += addSizeY;
            
            // move unit positions over
            for(int uu=0; uu<numUnits; uu++) {
                unit[uu].position[1] += addSizeY;
            }
        }
        
        // check if unit is too close to right X boundary
        if (sizeX - unit[ii].position[0] <= fieldCreateDistance) {
            sizeX += fieldCreateDistance - (sizeX - unit[ii].position[0]) + 1;
        }
        
        // check if unit is too close to bottom Y boundary
        if (sizeY - unit[ii].position[1] <= fieldCreateDistance) {
            sizeY += fieldCreateDistance - (sizeY - unit[ii].position[1]) + 1;
        }
    }
    
    makeBlankField(sizeX,sizeY);
	
	for(int ii=0; ii<numUnits; ii++) {
		if (!unit[ii].dead) {
			sIndex[unit[ii].position[1]][unit[ii].position[0]] = ii;
			sType[unit[ii].position[1]][unit[ii].position[0]] = unit[ii].type;
			occupied[unit[ii].position[1]][unit[ii].position[0]] = true;
			sHealth[unit[ii].position[1]][unit[ii].position[0]] = unit[ii].health;
			terrain[unit[ii].position[1]][unit[ii].position[0]] = 0;
		}
	}
	
	return;
}

void BattleField::makeBlankField(const int fSizeX, const int fSizeY) {
    // make a blank field
    
    int** blankSIndex = new int*[fSizeY];
	int** blankSType = new int*[fSizeY];
	int** blankOccupied = new int*[fSizeY];
	int** blankSHealth = new int*[fSizeY];
	int** blankTerrain = new int*[fSizeY];
	
	
	for(int jj = 0; jj < fSizeY; jj++) {
		blankSIndex[jj] = new int[fSizeX];
		blankSType[jj] = new int[fSizeX];
		blankOccupied[jj] = new int[fSizeX];
		blankSHealth[jj] = new int[fSizeX];
		blankTerrain[jj] = new int[fSizeX];
		
		for(int ii = 0; ii < fSizeX; ii++) {
			blankSIndex[jj][ii] = -1;
			blankSType[jj][ii] = 0;
			blankOccupied[jj][ii] = false;
			blankSHealth[jj][ii] = 0;
			blankTerrain[jj][ii] = 0;
		}
	}
    
    // delete old pointers
    /*for(int ii = 0; ii < sizeY; ii++) {
        delete[] sIndex[ii];
        delete[] sType[ii];
        delete[] occupied[ii];
        delete[] sHealth[ii];
        delete[] terrain[ii];
    }
     
    delete[] sIndex;
    delete[] sType;
    delete[] occupied;
    delete[] sHealth;
    delete[] terrain;*/
    
    // assign class pointers to blank field
    sIndex = blankSIndex;
    sType = blankSType;
    occupied = blankOccupied;
    sHealth = blankSHealth;
    terrain = blankTerrain;
}

void BattleField::printField(const int whichField) {
	int** fieldToPrint;
	
	switch (whichField) {
		case 1:
			fieldToPrint = sIndex;
		break;
		case 2:
			fieldToPrint = sType;
		break;
		case 3:
			fieldToPrint = occupied;
		break;
		case 4:
			fieldToPrint = sHealth;
		break;
		case 5:
			fieldToPrint = terrain;
		break;
	}
	
	for(int jj = 0; jj < sizeY; jj++) {
		for(int ii = 0; ii < sizeX; ii++) {
			cout << fieldToPrint[jj][ii];
			cout << " ";
		}
		cout << endl;
	}
    cout << endl;
}

BattleField::~BattleField() {
	for(int ii = 0; ii < sizeY; ii++) {
		delete[] sIndex[ii];
		delete[] sType[ii];
		delete[] occupied[ii];
		delete[] sHealth[ii];
		delete[] terrain[ii];
	}
	
	delete[] sIndex;
	delete[] sType;
	delete[] occupied;
	delete[] sHealth;
	delete[] terrain;
}