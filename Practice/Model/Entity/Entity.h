#pragma once
#include <string>

namespace EWF
{
	class Entity
	{
	private:
		int ID = 8008;

	public:
		Entity(int _ID) :
			ID(_ID) {}

		int getID() { return ID; }
		Entity& setID(int _ID)
		{
			ID = _ID;
			return *this;
		}
	};
}