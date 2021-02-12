#include "stdafx.h"
#include "Bed.h"
#include "Player.h"
#include "Sun.h"

namespace {
	//���v�B
	float sunsetSecond = 64800.0f;
	//���̏o�B
	float sunriseSecond = 21600.0f;
}

Bed::Bed(bool isHeadParts, const IntVector3& pairOnDir)
	: PairBlock(isHeadParts ? enCube_BedLeg : enCube_BedHead, isHeadParts ? pairOnDir*-1 : pairOnDir)
{}

bool Bed::OnClick(Player* player) {
	//�Q��
	player->SwitchSleep();
	//���X�|�[���n�_�̕ύX
	player->SetRespawnPos(GetModelPos());

	//����
	auto sun = FindGO<Sun>();
	float second = sun->GetSecond();
	//���v��茻�������i��ł���A���邢�͓��̏o��莞�����O��������B
	if (second > sunsetSecond || second < sunriseSecond) {
		sun->SetSecond(sunriseSecond);
	}

	return true;
}