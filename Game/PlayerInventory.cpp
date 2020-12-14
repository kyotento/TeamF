#include "stdafx.h"
#include "PlayerInventory.h"
#include "Inventory.h"
#include "ItemStack.h"
#include "GUIManager.h"
#include "InventoryController.h"
#include "InventorySlot.h"

namespace GUI{

	PlayerInventory::PlayerInventory( Inventory& inventory ) :
		InventoryView(inventory, L"Resource/spriteData/KariInventory.dds" ){
	}

	PlayerInventory::~PlayerInventory(){}
}
