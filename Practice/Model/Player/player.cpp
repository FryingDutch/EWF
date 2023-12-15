#include "Player.h"
#include <map>

namespace EWF
{
	uint32_t Player::maxHealth = 10;
	int32_t Player::health = 1;
	int32_t Player::minHealth = 0;

	uint32_t Player::attack = 0;
	uint32_t Player::defense;

	std::string Player::name;
	uint32_t Player::age = 19;

	bool Player::isMale;

	std::map<std::string, Item> Player::items;
	std::vector<std::string> Player::items2 = {};
}