/*
 *  Soldier.cpp
 *  mattsWarGame
 *
 *  Created by Catherine Creamer on 12/23/13.
 *  Copyright 2013 __MyCompanyName__. All rights reserved.
 *
 */

#include "Soldier.hpp"

Soldier::Soldier(){

}

void Soldier::initSoldier(const int unitType, const int posX, const int posY, const int sIndexIn){
	const int numUnitTypes = 8;
	const int numStats = 8;
	// Load in the stats file for each type of soldier
	ifstream statFile;
	statFile.open("C:/Documents/Matt/cppPrograms/warGame/unitVals.csv",ios::in);
	string stats[numStats+1][numUnitTypes+1];

	for (int jj=0;jj<numStats+1;jj++) {
		for (int ii=0;ii<numUnitTypes+1;ii++) {
			getline(statFile,stats[jj][ii],',');
		}
	}

	statFile.close();

	// set up the values for this object
    sIndex = sIndexIn;
	type = unitType;
	name = stats[0][type];
	population = atoi(stats[2][type].c_str());
	health = atoi(stats[3][type].c_str());
	damage = atoi(stats[4][type].c_str());
	range = atoi(stats[5][type].c_str());
	speed = atoi(stats[6][type].c_str());
	retreatSpeed = atoi(stats[7][type].c_str());
	vision = atoi(stats[8][type].c_str());

	positionX = posX;
	positionY = posY;

}

bool Soldier::moveUnit(const vector<vector<Soldier>> units, const int fSizeX, const int fSizeY) {
    // make a matrix to hold the places the unit can move
    // 0 is a place you can't move
    // 1 is a place you can move
    // 2 is unoccupied and the algorithm will change it to 0 or 1 based
    // on unit positions
	matrix<int> possibleMoves(fSizeY,fSizeX);

    // initialize all possible moves to unoccupied
    for(int yy=0; yy<fSizeY; yy++) {
		for(int xx=0; xx<fSizeX; xx++) {
			possibleMoves(yy,xx) = 2; // 2 stands for unoccupied
		}
	}

    // get rid of all squares where another unit is
    for(int pp=0; uu<units.; uu++) {
        for(int uu=0; uu<numUnits; uu++) {
            if (uu != sIndex)
                if unitPositions(yy,xx) > 0
                    possibleMoves(units[uu].positionY,units[uu].positionX) = 0;
        }
    }

    // test every possible path through unnoccupied (possibleMoves = 2) squares
    recMove(possibleMoves,currentSpeed,positionX,positionY);

	bool canMove = false;
	bool didMove = false;

	for(int xx=1; xx<fSizeX; xx++) {
		for(int yy=1; yy<fSizeY; yy++) {
			if (possibleMoves(yy,xx) == 1) {
                canMove = true;
			}
		}
	}

	if (canMove) {
		int moveX;
		int moveY;

		for(int jj = 0; jj < fSizeY; jj++) {
            for(int ii = 0; ii < fSizeX; ii++) {
                cout << possibleMoves(jj,ii);
                cout << " ";
            }
            cout << endl;
        }
        cout << endl;

		cout << "where do you want to move X?" << endl;
		cin >> moveX;
		cout << "where do you want to move Y?" << endl;
		cin >> moveY;

		if (possibleMoves(moveY,moveX) == 1) {
			positionY = moveY;
			positionX = moveX;
			didMove = true;
			cout << "successful move" << endl;
		} else {
            cout << "bad entry no move for you" << endl;
		}
	}

	return didMove;
}

bool Soldier::retreatUnit(matrix<int> unitPositions,bool retreat) {
    // this whole function needs to be reworked
	int currentSpeed;

    // choose the speed of the unit based on whether they are retreating
	if (retreat) {
		currentSpeed = retreatSpeed;
	} else {
		currentSpeed = speed;
	}

    // make a matrix to hold the places the unit can move
    // 0 is a place you can't move
    // 1 is a place you can move
    // 2 is unoccupied and the algorithm will change it to 0 or 1 based
    // on unit positions
	matrix<int> possibleMoves(unitPositions.size1,unitPositions.size2);

    // initialize all possible moves to unoccupied
    for(int yy=0; yy<fSizeY; yy++) {
		for(int xx=0; xx<fSizeX; xx++) {
			possibleMoves(yy,xx) = 2; // 2 stands for unoccupied
		}
	}

    // get rid of all squares where another unit is
    for(int uu=0; uu<numUnits; uu++) {
        if (uu != sIndex)
            if unitPositions(yy,xx) > 0
                possibleMoves(units[uu].positionY,units[uu].positionX) = 0;
    }

	if(retreat)
    {
        // get rid of all unavaible squares for retreating this means anything next to a melee unit
        for(int uu=0; uu<numUnits; uu++) {
            if((uu != sIndex) && (units[uu].type != 5)) {
                for(int reachX=units[uu].positionX-1; reachX<units[uu].positionX+2; reachX++) {
                    for(int reachY=units[uu].positionY-1; reachY<units[uu].positionY+2; reachY++) {
                        if ((reachX > -1) && (reachX < fSizeX+1)) {
                            if ((reachY > -1) && (reachY < fSizeY+1)) {
                                // 0 stands for unavailable
                                possibleMoves(reachY,reachX) = 0;
                            }
                        }
                    }
                }
            }
        }
    } else {

    }

    // test every possible path through unnoccupied (possibleMoves = 2) squares
    recMove(possibleMoves,currentSpeed,positionX,positionY);

	bool canMove = false;
	bool didMove = false;

	for(int xx=1; xx<fSizeX; xx++) {
		for(int yy=1; yy<fSizeY; yy++) {
			if (possibleMoves(yy,xx) == 1) {
                canMove = true;
			}
		}
	}

	if (canMove) {
		int moveX;
		int moveY;

		for(int jj = 0; jj < fSizeY; jj++) {
            for(int ii = 0; ii < fSizeX; ii++) {
                cout << possibleMoves(jj,ii);
                cout << " ";
            }
            cout << endl;
        }
        cout << endl;

		cout << "where do you want to move X?" << endl;
		cin >> moveX;
		cout << "where do you want to move Y?" << endl;
		cin >> moveY;

		if (possibleMoves(moveY,moveX) == 1) {
			positionY = moveY;
			positionX = moveX;
			didMove = true;
			cout << "successful move" << endl;
		} else {
            cout << "bad entry no move for you" << endl;
		}
	}

	return didMove;
}

void Soldier::recMove(matrix<int> posMove, int movesLeft, const int X, const int Y) {
    for (int xx=X-1; xx<X+2; xx++) {
        for (int yy=Y-1; yy<Y+2; yy++) {
            if (movesLeft > 0) {
                if (posMove(yy,xx) != 0) {
                    posMove(yy,xx) = 1;
                    movesLeft -= 1;
                    recMove(posMove,movesLeft,xx,yy);
                    movesLeft += 1;
                }
            }
        }
    }
}

void Soldier::defend(const int attacker,Soldier* units, const int numUnits, const int fSizeX, const int fSizeY) {
	bool ret = moveUnit(units[attacker].positionX,units[attacker].positionY,units,numUnits,fSizeX,fSizeY);

	if (ret) {
		health -= int (units[attacker].damage/2);
	} else {
		health -= units[attacker].damage;
	}

	if (health < 1)
		dead = true;

	return;
}
