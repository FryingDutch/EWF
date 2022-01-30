#pragma once
#include <string>
#include "Entity.h"

namespace EWF
{
	class Player : public Entity
	{
	private:
		static uint32_t maxHealth;
		static uint32_t health;
		static int32_t minHealth;

		static uint32_t attack;
		static uint32_t defense;

		static std::string name;
		static uint32_t age;

	public:
		static bool isMale;

	public:
		static void setName(std::string _name) { name = _name; }
		static void setAge(uint32_t _age) { age = _age; }

		static std::string getName() { return name; }
		static uint32_t getAge() { return age; }

		static uint32_t getHealth() { return health; }
		static void setHealth(uint32_t _health) { health = _health; }

		static uint32_t getMaxHealth() { return maxHealth; }
		static void setMaxHealth(uint32_t _maxHealth) { maxHealth = _maxHealth; }

		static uint32_t getAttack() { return health; }
		static void setAttack(uint32_t _attack) { attack = _attack; }

		static uint32_t getDefense() { return defense; }
		static void setDefense(uint32_t _defense) { defense = _defense; }
	};
}