#pragma once
#include <iostream>
#include <vector>
#include "../../SceneInterface.h"

namespace EWF
{
	class DefaultScene : public SceneInterface
	{
	public:
		DefaultScene() = default;
		void render(bool responseIsString = false, std::string _message = "Make a choice: ");
	};
}
