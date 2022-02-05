#pragma once
#include "DefaultScene.h"
#include "System.h"
#include "Player.h"

namespace EWF
{
	class IntroScene : public DefaultScene
	{
	public:
		IntroScene() = default;
		IntroScene(std::vector<std::string> _text) :
			DefaultScene(_text) {};

		void render(std::string _message = "Tell your name: ") {
			while (Player::getName().empty() || Player::getName().size() > 15)
			{
				system("CLS");
				Player::setName("");
				printStatsBanner();
				std::cout << text[STORY] << "\n\n";
				Player::setName(System::getInput(_message));
			}
		}
	};
}