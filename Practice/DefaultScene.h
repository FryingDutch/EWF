#pragma once
#include <iostream>
#include <vector>
#include "SceneInterface.h"

namespace EWF
{
	class DefaultScene : public SceneInterface
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

		void printStatsBanner();
		void render(bool responseIsString = false, std::string _message = "Make a choice: ");

		std::string getResponse();
		std::string getText(uint32_t index);
	};
}
