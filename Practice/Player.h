#pragma once
#include <string>
#include <vector>
#include "Entity.h"
#include "Item.h"
#include <map>

namespace EWF
{
	class Player : public Entity
	{
	private:
		static uint32_t maxHealth;
		static int32_t health;
		static int32_t minHealth;

		static uint32_t attack;
		static uint32_t defense;

		static std::string name;
		static uint32_t age;

		static std::map<std::string, Item> items;
		static std::vector<std::string> items2;

	public:
		static bool isMale;

	public:
		static void setName(std::string _name) { name = _name; }
		static void setAge(uint32_t _age) { age = _age; }

		static std::string getName() { return name; }
		static uint32_t getAge() { return age; }

		static int32_t getHealth() { return health; }
		static void setHealth(int32_t _health) { health = _health; }

		static uint32_t getMaxHealth() { return maxHealth; }
		static void setMaxHealth(uint32_t _maxHealth) { maxHealth = _maxHealth; }

		static void addItem(Item _item) { items[_item.getName()] = _item; }
		static void setItemActive(std::string itemName) { items[itemName].setOwned(true); }

		static Item getItem(std::string _item) { return items[_item]; }
		static std::map<std::string, Item> getItems() { return items; }

		static uint32_t getAttack() { return health; }
		static void setAttack(uint32_t _attack) { attack = _attack; }

		static uint32_t getDefense() { return defense; }
		static void setDefense(uint32_t _defense) { defense = _defense; }
	};
}