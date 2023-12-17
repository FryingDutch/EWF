#pragma once
#include <string>
#include "../../Model/Scene/Default/DefaultScene.h"
#include "../../Model/Player/Player.h"
#include "../../../InventoryScene.h"

namespace EWF
{
	struct SceneManager
	{
	private:
		static DefaultScene defaultScene;
		static InventoryScene inventoryScene;

	public:
		static Player m_player;

	public:
		enum : char
		{
			INTRO = 'I', DEFAULT = 'D'
		};
		
		static void applyStatsChanges(size_t _i);
		static void renderScene();
		static void setNextFileToRead();
		static void applyNewStatChanges();
		static void runGame();
	};
}