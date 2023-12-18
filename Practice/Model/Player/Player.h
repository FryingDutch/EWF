#pragma once
#include <string>
#include <vector>
#include "../Entity/Entity.h"
#include "../Item/Item.h"
#include <map>
#include "../../DataObject.h"
#include "json.hpp"

namespace EWF
{
	class Player : public DataObject
	{
	public:
		const std::string NAME = "NAME";
		const std::string AGE = "AGE";

		const std::string HEALTH = "HP";
		const std::string MAX_HEALTH = "MAXHP";
		const std::string MIN_HEALTH = "MINHP";
		const std::string ATTACK = "ATK";
		const std::string DEFENSE = "DEF";

		const std::string ITEMS = "items";

	public:
		Player()
		{
			this->setData(Player::NAME, "");
			this->setData(Player::AGE, 0);
			this->setData(Player::MAX_HEALTH, 10);
			this->setData(Player::MIN_HEALTH, 1);
			this->setData(Player::HEALTH, 3);
			this->setData(Player::ATTACK, 10);
			this->setData(Player::DEFENSE, 5);
			this->setData(Player::ITEMS, nlohmann::json());
		}

	public:
		static bool isMale;

	public:
		void setName(std::string _name) 
		{
			this->setData(Player::NAME, _name);
		}

		std::string getName()
		{
			return this->getData(Player::NAME);
		}

		void setAge(uint32_t _age) 
		{ 
			this->setData(Player::AGE, _age);
		}

		uint32_t getAge()
		{
			return this->getData(Player::AGE);
		}

		void setHealth(int32_t _health)
		{
			this->setData(Player::HEALTH, _health);
		}

		int32_t getHealth() 
		{
			return this->getData(Player::HEALTH);
		}

		void setMaxHealth(uint32_t _maxHealth)
		{ 
			this->setData(Player::MAX_HEALTH, _maxHealth);
		}

		uint32_t getMaxHealth() 
		{ 
			return this->getData(Player::MAX_HEALTH);
		}

		void setAttack(uint32_t _attack)
		{
			this->setData(Player::ATTACK, _attack);
		}

		uint32_t getAttack() 
		{ 
			return this->getData(Player::ATTACK);
		}

		void setDefense(uint32_t _defense)
		{
			this->setData(Player::DEFENSE, _defense);
		}

		uint32_t getDefense() 
		{ 
			return this->getData(Player::DEFENSE);
		}

		nlohmann::json getItems()
		{
			return this->getData(Player::ITEMS);
		}


		std::vector<Item> getOwnedItems()
		{
			nlohmann::json itemsAsJson = this->getItems();
			std::vector<Item> items;

			for (const auto& itemJson : itemsAsJson) {
				if (itemJson["owned"]) {
					items.push_back(Item{ itemJson });
				}
			}

			return items;
		}

		void addItem(Item _item) 
		{ 
			nlohmann::json items = this->getItems();
			items.push_back(_item.getData());
			this->setData(Player::ITEMS, items);
		}

		Item getItemByName(std::string _name)
		{
			nlohmann::json items = this->getData(Player::ITEMS);

			for (auto& item : items) {
				if (item["name"] == _name) {
					return Item{ item };
				}
			}

			return Item{};
		}

		void updateItem(Item _item)
		{
			nlohmann::json items = this->getData(Player::ITEMS);

			for (auto& item : items) {
				if (item["name"] == _item.getName()) {
					item = _item.getData();
				}
			}

			this->setData(Player::ITEMS, items);
		}
	};
}