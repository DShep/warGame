#include "BattleField.h"

int main() {
	int fSizeX = 10;
	int fSizeY = 10;
	int numUnits = 8;
	//int numPlayers = 1;
	int unitType[numUnits];
	int unitLocation[2][numUnits];
	
	Soldier* unit = new Soldier[numUnits];
	BattleField bField(fSizeX,fSizeY);
	
	for(int ii=0; ii<numUnits; ii++) {
		unitType[ii] = ii+1;
		unitLocation[1][ii] = 2*ii;
		unitLocation[0][ii] = 0;
		
		unit[ii].initSoldier(unitType[ii],unitLocation[1][ii],unitLocation[0][ii],ii);
	}
	
	bField.updateField(unit,numUnits);
	
	bField.printField(2);

    int* noAtt = new int[2];
    noAtt[0] = -1;
    noAtt[1] = -1;
    unit[2].move(noAtt,unit,numUnits,bField.sizeX,bField.sizeY);
    bField.updateField(unit,numUnits);
    bField.printField(2);
    
    delete[] unit;
    
    return 0;
}