#pragma once
#include <string>
#include "Entity.h"
#include "Player.h"


namespace EWF
{
	class Item : public Entity
	{
	private:
		std::string name{ "" };
		uint32_t amount{ 0 };
		uint32_t health{ 0 };
		uint32_t maxHealth{ 0 };
		uint32_t attack{ 0 };
		uint32_t defense{ 0 };

	public:
		bool owned{ false };
		bool oneUse{ false };

	public:
		Item(std::string _ID, bool _oneUse) :
			Entity(_ID), oneUse(_oneUse){}


	};
}