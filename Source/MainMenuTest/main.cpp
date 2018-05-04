#include "MainMenu.h"
#include <iostream>


using namespace std;

namespace MainMenuTest {

	void Test() {

		sf::RenderWindow rw(sf::VideoMode(1200, 800), "Asteroids");
		//WindowResolution::SetResolution(rw);
		
		UI ui(rw);
		sf::Font font;
		font.loadFromFile("");

		MainMenu *mMenu = MainMenu::Create(sf::Vector2f(1200, 800), font, ui);
		Status gameStatus = MAIN_MENU;

		while (rw.isOpen() && (gameStatus != EXIT)){

			if (gameStatus == MAIN_MENU) {
				
				mMenu->Draw();
				mMenu->HandleButtons();

				gameStatus = mMenu->GetCurrentStatus();
			}

			rw.display();
		}

	}

}

int main() {

	MainMenuTest::Test();

	return 0;

}