#pragma once
#include <string>
#include "Entity.h"
#include "Player.h"
#include "json.hpp"


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

	private:
		bool owned{ false };
		bool oneUse{ false };

	public:
		Item() : Entity(0) {};

		Item(int _ID, std::string _name, bool _oneUse) :
			Entity(_ID), name(_name), oneUse(_oneUse)
		{
			
		}

		std::string getName()
		{
			return this->name;
		}

		void setOwned(bool _owned)
		{
			this->owned = _owned;
		}

		bool getOwned()
		{
			return this->owned;
		}
	};
}