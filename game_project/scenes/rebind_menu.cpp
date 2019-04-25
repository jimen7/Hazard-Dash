#define _GLIBCXX_USE_NANOSLEEP
#include "rebind_menu.h"
#include "../components/cmp_text.h"
#include "../components/cmp_sprite.h"
#include "../game.h"
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Joystick.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <SFML/Config.hpp>
#include <thread>

using namespace std;
using namespace sf;

shared_ptr<sf::Texture> tex3;
Sprite background3;
shared_ptr<Entity> backEnt3;
shared_ptr<Entity> txt4;
float dtRebind;
bool isAssigned = true;

void callback(const std::string& data)
{
	std::cout << "Callback called because: " << data << '\n';
}


void task(int time)
{
	//while (!isAssigned) {
				
			//}

	std::this_thread::sleep_for(std::chrono::seconds(time));
	callback("async task done");
}


void RebindScene::Load() {
	dtRebind = 0.0f;
	{
		tex3 = make_shared<sf::Texture>();
		if (!tex3->loadFromFile("res/Sprites/Menu.png"))
		{
			throw ("Can't load menu image.");
		}
		backEnt3 = makeEntity();
		auto s = backEnt3->addComponent<SpriteComponent>();
		s->setTexure(tex3);
		// Text
		txt4 = makeEntity();
		// TODO Replace insert with actual values
		auto t1 = txt4->addComponent<TextComponent>("Jump: Assigned");
		auto t2 = txt4->addComponent<TextComponent>("Movement: Assigned");
		auto t3 = txt4->addComponent<TextComponent>("");
		auto t4 = txt4->addComponent<TextComponent>("Pause: Assigned");
		auto t5 = txt4->addComponent<TextComponent>("Click: Assigned");
		auto t6 = txt4->addComponent<TextComponent>("Back");
		auto t7 = txt4->addComponent<TextComponent>("");
		
		t1->setPosition(sf::Vector2f(520.0f, 300.0f));
		t1->setSize(40);
		t2->setPosition(sf::Vector2f(520, 400.0f));
		t2->setSize(40);
		t3->setPosition(sf::Vector2f(520, 500));
		t3->setSize(40);
		t4->setPosition(sf::Vector2f(520, 600));
		t4->setSize(40);
		t5->setPosition(sf::Vector2f(520, 700));
		t5->setSize(40);
		t6->setPosition(sf::Vector2f(1200, 700));
		t6->setSize(40);
		t7->setPosition(sf::Vector2f(900, 450));
		t7->setSize(40);
	};
	setLoaded(true);
	Scene::Load();
}

void reBind() {
	if (toBeRebind) {//If button needs to be rebound
		auto textComponents = txt4->GetCompatibleComponent<TextComponent>();
		for (int i = 0; i < textComponents.size(); i++) {	//Check every text component
			if (textComponents[i]->getText().find(actionDesc) != std::string::npos) {	//Check if a textbox has Unnasigned keyword
				//cout << "Button found" << textComponents[i]->getText() << endl;
				//cout << textComponents[i]->getText().find(actionDesc) << endl;
				if (Joystick::isConnected(0)) {
					if (inputTypeVar==0) {	//If Joystic Buttons need to be changed then

						for (int j = 0; j < sf::Joystick::getButtonCount(0); j++) { //For Joystic Buttons

							if (Joystick::isButtonPressed(0, j)) {
								Engine::setXButtonValue(actionDesc, j);
								textComponents[i]->SetText(actionDesc + ": Assigned");	//Set Keyword Back to ssigned
								txt4->GetCompatibleComponent<TextComponent>()[6]->SetText("");
								toBeRebind = false;
								break;
							}

						}

					}
					else if (inputTypeVar==1) {	//If moveemnt Axis need to be changed then
						
						//for (int j = 0; i < sf::Joystick::AxisCount; i++) {
						for (int AxisInt = sf::Joystick::Axis::X; AxisInt != sf::Joystick::Axis::PovY;AxisInt++) {	//Transform enum of Axis to numbers so I can goi through them
							sf::Joystick::Axis test = static_cast<sf::Joystick::Axis>(AxisInt);

							float xxx = sf::Joystick::getAxisPosition(0, test);

							//cout << float(test)<< xxx << "\n" << endl;

							if (xxx > 80.f || xxx < -80.0f) {
								Engine::setXAxisValue(actionDesc,test);
								textComponents[i]->SetText(actionDesc + ": Assigned");	//Set Keyword Back to ssigned
								txt4->GetCompatibleComponent<TextComponent>()[6]->SetText("");
								toBeRebind = false;
								break;
							}


						}

					}
				}


			}
		}

	}
}

