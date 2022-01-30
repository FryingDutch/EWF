#pragma once
#include <string>

namespace EWF
{
	class Entity
	{
	private:
		std::string ID = "8008";

	public:
		Entity(std::string _ID) :
			ID(_ID) {}

		std::string getID() { return ID; }
		Entity& setID(std::string _ID)
		{
			ID = _ID;
			return *this;
		}
	};
}