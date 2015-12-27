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

	// mouse click variables
	int mouseClickXInd = 0;
	int mouseClickYInd = 0;
	float mouseClickWindowFractionX = 0;
	float mouseClickWindowFractionY = 0;

	bool tileSelectedStatus = false;
    bool unitSelectedStatus = false;
    int selectedUnitIndex = -1;
    int selectedUnitPlayer = -1;

	std::vector< std::vector< int > > unitType(numPlayer,std::vector< int > (numUnit));
	std::vector< std::vector< int > > unitLocationX(numPlayer,std::vector< int > (numUnit));
	std::vector< std::vector< int > > unitLocationY(numPlayer,std::vector< int > (numUnit));

	// Soldier is a class that contains all the information pertaining to a given unit
	// make a std::vector of std::vectors which contains a list of soldiers for each player;
	std::vector< std::vector< Soldier > > unit(numPlayer);

    // create the window for the game
    sf::RenderWindow battleFieldWindow(sf::VideoMode(windowSizeY, windowSizeX), "War Game");
    sf::Vector2u windowSize = battleFieldWindow.getSize();
    // disable repeated events when you hold down a key
    battleFieldWindow.setKeyRepeatEnabled(false);
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
            switch (event.type) {
                case sf::Event::Closed:
                    battleFieldWindow.close();
                    break;

                case sf::Event::Resized:
                    battleFieldWindow.setView(sf::View(sf::FloatRect(0, 0, event.size.width, event.size.height)));
                    break;

                case sf::Event::KeyPressed:
                    selectedUnitPlayer = -1;
                    selectedUnitIndex = -1;
                    if (event.key.code == sf::Keyboard::W) {
                        for(int pp=0; pp<numPlayer; pp++) {
                            for(int uu=0; uu<numUnit; uu++) {
                                if (unit[pp][uu].selected) {
                                    selectedUnitPlayer = pp;
                                    selectedUnitIndex = uu;
                                }
                            }
                        }
                    }

                    if (~(selectedUnitIndex == -1)) {
                        unit[selectedUnitPlayer][selectedUnitIndex].moveUnit(unit,bField.selectedMoves,bField.sizeX,bField.sizeY);
                    }
                    break;

                case sf::Event::MouseWheelMoved:
                        std::cout << "wheel movement: " << event.mouseWheel.delta << std::endl;
                        std::cout << "mouse x: " << event.mouseWheel.x << std::endl;
                        std::cout << "mouse y: " << event.mouseWheel.y << std::endl;
                    break;

                case sf::Event::MouseButtonPressed:
                    if (event.mouseButton.button == sf::Mouse::Left)
                    {
                        windowSize = battleFieldWindow.getSize();
                        windowSizeX = windowSize.x;
                        windowSizeY = windowSize.y;

                        mouseClickWindowFractionX = (float) event.mouseButton.x / (float) windowSize.x;
                        mouseClickWindowFractionY = (float) event.mouseButton.y / (float) windowSize.y;

                        mouseClickXInd = (int) ceil(mouseClickWindowFractionX * (float) bField.sizeX) - 1;
                        mouseClickYInd = (int) ceil(mouseClickWindowFractionY * (float) bField.sizeY) - 1;

                        tileSelectedStatus = bField.selected[mouseClickYInd][mouseClickXInd];

                        // loop through all the battlefield locations and find which square was clicked on
                        for(auto& col: bField.selected) {
                            for(auto& element: col) {
                                element = false;
                            }
                        }

                        for(auto& player: unit) {
                            for(auto& element: player) {
                                unitSelectedStatus = element.selected;

                                if ((mouseClickXInd == element.positionX) && (mouseClickYInd == element.positionY)) {
                                    element.selected = !unitSelectedStatus;
                                } else {
                                    element.selected = false;
                                }
                            }
                        }

                        bField.selected[mouseClickYInd][mouseClickXInd] = !tileSelectedStatus;
                    }
                    break;

                case sf::Event::MouseEntered:
                    break;

                default:
                    break;
            }
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
