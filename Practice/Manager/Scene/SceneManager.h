#pragma once
#include <string>
#include "../../Model/Scene/Default/DefaultScene.h"
#include "../../Model/Player/Player.h"

namespace EWF
{
	struct SceneManager
	{
	private:
		static DefaultScene defaultScene;

	public:
		static Player m_player;

	public:
		enum : char
		{
			INTRO = 'I', DEFAULT = 'D'
		};
		
		static void applyStatsChanges(size_t _i);
		static void buildScene(char _sceneType);
		static void runGame();
	};
}