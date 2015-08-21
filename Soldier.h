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
using namespace std;

class Soldier {
	private:
		void recMove(int** posMove, int movesLeft, const int X, const int Y);
	
	public:
		string name;
        int sIndex;
		int type;
		int health;
		int damage;
		int range;
		int speed;
		int population;
		int position[2];
		int retreatSpeed;
		int vision;
		bool dead;
		
		Soldier();
		void initSoldier(const int unitType, const int posX, const int posY, const int index);
		bool move(const int* attPos, Soldier* allUnits, const int numUnits, const int fSizeX, const int fSizeY);
		void defend(const int attacker, Soldier* unitField, const int numUnits, const int fSizeX, const int fSizeY);
};