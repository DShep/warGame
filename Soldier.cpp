/*
 *  Soldier.cpp
 *  mattsWarGame
 *
 *  Created by Catherine Creamer on 12/23/13.
 *  Copyright 2013 __MyCompanyName__. All rights reserved.
 *
 */

#include "Soldier.hpp"

Soldier::Soldier(const int unitType, const int posX, const int posY, const int sIndexIn){
	const int numUnitTypes = 8;
	const int numStats = 8;
	// Load in the stats file for each type of soldier
	std::ifstream statFile;
	statFile.open("unitVals.csv",std::ios::in);
	std::string stats[numStats+1][numUnitTypes+1];

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

    // load a texture for the unit
    std::vector< std::string > unitTexturePaths(5);
    unitTexturePaths[0] = "textures\\units\\footmanTexture.png";
    unitTexturePaths[1] = "textures\\units\\pikemanTexture.png";
    unitTexturePaths[2] = "textures\\units\\lightCalveryTexture.png";
    unitTexturePaths[3] = "textures\\units\\heavyCalveryTexture.png";
    unitTexturePaths[4] = "textures\\units\\archerTexture.png";

    if (!unitTexture.loadFromFile(unitTexturePaths[type])) {
        std::cerr << "unit texture not properly loaded" << std::endl;
        exit(EXIT_FAILURE);
    }

    // create a sprite for the unit
    unitSprite.setTexture(unitTexture);
}

void Soldier::updateSprite(const sf::RenderWindow& battleFieldWindow, const int battleFieldSizeX, const int battleFieldSizeY) {
    sf::Vector2u windowSize = battleFieldWindow.getSize();
    unsigned windowSizeX = windowSize.x;
    unsigned windowSizeY = windowSize.y;

    float spriteSizeX = (float) (windowSizeX) / (float) (battleFieldSizeX);
    float spriteSizeY = (float) (windowSizeY) / (float) (battleFieldSizeY);

    sf::Rect<float> tileTextureSize = unitSprite.getLocalBounds();
    unitSprite.setScale(spriteSizeX/tileTextureSize.width,spriteSizeY/tileTextureSize.height);
    unitSprite.setPosition(sf::Vector2f(positionX*spriteSizeX,positionY*spriteSizeY));
}
/*
bool Soldier::moveUnit(const std::vector< std::vector< Soldier > > unit, const int fSizeX, const int fSizeY) {
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
    for(int pp=0; pp<unit.size(); pp++) {
        for(int uu=0; uu<unit[pp].size(); uu++) {
            if (uu != sIndex)
                possibleMoves(unit[pp][uu].positionY,unit[pp][uu].positionX) = 0;
        }
    }

    // test every possible path through unoccupied (possibleMoves = 2) squares
    recMove(possibleMoves,speed,positionX,positionY);

    // set up two variables to keep track of whether we can move
    // and whether we chose to move
	bool canMove = false;
	bool didMove = false;

	for(int xx=1; xx<fSizeX; xx++) {
		for(int yy=1; yy<fSizeY; yy++) {
			if (possibleMoves(yy,xx) == 1) {
                canMove = true;
			}
		}
	}

    // current interface with the user for deciding where to move
	if (canMove) {
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
    for(int uu=0; uu<numunit; uu++) {
        if (uu != sIndex)
            if unitPositions(yy,xx) > 0
                possibleMoves(unit[uu].positionY,unit[uu].positionX) = 0;
    }

	if(retreat)
    {
        // get rid of all unavaible squares for retreating this means anything next to a melee unit
        for(int uu=0; uu<numunit; uu++) {
            if((uu != sIndex) && (unit[uu].type != 5)) {
                for(int reachX=unit[uu].positionX-1; reachX<unit[uu].positionX+2; reachX++) {
                    for(int reachY=unit[uu].positionY-1; reachY<unit[uu].positionY+2; reachY++) {
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

    // test every possible path through unoccupied (possibleMoves = 2) squares
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
		std:cin >> moveX;
		cout << "where do you want to move Y?" << endl;
		std:cin >> moveY;

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

void Soldier::recMove(matrix<int> & posMove, int movesLeft, const int X, const int Y) {
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

void Solider::attack() {
    // identify positions that are within range
    targets =

}

void Soldier::defend(const int attacker,Soldier* unit, const int numunit, const int fSizeX, const int fSizeY) {
	bool ret = moveUnit(unit[attacker].positionX,unit[attacker].positionY,unit,numunit,fSizeX,fSizeY);

	if (ret) {
		health -= int (unit[attacker].damage/2);
	} else {
		health -= unit[attacker].damage;
	}

	if (health < 1)
		dead = true;

	return;

}*/
