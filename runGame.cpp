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
    sf::RenderWindow battlefieldWindow(sf::VideoMode(windowSizeY, windowSizeX), "War Game");
    sf::Vector2u windowSize = battlefieldWindow.getSize();
    // disable repeated events when you hold down a key
    battlefieldWindow.setKeyRepeatEnabled(false);
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
    while (battlefieldWindow.isOpen())
    {
        sf::Event event;
        while (battlefieldWindow.pollEvent(event))
        {
            switch (event.type) {
                case sf::Event::Closed:
                    battlefieldWindow.close();
                    break;

                case sf::Event::Resized:
                    battlefieldWindow.setView(sf::View(sf::FloatRect(0, 0, event.size.width, event.size.height)));
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
                    break;

                case sf::Event::MouseButtonPressed:
                    if (event.mouseButton.button == sf::Mouse::Left)
                    {
                        windowSize = battlefieldWindow.getSize();
                        windowSizeX = windowSize.x;
                        windowSizeY = windowSize.y;

                        mouseClickWindowFractionX = (float) event.mouseButton.x / (float) windowSize.x;
                        mouseClickWindowFractionY = (float) event.mouseButton.y / (float) windowSize.y;

                        mouseClickXInd = (int) ceil(mouseClickWindowFractionX * (float) bField.sizeX) - 1;
                        mouseClickYInd = (int) ceil(mouseClickWindowFractionY * (float) bField.sizeY) - 1;

                        if (bField.selectedMoves[mouseClickYInd][mouseClickXInd]) {
                            for(auto& player: unit) {
                                for(auto& element: player) {
                                    if (element.selected) {
                                        element.positionX = mouseClickXInd;
                                        element.positionY = mouseClickYInd;
                                    }
                                }
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

                        for(auto& col: bField.selectedMoves) {
                            for(auto& element: col) {
                                element = false;
                            }
                        }
                    }
                    break;

                case sf::Event::MouseEntered:
                    break;

                default:
                    break;
            }
        }

        bField.updateBattlefield(unit,battlefieldWindow);

        ////////////////// CLEAR DISPLAY CYCLE /////////////////
        battlefieldWindow.clear();

        for(auto& row: bField.tileSprite) {
            for(auto& element: row) {
                battlefieldWindow.draw(element);
            }
        }

        for(auto& player: unit) {
            for(auto& element: player) {
                element.updateSprite(battlefieldWindow,bField.sizeX,bField.sizeY);
                battlefieldWindow.draw(element.unitSprite);
            }
        }
        battlefieldWindow.display();
    }

    return 0;
}
