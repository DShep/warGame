#include "BattleField.hpp"

int main() {
    // f before a variable refers to field
    // s before a variable refers to soldier
	int fSizeX = 10;
	int fSizeY = 10;
	int numUnit = 8;
	int numPlayer = 2;
	int windowSizeX = 900;
	int windowSizeY = 900;

	std::vector< std::vector< int > > unitType(numPlayer,std::vector< int > (numUnit));
	std::vector< std::vector< int > > unitLocationX(numPlayer,std::vector< int > (numUnit));
	std::vector< std::vector< int > > unitLocationY(numPlayer,std::vector< int > (numUnit));

	// Soldier is a class that contains all the information pertaining to a given unit
	// make a std::vector of std::vectors which contains a list of soldiers for each player;
	std::vector< std::vector< Soldier > > unit(numPlayer);

    // create the window for the game
    sf::RenderWindow battleFieldWindow(sf::VideoMode(windowSizeY, windowSizeX), "War Game");

    // resize the second dimension of this std::vector for every unit
    // in each player's army
    // each player can have a different number of unit
    // which is why its a 2d std::vector and not a matrix
    for(int pp=0; pp<numPlayer; pp++) {
        unit[pp].reserve(numUnit);

        for(int uu=0; uu<numUnit; uu++) {
            unitType[pp][uu] = uu/2;
            unitLocationX[pp][uu] = 3*uu;
            unitLocationY[pp][uu] = 3*pp;
            unit[pp].emplace_back(unitType[pp][uu],unitLocationX[pp][uu],unitLocationY[pp][uu],uu*numUnit+uu);
        }
    }

	// battlefield is a class that contains all the information for the current game board
	Battlefield bField(fSizeX,fSizeY);

    /////////////// GRAPHICS!!! //////////////
    while (battleFieldWindow.isOpen())
    {
        sf::Event event;
        while (battleFieldWindow.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                battleFieldWindow.close();

            if (event.type == sf::Event::Resized)
                battleFieldWindow.setView(sf::View(sf::FloatRect(0, 0, event.size.width, event.size.height)));
        }

        bField.updateBattlefield(unit,battleFieldWindow);

        ////////////////// CLEAR DISPLAY CYCLE /////////////////
        battleFieldWindow.clear();

        for(auto& row: bField.tileSprite) {
            for(auto& element: row) {
                battleFieldWindow.draw(element);
            }
        }

        for(auto& player: unit) {
            for(auto& element: player) {
                element.updateSprite(battleFieldWindow,bField.sizeX,bField.sizeY);
                battleFieldWindow.draw(element.unitSprite);
            }
        }
        battleFieldWindow.display();
    }

    return 0;
}
