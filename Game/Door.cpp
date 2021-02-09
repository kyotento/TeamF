#include "stdafx.h"
#include "Door.h"
#include "World.h"

bool Door::OnClick(Player* player) {
	enMuki muki;
	if (GetMuki() == enMuki::enXm) {
		muki = enMuki::enZm;
	}
	if (GetMuki() == enMuki::enZm) {
		muki = enMuki::enXm;
	}
	if (GetMuki() == enMuki::enXp) {
		muki = enMuki::enZp;
	}
	if (GetMuki() == enMuki::enZp) {
		muki = enMuki::enXp;
	}
	SetMuki(muki);
	//ƒyƒA‚ÌŠJ•Â
	if (m_pairType != enCube_None) {
		SuicideObj::CSE* se;
		IntVector3 pos = CalcBlockUnitPos() + m_pairOnDir;
		auto block = m_sWorld->GetBlock(pos);
		if (block && block->GetBlockType() == m_pairType) {
			se = NewGO<SuicideObj::CSE>(L"Resource/soundData/door/open.wav");
			se->SetVolume(0.3f);
			se->Play();
			block->SetMuki(muki);
		}
	}

	return true;
}