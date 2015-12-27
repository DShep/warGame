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

#include <SFML/Graphics.hpp>

class Soldier {
	private:
        void recMove(std::vector< std::vector< int > > & possibleMoves, int movesLeft, const int X, const int Y);
        sf::Texture unitTexture;
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
		bool selected;


        sf::Sprite unitSprite;

		Soldier(const int unitType, const int posX, const int posY, const int sIndexIn);
        void moveUnit(const std::vector< std::vector< Soldier > > unit, std::vector< std::vector< int > >& selected, const sf::RenderWindow& battleFieldWindow, const int battleFieldSizeX, const int battleFieldSizeY);
		void defend(const int attacker, Soldier* unitField, const int numunit, const int fSizeX, const int fSizeY);
		void updateSprite(const sf::RenderWindow& battleFieldWindow, const int battleFieldSizeX, const int battleFieldSizeY);
};
