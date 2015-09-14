/*
 *  soldier.h
 *  mattsWarGame
 *
 *  Created by Catherine Creamer on 12/23/13.
 *  Copyright 2013 __MyCompanyName__. All rights reserved.
 *
 */
#include <algorithm>	// std::min
#include <string>		//string
#include <iostream>
#include <fstream>

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>

using namespace boost::numeric::ublas;

class Soldier {
	private:
		void recMove(matrix<int> posMove, int movesLeft, const int X, const int Y);

	public:
		std::string name;
        int sIndex;
		int type;
		int health;
		int damage;
		int range;
		int speed;
		int population;
		int positionX;
		int positionY;
		int retreatSpeed;
		int vision;
		bool dead;

		Soldier();
		void initSoldier(const int unitType, const int posX, const int posY, const int index);
		bool moveUnit(const int attPosX, const int attPosY, Soldier* allunit, const int numunit, const int fSizeX, const int fSizeY);
		void defend(const int attacker, Soldier* unitField, const int numunit, const int fSizeX, const int fSizeY);
};
