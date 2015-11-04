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

    // how far a unit should be from the edge
    // resize map when closer
    fieldCreateDistance = 10;

    matrix<int> terrain(sizeY,sizeX); // terrain in a given square
    matrix<int> elevation(sizeY,sizeX);
    matrix<sf::Sprite> tileSprit(sizeY,sizeX);

    // load terrain textures
    vector< string > tileTexturePaths(3);
    tileTexturePaths[0] = "textures\\tiles\\grass.png";
    tileTexturePaths[1] = "textures\\tiles\\forest.png";
    tileTexturePaths[2] = "textures\\tiles\\river.png";

    vector< sf::Texture > tileTexture(3);

    for(auto& tt: tileTexture) {
        if (!tileTexture[tt].loadFromFile(tileTexturePaths[tt])) {
            std::cerr << "tile texture not properly loaded" << std::endl;
        }
    }

    setDefaultValues();
}

void setDefaultValues() {
    for(auto& rows: terrain) {
		for(auto& cols: rows) {
			terrain(rows,cols) = 0;
			eleavtion(rows,cols) = 0;
			tileSprite(rows,cols).setTexture(tileTexture[terrain(rows,cols)]);
		}
	}
}

void BattleField::resizeBattleField() {
    // resize the variables and don't keep the values
    terrain.resize(sizeY,sizeX,false);
    elevation.resize(sizeY,sizeX,false);
    tileSprite.resize(sizeY,sizeX,false);

    // currently set everything to default, will eventually need
    // to figure out how to add to the matricies while preserving
    // the old state
    setDefaultValues();
}

void BattleField::updateField(vector< vector< Soldier > > & unit) {
    // minimum and maximum solider X and Y positions
    int minX = fieldCreateDistance;
    int maxX = sizeX-fieldCreateDistance;
    int minY = fieldCreateDistance;
    int maxY = sizeY-fieldCreateDistance;

    // designate the amount to shift the battlefield by
    // non zero when soldiers are exploring new territory
    int minShiftX = 0;
    int maxShiftX = 0;
    int minShiftY = 0;
    int maxShiftY = 0;

    // first find the min and max values of Soldier X and Y positions
    // then make a battlefield that is large enough so that every soldier
    // is fieldCreateDistance from edges
    for(auto& pp: unit) {
        for(auto& uu: player) {
            if (unit[pp][uu].positionX < minX)
                minX = unit[pp][uu].positionX;

            if (unit[pp][uu].positionX > maxX)
                maxX = unit[pp][uu].positionX;

            if (unit[pp][uu].positionY < minY)
                minY = unit[pp][uu].positionY;

            if (unit[pp][uu].positionY > maxY)
                maxY = unit[pp][uu].positionY;
        }
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
    for(auto& pp: unit) {
        for(auto& uu: player) {
            unit[pp][uu].positionX += minShiftX;
            unit[pp][uu].positionY += minShiftY;
        }
    }

    // update the battlefield size
    sizeX += minShiftX;
    sizeX += maxShiftX;
    sizeY += minShiftY;
    sizeY += maxShiftY;

    // resize the field to have the correct size
    resizeBattleField();
}

void BattleField::printField(const int whichField) {
    float tileSpriteSizeX = (float) (windowSizeX) / (float) (sizeX);
    float tileSpriteSizeY = (float) (windowSizeY) / (float) (sizeY);

    // define the matrix of sprites that will be the battlefield
    matrix<sf::Sprite> tileSprites(sizeY,sizeX);
    matrix<sf::Sprite> unitSprites(sizeY,sizeX);

    // bind the grassTexture texture to the tileSprites
    for(unsigned yy=0; yy<sizeY; yy++) {
        for(unsigned xx=0; xx<sizeX; xx++) {
            tileSprites(yy,xx).setTexture(grassTexture);
            const sf::Rect<float> grassTextureSize = tileSprites(yy,xx).getGlobalBounds();
            tileSprites(yy,xx).setScale(tileSpriteSizeX/grassTextureSize.width,tileSpriteSizeY/grassTextureSize.height);
            tileSprites(yy,xx).setPosition(sf::Vector2f(xx*tileSpriteSizeX,yy*tileSpriteSizeY));
            //tileSprites(yy,xx).setColor(sf::Color(255, 0, 0));

            unitSprites(yy,xx).setTexture(soldierTexture);
            const sf::Rect<float> soldierTextureSize = unitSprites(yy,xx).getGlobalBounds();
            unitSprites(yy,xx).setScale(tileSpriteSizeX/soldierTextureSize.width,tileSpriteSizeY/soldierTextureSize.height);
            unitSprites(yy,xx).setPosition(sf::Vector2f(xx*tileSpriteSizeX,yy*tileSpriteSizeY));
            //unitSprites(yy,xx).setColor(sf::Color(255, 0, 0));
        }
    }

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        for(unsigned yy=0; yy<sizeY; yy++) {
            for(unsigned xx=0; xx<sizeX; xx++) {
                window.draw(tileSprites(yy,xx));
                window.draw(unitSprites(yy,xx));
            }
        }
        window.display();
    }
}
