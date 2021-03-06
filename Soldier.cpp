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
	// honestly i haven't sat down to understand exactly
	// how this works but it does
    std::vector <std::vector <std::string> > stats;
    std::ifstream statsFile("unitVals.csv");

    while (statsFile)
    {
        std::string tempReadOfLine;
        if (!getline( statsFile, tempReadOfLine )) break;

        std::istringstream thisLine( tempReadOfLine );
        std::vector <std::string> record;

        while (thisLine)
        {
            std::string tempReadOfChar;
            if (!getline( thisLine, tempReadOfChar, ',' )) break;
            record.push_back( tempReadOfChar );
        }

        stats.push_back( record );
    }

    if (!statsFile.eof()) {
        std::cerr << "Error reading stats file\n";
    }

    dead = false;
    selected = false;

	// set up the values for this object
    sIndex = sIndexIn;
	type = unitType;
	name = stats[0][type+1];
	population = atoi(stats[2][type+1].c_str());
	health = atoi(stats[3][type+1].c_str());
	damage = atoi(stats[4][type+1].c_str());
	range = atoi(stats[5][type+1].c_str());
	speed = atoi(stats[6][type+1].c_str());
	retreatSpeed = atoi(stats[7][type+1].c_str());
	vision = atoi(stats[8][type+1].c_str());

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

    if (selected) {
        unitSprite.setColor(sf::Color(255, 128, 128));
    } else {
        unitSprite.setColor(sf::Color(255, 255, 255));
    }
}

void Soldier::recMove(std::vector< std::vector< int > > & possibleMoves, int movesLeft, const int X, const int Y) {
    for (int xx=X-1; xx<X+2; xx++) {
        for (int yy=Y-1; yy<Y+2; yy++) {
            if (movesLeft > 0) {
                if (possibleMoves[yy][xx] != 0) {
                    possibleMoves[yy][xx] = 1;
                    movesLeft -= 1;
                    recMove(possibleMoves,movesLeft,xx,yy);
                    movesLeft += 1;
                }
            }
        }
    }
}

void Soldier::moveUnit(const std::vector< std::vector< Soldier > > unit, std::vector< std::vector< int > >& selectedMoves, const int battlefieldSizeX, const int battlefieldSizeY) {
    // make a matrix to hold the places the unit can move
    // 0 is a place you can't move
    // 1 is a place you can move
    // 2 is unoccupied and the algorithm will change it to 0 or 1 based
    // on unit positions
	std::vector< std::vector< int > > possibleMoves;

    for(int yy=0; yy<battlefieldSizeY; yy++) {
        possibleMoves.emplace_back(std::vector<int> (battlefieldSizeX));

        for(int xx=0; xx<battlefieldSizeX; xx++) {
            possibleMoves[yy].emplace_back(0);
        }
    }



    // initialize all possible moves to unoccupied
    for(auto& col: possibleMoves) {
        for(auto& element: col) {
			element = 2; // 2 stands for unoccupied
		}
	}

    // get rid of all squares where another unit is
    for(auto& player: unit) {
        for(auto& element: player) {
            possibleMoves[element.positionY][element.positionX] = 0;
        }
    }

    // test every possible path through unoccupied (possibleMoves = 2) squares
    recMove(possibleMoves,speed,positionX,positionY);

	for(int yy=0; yy<battlefieldSizeY; yy++) {
		for(int xx=0; xx<battlefieldSizeX; xx++) {
			if (possibleMoves[yy][xx] == 1) {
                selectedMoves[yy][xx] = true;
			} else {
                selectedMoves[yy][xx] = false;
			}
		}
	}
}


/*
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
