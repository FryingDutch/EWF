#pragma once
#include <iostream>
#include <vector>
#include "Model/SceneInterface.h"
#include "json.hpp"
#include "Model/Player/Player.h"
#include "Model/Item/Item.h"

namespace EWF
{
	class ItemSpecScene : public SceneInterface
	{
	private:
		Item m_item;

	public:
		void render(bool responseIsString = false, std::string _message = "Make a choice: ");
	};
}
