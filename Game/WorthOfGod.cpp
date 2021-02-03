#include "stdafx.h"
#include "WorthOfGod.h"
#include "Player.h"
#include "World.h"
#include "Enemy.h"

bool WorthOfGod::OnClick(Player* player) {
	//‘Sœ‹
	auto& entitys = player->GetWorld()->GetEntities();
	for (Entity* entity : entitys) {
		Enemy* enemy = dynamic_cast<Enemy*>(entity);
		if (enemy) {
			enemy->TakenDamage(1000000000);
		}
	}

	return true;
}