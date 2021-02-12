#include "stdafx.h"
#include "Bed.h"
#include "Player.h"
#include "Sun.h"

namespace {
	//日没。
	float sunsetSecond = 64800.0f;
	//日の出。
	float sunriseSecond = 21600.0f;
}

Bed::Bed(bool isHeadParts, const IntVector3& pairOnDir)
	: PairBlock(isHeadParts ? enCube_BedLeg : enCube_BedHead, isHeadParts ? pairOnDir*-1 : pairOnDir)
{}

bool Bed::OnClick(Player* player) {
	//寝る
	player->SwitchSleep();
	//リスポーン地点の変更
	player->SetRespawnPos(GetModelPos());

	//朝に
	auto sun = FindGO<Sun>();
	float second = sun->GetSecond();
	//日没より現時刻が進んでいる、あるいは日の出より時刻が前だったら。
	if (second > sunsetSecond || second < sunriseSecond) {
		sun->SetSecond(sunriseSecond);
	}

	return true;
}