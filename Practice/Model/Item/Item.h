#pragma once
#include <string>
#include "../Entity/Entity.h"
#include "../Player/Player.h"
#include "json.hpp"
#include "../../../DataObject.h"

namespace EWF
{
	class Item : public DataObject
	{
	public:
		const std::string ID = "id";
		const std::string NAME = "name";
		const std::string QTY = "qty";
		const std::string OWNED = "owned";

	public:
		Item(nlohmann::json _data) :
			DataObject(_data) {}

		Item() 
		{
			this->setData(Item::ID, NULL);
			this->setData(Item::NAME, std::string{});
			this->setData(Item::OWNED, false);
			this->setData(Item::QTY, 0);
		};

		uint32_t getId()
		{
			return this->getData(Item::ID);
		}

		void setName(std::string _name)
		{
			this->setData(Item::NAME, _name);
		}

		std::string getName()
		{
			return this->getData(Item::NAME);
		}

		void setQuantity(uint32_t _qty)
		{
			this->setData(Item::QTY, _qty);
		}

		uint32_t getQuantity()
		{
			return this->getData(Item::QTY);
		}

		void setOwned(bool _owned)
		{
			this->setData(Item::OWNED, _owned);
		}

		bool getOwned()
		{
			return this->getData(Item::OWNED);
		}
	};
}