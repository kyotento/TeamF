#include "stdafx.h"
#include "Box.h"
#include "ItemData.h"

Box::Box()
{
	for (int i = 0; i < m_inventryWidth; i++) {
		m_inventoryList.push_back(new Inventory());
		m_inventoryList[i]->s_item = GetItemData().GetItem(enCube_None);
	}
}

int Box::AddItem(EnCube enCube, int number)
{
	for (auto itr : m_inventoryList) {
		if (number == 0)
			continue;
		//何もアイテム入ってなかったら
		if (itr->s_item->GetBlockType() == enCube_None) {
			itr->s_item = GetItemData().GetItem(enCube);
			itr->s_number = number;
			number = 0;
		}
		//同じ種類のアイテムが入ってたら
		else if (itr->s_item->GetBlockType() == enCube) {
			int temNumber = itr->s_number + number;
			//アイテム溢れたら
			if (temNumber > itr->s_item->GetLimit()) {
				itr->s_number = itr->s_item->GetLimit();
				number = temNumber - itr->s_item->GetLimit();
			}
			//溢れなかったら
			else {
				itr->s_number = temNumber;
				number = 0;
			}
		}
	}
	return number;
}

void Box::PostRender()
{
	wchar_t output[256];
	swprintf_s(output, L"  %ls %d\n  %ls %d\n  %ls %d\n  %ls %d\n  %ls %d\n  %ls %d\n  %ls %d\n  %ls %d\n  %ls %d\n",m_inventoryList[0]->s_item->GetItemName(),m_inventoryList[0]->s_number, m_inventoryList[1]->s_item->GetItemName(), m_inventoryList[1]->s_number, m_inventoryList[2]->s_item->GetItemName(), m_inventoryList[2]->s_number, m_inventoryList[3]->s_item->GetItemName(), m_inventoryList[3]->s_number,
		m_inventoryList[4]->s_item->GetItemName(), m_inventoryList[4]->s_number, m_inventoryList[5]->s_item->GetItemName(), m_inventoryList[5]->s_number, m_inventoryList[6]->s_item->GetItemName(), m_inventoryList[6]->s_number, m_inventoryList[7]->s_item->GetItemName(), m_inventoryList[7]->s_number, m_inventoryList[8]->s_item->GetItemName(), m_inventoryList[8]->s_number);
	m_font.DrawScreenPos(output, { 500.0f,300.0f }, CVector4::White(), { 0.6f,0.6f },
		CVector2::Zero(),
		0.0f,
		DirectX::SpriteEffects_None,
		0.7f
	);
}