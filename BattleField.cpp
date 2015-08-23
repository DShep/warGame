/*
 *  BattleField.cpp
 *  mattsWarGame
 *
 *  Created by Matt Creamer on 12/27/13.
 *  Copyright 2013 __MyCompanyName__. All rights reserved.
 *
 */

#include "BattleField.hpp"

BattleField::BattleField(const int fSizeX, const int fSizeY) {
    // f before a variable refers to field
    // s before a variable refers to soldier
	sizeX = fSizeX;
	sizeY = fSizeY;

    // each of these matricies can derived from sIndex which could
    // find the soldier at that location. is it useful to have all as separate matricies?
    matrix<int> sIndex(sizeY,sizeX); // unique indices for each soldier
    matrix<int> sType(sizeY,sizeX); // integers which describes the type of each soldier
    matrix<bool> occupied(sizeY,sizeX); // matrix of whether a given square is occupied
    matrix<int> sHealth(sizeY,sizeX); // health of the unit in the given square
    matrix<int> terrain(sizeY,sizeX); // terrain in a given square

    setDefaultValues();
}

void BattleField::updateField(Soldier* unit, const int numUnits) {

    // distance from any unit to render the field
    int fieldCreateDistance = 10;

    // minimum and maximum solider X and Y positions
    int minX = fieldCreateDistance;
    int maxX = sizeX-fieldCreateDistance;
    int minY = fieldCreateDistance;
    int maxY = sizeY-fieldCreateDistance;

    // designate the amount to shift the battlefield by
    // non zero when soldiers are exploring new territory
    int minShiftX;
    int maxShiftX;
    int minShiftY;
    int maxShiftY;

    // first find the min and max values of Soldier X and Y positions
    // then make a battlefield that is large enough so that every soldier
    // is fieldCreateDistance from edges
    for(int ss = 0; ss < numUnits; ss++) {
        if (unit[ss].positionX < minX)
            minX = unit[ss].positionX;

        if (unit[ss].positionX > maxX)
            maxX = unit[ss].positionX;

        if (unit[ss].positionY < minY)
            minY = unit[ss].positionY;

        if (unit[ss].positionY > maxY)
            maxY = unit[ss].positionY;
    }

    // find the max and min shift amounts
    // these determine how much the map will grow at each edge
    if ((minX - fieldCreateDistance) < 0)
        minShiftX = fieldCreateDistance - minX;

    if ((maxX + fieldCreateDistance) > sizeX)
        maxShiftX = maxX + fieldCreateDistance - sizeX;

    if ((minY - fieldCreateDistance) < 0)
        minShiftY = fieldCreateDistance - minY;

    if ((maxY + fieldCreateDistance) > sizeY)
        maxShiftY = maxY + fieldCreateDistance - sizeY;

    // now that we have the shift amounts update the soldier
    // locations relative to the edges
    for(int ss=0; ss<numUnits; ss++) {
        unit[ss].positionX += minShiftX;
        unit[ss].positionY += minShiftY;
    }

    // update the battlefield size
    sizeX += minShiftX;
    sizeX += maxShiftX;
    sizeY += minShiftY;
    sizeY += maxShiftY;

    // resize the field to have the correct size
    resizeBattleField();

    // set the field current state
    setCurrentValues(unit,numUnits);

	return;
}

void BattleField::resizeBattleField() {
    // resize the variables and don't keep the values
    sIndex.resize(sizeY,sizeX,false);
    sType.resize(sizeY,sizeX,false);
    occupied.resize(sizeY,sizeX,false);
    sHealth.resize(sizeY,sizeX,false);
    terrain.resize(sizeY,sizeX,false);
}

void BattleField::setDefaultValues() {
    // initialize the values of the matricies
	for(int yy = 0; yy < sizeY; yy++) {
		for(int xx = 0; xx < sizeX; xx++) {
			sIndex(yy,xx) = -1;
			sType(yy,xx) = 0;
			occupied(yy,xx) = false;
			sHealth(yy,xx) = 0;
			terrain(yy,xx) = 0;
		}
	}
}

void BattleField::setCurrentValues(Soldier* unit,const int numUnits) {\
    // call setDefaultValues first to make sure all data
    // is in default position
    setDefaultValues();

    // temporary variables to hold unit position
    // to improve code readability
    int unitX;
    int unitY;

	for(int ss=0; ss<numUnits; ss++) {
		if (!unit[ss].dead) {
            unitX = unit[ss].positionX;
            unitY = unit[ss].positionY;

			sIndex(unitY,unitX) = ss;
			sType(unitY,unitX) = unit[ss].type;
			occupied(unitY,unitX) = true;
			sHealth(unitY,unitX) = unit[ss].health;
			terrain(unitY,unitX) = 0;
		}
	}
}

void BattleField::printField(const int whichField) {
    matrix<int> fieldToPrint;

	switch (whichField) {
		case 1:
			fieldToPrint = sIndex;
		break;
		case 2:
			fieldToPrint = sType;
		break;
		case 3:
			fieldToPrint = matrix<int> (occupied);
		break;
		case 4:
			fieldToPrint = sHealth;
		break;
		case 5:
			fieldToPrint = terrain;
		break;
	}

	/*for(int jj = 0; jj < sizeY; jj++) {
		for(int ii = 0; ii < sizeX; ii++) {
			cout << fieldToPrint[jj][ii];
			cout << " ";
		}
		cout << endl;
	}
    cout << endl;*/

    std::cout << fieldToPrint << std::endl;
}
