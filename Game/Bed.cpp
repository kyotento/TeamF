#include "stdafx.h"
#include "Bed.h"
#include "Player.h"

Bed::Bed(bool isHeadParts, enMuki muki)
	: PairBlock(isHeadParts ? enCube_BedLeg : enCube_BedHead, isHeadParts ? GetMukiDir(muki) * -1 : GetMukiDir(muki))
{}

bool Bed::OnClick(Player* player) {
	//寝る
	player->SwitchSleep();
	//リスポーン地点の変更
	player->SetRespawnPos(GetModelPos());

	return true;
}