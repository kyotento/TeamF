#include "stdafx.h"
#include "Bed.h"

Bed::Bed(bool isHeadParts, enMuki muki)
	: PairBlock(isHeadParts ? enCube_BedHead : enCube_BedLeg, isHeadParts ? GetMukiDir(muki) * -1 : GetMukiDir(muki))
{}

bool Bed::OnClick(Player* player) {
	//�Q��
	//player->SwitchSleep();
	//���X�|�[���n�_�̕ύX
	//player->SetRespawnPos(GetModelPos());

	return true;
}