void RebindScene::Update(const double& dt) {
	dtRebind += dt;

	reBind();

	// get the current mouse position in the window
	const sf::Vector2i pixelPos = sf::Mouse::getPosition(Engine::GetWindow());
	// convert it to world coordinate, because we scale in the render from 1080p to the target resolution
	const sf::Vector2f worldPos = Engine::GetWindow().mapPixelToCoords(pixelPos);

	if (worldPos.x > 552 && worldPos.x < 733 && worldPos.y > 303 && worldPos.y < 352) {
		txt4->GetCompatibleComponent<TextComponent>()[0]->setColour(Color::Red);
	}
	else if (worldPos.x > 523 && worldPos.x < 741 && worldPos.y > 406 && worldPos.y < 450) {
		txt4->GetCompatibleComponent<TextComponent>()[1]->setColour(Color::Red);
	}
	else if (worldPos.x > 525 && worldPos.x < 753 && worldPos.y > 504 && worldPos.y < 547) {
		txt4->GetCompatibleComponent<TextComponent>()[2]->setColour(Color::Red);
	}
	else if (worldPos.x > 523 && worldPos.x < 738 && worldPos.y > 600 && worldPos.y < 658) {
		txt4->GetCompatibleComponent<TextComponent>()[3]->setColour(Color::Red);
	}
	else if (worldPos.x > 523 && worldPos.x < 885 && worldPos.y > 700 && worldPos.y < 757) {	//MOUSECLICK
		txt4->GetCompatibleComponent<TextComponent>()[4]->setColour(Color::Red);
	}
	else if (worldPos.x > 1203 && worldPos.x < 1296 && worldPos.y > 706 && worldPos.y < 744) {
		txt4->GetCompatibleComponent<TextComponent>()[5]->setColour(Color::Red);
	}
	else {
		for (int i = 0; i < 6; i++) {
			txt4->GetCompatibleComponent<TextComponent>()[i]->setColour(Color::White);
		}
	}

	if (sf::Mouse::isButtonPressed(Engine::getKeysss()["Click"].myMouseButton) && dtRebind > 0.2f) {
		dtRebind = 0.0f;
		if (worldPos.x > 1203 && worldPos.x < 1296 && worldPos.y > 706 && worldPos.y < 744) {
			Engine::ChangeScene(&optionsLevel);
		}
		// TODO DIMITRIOS YOU DO YOUR THING BELOW
		else if (worldPos.x > 552 && worldPos.x < 733 && worldPos.y > 303 && worldPos.y < 352) {//Jump
			txt4->GetCompatibleComponent<TextComponent>()[6]->SetText("Press Button to\nconfirm choice");
			//txt4->GetCompatibleComponent<TextComponent>()[6]->SetText("Test");
			
			actionDesc = "Jump";
			txt4->GetCompatibleComponent<TextComponent>()[0]->SetText(actionDesc + ": Unassigned");
			toBeRebind = true;
			inputTypeVar = 0;
			
		}
		else if (worldPos.x > 523 && worldPos.x < 741 && worldPos.y > 406 && worldPos.y < 450) {
			txt4->GetCompatibleComponent<TextComponent>()[6]->SetText("Press Button to\nconfirm choice");
			cout << "Option 2" << endl;

			actionDesc = "Movement";
			txt4->GetCompatibleComponent<TextComponent>()[1]->SetText(actionDesc + ": Unassigned");
			toBeRebind = true;
			inputTypeVar = 1;

		}
		else if (worldPos.x > 525 && worldPos.x < 753 && worldPos.y > 504 && worldPos.y < 547) {
			txt4->GetCompatibleComponent<TextComponent>()[6]->SetText("Press Button to\nconfirm choice");
			cout << "Option 3" << endl;
		}
		else if (worldPos.x > 523 && worldPos.x < 738 && worldPos.y > 600 && worldPos.y < 658) {
			txt4->GetCompatibleComponent<TextComponent>()[6]->SetText("Press Button to\nconfirm choice");
			cout << "Option 4" << endl;
		}
		else if (worldPos.x > 523 && worldPos.x < 885 && worldPos.y > 700 && worldPos.y < 757) {
			txt4->GetCompatibleComponent<TextComponent>()[6]->SetText("Press Button to\nconfirm choice");
			cout << "Option 5" << endl;
		}

	}

	Scene::Update(dt);
}
