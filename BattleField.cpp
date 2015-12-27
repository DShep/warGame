/*
 *  Battlefield.cpp
 *  mattsWarGame
 *
 *  Created by Matt Creamer on 12/27/13.
 *  Copyright 2013 __MyCompanyName__. All rights reserved.
 *
 */

Battlefield::Battlefield(const int fSizeX, const int fSizeY) {
    // f before a variable refers to field
    // s before a variable refers to soldier
	sizeX = fSizeX;
	sizeY = fSizeY;

    // load terrain textures
    std::vector< std::string > tileTexturePath(4);
    tileTexturePath[0] = "textures\\tiles\\unassignedTexture.jpg";
    tileTexturePath[1] = "textures\\tiles\\grassTexture.jpg";
    tileTexturePath[2] = "textures\\tiles\\forestTexture.jpg";
    tileTexturePath[3] = "textures\\tiles\\riverTexture.jpg";

    tileTexture.resize(tileTexturePath.size());

    for(unsigned tt=0; tt<tileTexture.size(); tt++) {
        if (!tileTexture[tt].loadFromFile(tileTexturePath[tt])) {
            std::cerr << "tile texture not properly loaded" << std::endl;
        }
    }


        // update the battlefield size
    terrain.resize(sizeY);
    elevation.resize(sizeY);
    fog.resize(sizeY);
    selectedMoves.resize(sizeY);
    tileSprite.resize(sizeY);

    for (int yy=0; yy<sizeY; yy++) {
        terrain[yy].resize(sizeX,0);
        elevation[yy].resize(sizeX,0);
        fog[yy].resize(sizeX,0);
        selectedMoves[yy].resize(sizeX,false);
        tileSprite[yy].resize(sizeX);
    }

    generateTerrain();
}

void Battlefield::updateBattlefield(std::vector< std::vector< Soldier > > & unit, const sf::RenderWindow& battleFieldWindow) {
    // check for units that are close to the border and resize the field
    resizeBattlefield(unit);

    generateTerrain();

    // update the sprites to have the correct size and location
    updateSprite(battleFieldWindow);
}


void Battlefield::resizeBattlefield(std::vector< std::vector< Soldier > > & unit) {
// minimum and maximum solider X and Y positions
    int minX = fieldCreateDistance;
    int maxX = sizeX-fieldCreateDistance;
    int minY = fieldCreateDistance;
    int maxY = sizeY-fieldCreateDistance;

    // designate the amount to shift the battlefield by
    // non zero when soldiers are exploring new territory
    int leftShift = 0;
    int rightShift = 0;
    int bottomShift = 0;
    int topShift = 0;

    // first find the min and max values of Soldier X and Y positions
    // then make a battlefield that is large enough so that every soldier
    // is fieldCreateDistance from edges
    for(auto& playerInd: unit) {
        for(auto& unitInd: playerInd) {
            if (unitInd.positionX < minX)
                minX = unitInd.positionX;

            if (unitInd.positionX > maxX)
                maxX = unitInd.positionX;

            if (unitInd.positionY < minY)
                minY = unitInd.positionY;

            if (unitInd.positionY > maxY)
                maxY = unitInd.positionY;
        }
    }

    // find the max and min shift amounts
    // these determine how much the map will grow at each edge
    if ((minX - fieldCreateDistance) < 0)
        leftShift = fieldCreateDistance - minX;

    if ((maxX + fieldCreateDistance) > sizeX)
        rightShift = maxX + fieldCreateDistance - sizeX + 1;

    if ((minY - fieldCreateDistance) < 0)
        bottomShift = fieldCreateDistance - minY;

    if ((maxY + fieldCreateDistance) > sizeY)
        topShift = maxY + fieldCreateDistance - sizeY + 1;

    // now that we have the shift amounts update the soldier
    // locations relative to the edges
    for(auto& playerInd: unit) {
        for(auto& unitInd: playerInd) {
            unitInd.positionX += leftShift;
            unitInd.positionY += bottomShift;
        }
    }



    // update the battlefield size

    for (int bb=0; bb<bottomShift; bb++) {
        terrain.emplace(terrain.begin(),std::vector<int> (sizeX));
        elevation.emplace(elevation.begin(),std::vector<int> (sizeX));
        fog.emplace(fog.begin(),std::vector<int> (sizeX));
        selectedMoves.emplace(selectedMoves.begin(),std::vector<int> (sizeX));
        tileSprite.emplace(tileSprite.begin(),std::vector<sf::Sprite> (sizeX));
    }

    for (int tt=0; tt<topShift; tt++) {
        terrain.emplace_back(std::vector<int> (sizeX));
        elevation.emplace_back(std::vector<int> (sizeX));
        fog.emplace_back(std::vector<int> (sizeX));
        selectedMoves.emplace_back(std::vector<int> (sizeX));
        tileSprite.emplace_back(std::vector<sf::Sprite> (sizeX));
    }

    sizeX += leftShift;
    sizeX += rightShift;
    sizeY += bottomShift;
    sizeY += topShift;

    for (int yy=0; yy<sizeY; yy++) {
        for (int ll=0; ll<leftShift; ll++) {
            terrain[yy].emplace(terrain[yy].begin(),0);
            elevation[yy].emplace(elevation[yy].begin(),0);
            fog[yy].emplace(fog[yy].begin(),0);
            selectedMoves[yy].emplace(selectedMoves[yy].begin(),false);
            tileSprite[yy].emplace(tileSprite[yy].begin());
        }

        for (int rr=0; rr<rightShift; rr++) {
            terrain[yy].emplace_back(0);
            elevation[yy].emplace_back(0);
            fog[yy].emplace_back(0);
            selectedMoves[yy].emplace_back(false);
            tileSprite[yy].emplace_back();
        }
    }
}

void Battlefield::generateTerrain() {
    // currently set everything to default, will eventually need
    // to figure out how to add to the matricies while preserving
    // the old state

    for(int row=0; row<sizeY; row++) {
		for(int col=0; col<sizeX; col++) {
            if (terrain[row][col] == 0) {
                terrain[row][col] = 1;
                elevation[row][col] = 0;
                fog[row][col] = 1;
                selectedMoves[row][col] = false;
                tileSprite[row][col].setTexture(tileTexture[terrain[row][col]]);
            }
		}
	}
}

void Battlefield::updateSprite(const sf::RenderWindow& battleFieldWindow) {
    sf::Vector2u windowSize = battleFieldWindow.getSize();
    unsigned windowSizeX = windowSize.x;
    unsigned windowSizeY = windowSize.y;

    float spriteSizeX = (float) (windowSizeX) / (float) (sizeX);
    float spriteSizeY = (float) (windowSizeY) / (float) (sizeY);
    sf::Rect<float> tileTextureSize;

    // update the size and location of the tile sprites
    for(int row=0; row<sizeY; row++) {
		for(int col=0; col<sizeX; col++) {
            tileTextureSize = tileSprite[row][col].getLocalBounds();
            tileSprite[row][col].setScale(spriteSizeX/tileTextureSize.width, spriteSizeY/tileTextureSize.height);
            tileSprite[row][col].setPosition(sf::Vector2f(col*spriteSizeX,row*spriteSizeY));

            if (selectedMoves[row][col]) {
                tileSprite[row][col].setColor(sf::Color(128, 128, 255));
            } else {
                tileSprite[row][col].setColor(sf::Color(255, 255, 255));
            }
        }
    }
}


