#include "stdafx.h"
#include "CraftingTable.h"
#include "Player.h"

bool CraftingTable::OnClick( Player * player ){
	player->TakenDamage( 5 );
	return true;
}
