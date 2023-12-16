#pragma once
#include <string>
#include "../Entity/Entity.h"
#include "../Player/Player.h"
#include "json.hpp"


namespace EWF
{
	class Item : public Entity
	{
	private:
		std::string name{ "" };

	private:
		bool owned{ false };
		bool oneUse{ false };

	public:
		Item() : Entity(0) {};

		Item(int _ID, std::string _name, bool _oneUse) :
			Entity(_ID), name(_name), oneUse(_oneUse)
		{}

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