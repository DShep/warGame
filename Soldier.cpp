/*
 *  Soldier.cpp
 *  mattsWarGame
 *
 *  Created by Catherine Creamer on 12/23/13.
 *  Copyright 2013 __MyCompanyName__. All rights reserved.
 *
 */

#include "Soldier.h"

Soldier::Soldier(){
	
}

void Soldier::initSoldier(const int unitType, const int posX, const int posY, const int sIndexIn){
	const int numUnitTypes = 8;
	const int numStats = 8;
	// Load in the stats file for each type of soldier
	ifstream statFile;
	statFile.open("/Users/Matt/Documents/warGame/warGame/unitVals.csv",ios::in);
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
	
	position[0] = posX;
	position[1] = posY;
	
}

bool Soldier::move(const int* attPos, Soldier* allUnits, const int numUnits, const int fSizeX, const int fSizeY) {
	bool retreat;
	int funcSpeed;
	
    // decide whether you're going to retreat
	if (attPos[0] == -1) {
		retreat = false;
		funcSpeed = speed;
	} else {
		retreat = true;
		funcSpeed = retreatSpeed;
	}

	int** possibleMoves = new int*[fSizeY];
	
    // initialize all possible moves to false
    for(int yy=0; yy<fSizeY; yy++) {
		possibleMoves[yy] = new int[fSizeX];
		for(int xx=0; xx<fSizeX; xx++) {
            // 2 stands for unoccupied
			possibleMoves[yy][xx] = 2;
		}
	}
    
	bool canMove = false;
	bool didMove = false;
    
    // get rid of all squares where another unit is
    for(int uu=0; uu<numUnits; uu++) {
        if (uu != sIndex) {
            possibleMoves[allUnits[uu].position[1]][allUnits[uu].position[0]] = 0;
        }
    }
	
	if(retreat)
    {
        // get rid of all unavaible squares for retreating this means anything next to a melee unit
        for(int uu=0; uu<numUnits; uu++) {
            if((uu != sIndex) && (allUnits[uu].type != 5)) {
                for(int reachX=allUnits[uu].position[0]-1; reachX<allUnits[uu].position[0]+2; reachX++) {
                    for(int reachY=allUnits[uu].position[1]-1; reachY<allUnits[uu].position[1]+2; reachY++) {
                        if ((reachX > -1) && (reachX < fSizeX+1)) {
                            if ((reachY > -1) && (reachY < fSizeY+1)) {
                                // 0 stands for unavailable
                                possibleMoves[reachY][reachX] = 0;
                            }
                        }
                    }
                }
            }
        }
    } else {
        
    }
    
    /*for(int jj = 0; jj < fSizeY; jj++) {
        for(int ii = 0; ii < fSizeX; ii++) {
            cout << possibleMoves[jj][ii];
            cout << " ";
        }
        cout << endl;
    }
    cout << endl;*/
    
    // test every possible path through unnoccupied (possibleMoves = 2) squares
    recMove(possibleMoves,funcSpeed,position[0],position[1]);
    
	for(int xx=1; xx<fSizeX; xx++) {
		for(int yy=1; yy<fSizeY; yy++) {
			if (possibleMoves[yy][xx] == 1) {
                canMove = true;
			}
		}
	}
    
	if (canMove) {
		int moveX;
		int moveY;
		
		for(int jj = 0; jj < fSizeY; jj++) {
            for(int ii = 0; ii < fSizeX; ii++) {
                cout << possibleMoves[jj][ii];
                cout << " ";
            }
            cout << endl;
        }
        cout << endl;
        
		cout << "where do you want to move X?" << endl;
		cin >> moveX;
		cout << "where do you want to move Y?" << endl;
		cin >> moveY;
		
		if (possibleMoves[moveY][moveX] == 1) {
			position[1] = moveY;
			position[0] = moveX;
			didMove = true;
			cout << "successful move" << endl;
		} else {
            cout << "bad entry no move for you" << endl;
		}
	}
	
	// delete the new variable for memory n shit
	for(int yy=0; yy<fSizeY; yy++) {
		delete[] possibleMoves[yy];
	}
	
	delete[] possibleMoves;
	
	return didMove;
}

void Soldier::recMove(int** posMove, int movesLeft, const int X, const int Y) {
    for (int xx=X-1; xx<X+2; xx++) {
        for (int yy=Y-1; yy<Y+2; yy++) {
            if (movesLeft > 0) {
                if (posMove[yy][xx] != 0) {
                    posMove[yy][xx] = 1;
                    movesLeft -= 1;
                    recMove(posMove,movesLeft,xx,yy);
                    movesLeft += 1;
                }
            }
        }
    }
}

void Soldier::defend(const int attacker,Soldier* allUnits, const int numUnits, const int fSizeX, const int fSizeY) {
	bool ret = move(allUnits[attacker].position,allUnits,numUnits,fSizeX,fSizeY);
	
	if (ret) {
		health -= allUnits[attacker].damage/2;
	} else {
		health -= allUnits[attacker].damage;
	}
	
	if (health < 1)
		dead = true;
	
	return;
}