#include "Player.h"

namespace EWF
{
	uint32_t Player::maxHealth = 3;
	uint32_t Player::health = Player::maxHealth;
	int32_t Player::minHealth = 0;

	uint32_t Player::attack = 0;
	uint32_t Player::defense;

	std::string Player::name;
	uint32_t Player::age;

	bool Player::isMale;
}