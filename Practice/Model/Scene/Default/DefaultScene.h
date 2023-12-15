#pragma once
#include <iostream>
#include <vector>
#include "../../SceneInterface.h"
#include "../Abstract/AbstractScene.h"

namespace EWF
{
	class DefaultScene : public SceneInterface, AbstractScene
	{
	protected:
		std::string response{ "" };
		std::vector<std::string> text;

	public:
		enum
		{
			STORY = 0, OPT1, OPT2, OPT3, OPT4, OPT5
		};

	public:
		DefaultScene() = default;
		DefaultScene(std::vector<std::string> _text);
		void setText(std::vector<std::string> _text);
		void render(bool responseIsString = false, std::string _message = "Make a choice: ");

		std::string getResponse();
		std::string getText(uint32_t index);
	};
}
