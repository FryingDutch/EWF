#include "Player.h"

namespace EWF
{
	uint32_t Player::maxHealth = 10;
	int32_t Player::health = 1;
	int32_t Player::minHealth = 0;

	uint32_t Player::attack = 0;
	uint32_t Player::defense;

	std::string Player::name;
	uint32_t Player::age;

	bool Player::isMale;
}