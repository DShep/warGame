/*
 *  Battlefield.h
 *  mattsWarGame
 *
 *  Created by Catherine Creamer on 12/27/13.
 *  Copyright 2013 __MyCompanyName__. All rights reserved.
 *
 */

#include "Soldier.hpp"

class Battlefield {
    private:
        int windowSizeX;
        int windowSizeY;

        // how far a unit should be from the edge
        // resize map when closer
        const int fieldCreateDistance = 10;

        std::vector< std::vector< int > > terrain; // terrain in a given square
        std::vector< std::vector< int > > elevation;
        std::vector< std::vector< int > > fog;

        std::vector< sf::Texture > tileTexture;

        void generateTerrain();
        void updateSprite(const sf::RenderWindow& battleFieldWindow);
        void resizeBattlefield(std::vector< std::vector< Soldier > > & unit);
        void updateMap();
    public:
        int sizeX;
        int sizeY;
        Battlefield(const int fSizeX, const int fSizeY);
        void updateBattlefield(std::vector< std::vector< Soldier > > & unit, const sf::RenderWindow& battleFieldWindow);
        std::vector< std::vector< sf::Sprite > > tileSprite;
};
