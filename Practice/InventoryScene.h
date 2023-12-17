#pragma once
#include <iostream>
#include <vector>
#include "Model/SceneInterface.h"

namespace EWF
{
	class InventoryScene : public SceneInterface
	{
	public:
		InventoryScene() = default;
		void render(bool responseIsString = false, std::string _message = "Make a choice: ");
	};
}
