#include "stdafx.h"
#include "Bed.h"
#include "Player.h"

Bed::Bed(bool isHeadParts, const IntVector3& pairOnDir)
	: PairBlock(isHeadParts ? enCube_BedLeg : enCube_BedHead, isHeadParts ? pairOnDir*-1 : pairOnDir)
{}

bool Bed::OnClick(Player* player) {
	//�Q��
	player->SwitchSleep();
	//���X�|�[���n�_�̕ύX
	player->SetRespawnPos(GetModelPos());

	return true;
}