#include "stdafx.h"
#include "CraftingTable.h"
#include "CraftingInventory.h"
#include "Player.h"

bool CraftingTable::OnClick( Player * player ){
	player->OpenGUI( std::make_unique<GUI::CraftingInventory>( player->GetInventory() ) );
	return true;
}
