#include "stdafx.h"
#include "Bed.h"
#include "Player.h"

Bed::Bed(bool isHeadParts, const IntVector3& pairOnDir)
	: PairBlock(isHeadParts ? enCube_BedLeg : enCube_BedHead, isHeadParts ? pairOnDir*-1 : pairOnDir)
{}

bool Bed::OnClick(Player* player) {
	//寝る
	player->SwitchSleep();
	//リスポーン地点の変更
	player->SetRespawnPos(GetModelPos());

	return true;
}