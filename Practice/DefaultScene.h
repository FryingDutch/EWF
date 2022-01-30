#pragma once
#include <iostream>
#include <vector>

namespace EWF
{
	class DefaultScene
	{
	protected:
		uint16_t response{ 0 };
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
		void render(std::string _message = "Make a choice: ");

		uint16_t getResponse();
		std::string getText(uint32_t index);
	};
}
