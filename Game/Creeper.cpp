#include "stdafx.h"
#include "Creeper.h"

namespace {
	constexpr int bombArea = 10;
	constexpr float ATTACK_DISTANCE = Block::WIDTH * 2.0f;
}

bool Creeper::Start()
{
	//�A�j���[�V�����̏������B
	m_animationClips[enAnimationClip_idle].Load(L"Resource/animData/zombie_idle.tka", true);
	m_animationClips[enAnimationClip_move].Load(L"Resource/animData/zombie_move.tka", true);
	m_animationClips[enAnimationClip_tracking].Load(L"Resource/animData/zombie_tracking.tka", true);
	m_animationClips[enAnimationClip_attack].Load(L"Resource/animData/zombie_attack.tka", true);
	m_animationClips[enAniamtionClip_fan].Load(L"Resource/animData/zombie_fan.tka", true);

	//�X�L�����f���@�̏������B
	m_skinModelRender->Init(L"Resource/modelData/zombie.tkm", m_animationClips, enAnimationClip_num);
	m_skinModelRender->SetPos(m_position);
	m_skinModelRender->SetScale(m_scale);
	m_skinModelRender->SetRot(m_rot);
	//���C�g�����f��������
	m_raytraceModel.Init(*m_skinModelRender);

	m_characon.SetPosition(m_position);

	m_hp = 1;					//�̗͂̐ݒ�B
	m_attackPow = 30;			//�U���͂̐ݒ�B
	m_exp = 0.7f;				//�擾�o���l�ʁB
	m_knockBack = 0.5f;			//�m�b�N�o�b�N�{���B

	m_damageVoice = L"Resource/soundData/enemy/zombiedamage.wav";
	m_deathVoice = L"Resource/soundData/enemy/zombiedeath.wav";

	//�A�C�e���h���b�v
	SetDropItemId(enCube_CoalOre);
	SetChanceDropping(100);
	//���A�h���b�v
	SetDropItemId(enCube_WoGBlock, true);
	SetChanceDropping(5, true);

	return true;
}

void Creeper::Update()
{	
	//EscMenu���J���Ă���Ƃ��X�V���~�߂�B
	if (m_game->GetIsEscMenu()) {
		return;
	}

	//�G�l�~�[�����݂��Ă���Ƃ��̂݁B
	if (m_enemyState != enEnemy_death) {
		//����
		{
			//�T�o�C�o���̂Ƃ��B
			if (m_gameMode->GetGameMode() == GameMode::enGameModeSurvival) {
				if ((m_player->GetPos() - GetPos()).LengthSq() < ATTACK_DISTANCE * ATTACK_DISTANCE) {
					m_enemyState = enEnemy_attack;
				}
				else if (m_enemyState != enEnemy_attack) {
					m_enemyState = enEnemy_idle;
				}
			}
			//���f���A�L�����R���A�����蔻��p�̍��W���X�V�B
			m_characonMove = CVector3::Down()*10.0f* m_moveSpeed;
			m_position = m_characon.Execute(m_characonMove);
			m_skinModelRender->SetPos(m_position);
			CVector3 colPos = { m_position.x, m_position.y + Block::WIDTH, m_position.z };	//�����蔻��̍��W�B
			m_damageCollision->SetPosition(colPos);
		}
		//StateManagement();		//��ԊǗ��B

		//�T�o�C�o���̂Ƃ��B
		if (m_gameMode->GetGameMode() == GameMode::enGameModeSurvival) {
			Attack();				//�U���B
		}
	}
	KnockBack();	//�m�b�N�o�b�N�����B
	Death();		//���S����
	AttackSun();	//���z�ɂ��_���[�W�B
}

void Creeper::Attack() {
	if (m_enemyState != enEnemy_attack) {	//�U����Ԃ̂Ƃ��B
		return;
	}

	//�����܂ł̃J�E���g�_�E��
	if (m_bombCount == 0) {
		SuicideObj::CSE* voice;
		voice = NewGO<SuicideObj::CSE>(L"Resource/soundData/enemy/beam5.wav");
		voice->Play();
	}
	m_bombCount++;
	if (m_bombCount % 10 < 5) {
		//���f���̐F�����ɖ߂��B
		m_skinModelRender->GetSkinModel().FindMaterialSetting([](MaterialSetting* mat) {
			//mat->SetAlbedoScale(CVector4::White());
			mat->SetEmissive(1.0f);
		});
	}
	else {
		//���f���̐F�𔒂��B
		m_skinModelRender->GetSkinModel().FindMaterialSetting([](MaterialSetting* mat) {
			//mat->SetAlbedoScale({ 10.0f, 10.0f, 10.0f, 1.0f });
			mat->SetEmissive(10.0f);
		});
	}

	if (m_bombCount < 120) {
		return;
	}

	//����
	SuicideObj::CSE* voice;
	voice = NewGO<SuicideObj::CSE>(L"Resource/soundData/enemy/destruction1.wav");
	voice->Play();

	//�u���b�N�j��
	IntVector3 pos = { (int)(GetPos().x / Block::WIDTH),(int)(GetPos().y / Block::WIDTH),(int)(GetPos().z / Block::WIDTH) };
	for (int x = -bombArea; x < bombArea; x++) {
		for (int y = -bombArea; y < bombArea; y++) {
			for (int z = -bombArea; z < bombArea; z++) {
				if ((CMath::RandomZeroToOne()+0.5f) * (float)bombArea * (float)bombArea > CVector3((float)abs(x), (float)abs(y), (float)abs(z)).LengthSq()) {
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
	attackCol->CreateSphere(GetPos(), m_rot, Block::WIDTH * bombArea);
	attackCol->SetTimer(0);//�����P�t���[���B
	attackCol->SetCallback([&](SuicideObj::CCollisionObj::SCallbackParam& param) {
		if (param.EqualName(L"CPlayer")) {//���O�����B
			CVector3 direction = m_player->GetPos() - GetPos();
			float damegeScale = 1.0f - (direction.Length() / (Block::WIDTH * bombArea));//��������
			m_player->TakenDamage((int)(m_attackPow * damegeScale), direction, true);//�v���C���[�Ƀ_���[�W��^����B
		}
		if (param.EqualName(L"CEnemy")) {//���O�����B
			auto enemy = param.GetClass<Enemy>();
			CVector3 direction = enemy->GetPos() - GetPos();
			float damegeScale = 1.0f - (direction.Length() / (Block::WIDTH * bombArea));//��������
			enemy->TakenDamage((int)(m_attackPow * damegeScale));//�_���[�W��^����B
		}
	});

	//����
	TakenDamage(10000);
}