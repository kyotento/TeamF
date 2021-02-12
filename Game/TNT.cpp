#include "stdafx.h"
#include "TNT.h"
#include "World.h"
#include "Player.h"
#include "Enemy.h"

namespace {
	constexpr int bombArea = 4;
	constexpr int damege = 15;
}

bool TNT::OnClick(Player* player) {
	if (!m_isStared) {
		m_timer = new TNTTimer(this, player->GetWorld());
		m_isStared = true;
	}
	return true;
}
void TNT::ChangeModelColor(bool isLight) {
	if (!isLight) {
		//���f���̐F�����ɖ߂��B
		m_model.GetInstancingModel()->GetModelRender().GetSkinModel().FindMaterialSetting([](MaterialSetting* mat) {
			mat->SetEmissive(1.0f);
		});
	}
	else {
		//���f���̐F�𔒂��B
		m_model.GetInstancingModel()->GetModelRender().GetSkinModel().FindMaterialSetting([](MaterialSetting* mat) {
			mat->SetEmissive(10.0f);
		});
	}
}

void TNTTimer::Update(){
	//�����܂ł̃J�E���g�_�E��
	if (m_bombCount == 0) {
		SuicideObj::CSE* voice;
		voice = NewGO<SuicideObj::CSE>(L"Resource/soundData/enemy/beam5.wav");
		voice->Play();
	}
	m_bombCount++;
	if (m_bombCount % 10 < 5) {
		//���f���̐F�����ɖ߂��B
		m_tnt->ChangeModelColor(false);
	}
	else {
		//���f���̐F�𔒂��B
		m_tnt->ChangeModelColor(true);
	}

	if (m_bombCount < 120) {
		return;
	}

	//����
	SuicideObj::CSE* voice;
	voice = NewGO<SuicideObj::CSE>(L"Resource/soundData/enemy/destruction1.wav");
	voice->Play();

	IntVector3 pos = m_tnt->CalcBlockUnitPos();	

	//�u���b�N�j��
	for (int x = -bombArea; x < bombArea; x++) {
		for (int y = -bombArea; y < bombArea; y++) {
			for (int z = -bombArea; z < bombArea; z++) {

				if (x == 0 && y == 0 && z == 0) {
					continue;
				}

				if ((CMath::RandomZeroToOne() + 0.5f) * (float)bombArea * (float)bombArea > CVector3((float)abs(x), (float)abs(y), (float)abs(z)).LengthSq()) {
					if (CMath::RandomZeroToOne() > 0.005f) {
						m_world->DestroyBlockNoDrop(pos + IntVector3(x, y, z));
					}
					else {
						m_world->DestroyBlock(pos + IntVector3(x, y, z));
					}
				}
			}
		}
	}

	//�_���[�W����
	SuicideObj::CCollisionObj* attackCol = NewGO<SuicideObj::CCollisionObj>();
	attackCol->CreateSphere(m_tnt->GetModelPos(), CQuaternion::Identity(), Block::WIDTH * bombArea);
	attackCol->SetTimer(0);//�����P�t���[���B
	attackCol->SetCallback([&](SuicideObj::CCollisionObj::SCallbackParam& param) {
		if (param.EqualName(L"CPlayer")) {//���O�����B
			auto Tplayer = param.GetClass<Player>();
			CVector3 direction = Tplayer->GetPos() - m_tnt->GetModelPos();
			float damegeScale = 1.0f - (direction.Length() / (Block::WIDTH * bombArea));//��������
			Tplayer->TakenDamage((int)(damege * damegeScale), direction, true);//�v���C���[�Ƀ_���[�W��^����B
		}
		if (param.EqualName(L"CEnemy")) {//���O�����B
			auto enemy = param.GetClass<Enemy>();
			CVector3 direction = enemy->GetPos() - m_tnt->GetModelPos();
			float damegeScale = 1.0f - (direction.Length() / (Block::WIDTH * bombArea));//��������
			enemy->TakenDamage((int)(damege * damegeScale));//�_���[�W��^����B
		}
	});
	
	//���Օi
	m_world->DestroyBlockNoDrop(pos);
}