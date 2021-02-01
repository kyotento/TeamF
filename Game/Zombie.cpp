#include "stdafx.h"
#include "Zombie.h"


Zombie::Zombie() : Enemy(enEntity_Zombie)
{
}

Zombie::~Zombie()
{
}

bool Zombie::Start()
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

	m_hp = 20;					//�̗͂̐ݒ�B
	m_attackPow = 3;			//�U���͂̐ݒ�B
	m_knockBack = 0.5f;			//�m�b�N�o�b�N�{���B

	m_damageVoice = L"Resource/soundData/enemy/cat1.wav";
	m_deathVoice = L"Resource/soundData/enemy/cat2.wav";

	return true;
}

void Zombie::Update()
{
	//todo ���B
	if (m_position.y <= 3.f) {
		return;
	}
	//�G�l�~�[�����݂��Ă���Ƃ��̂݁B
	if (m_enemyState != enEnemy_death) {
		Tracking();				//�v���C���[��ǐՂ��鏈���B
		StateManagement();		//��ԊǗ��B

		//�T�o�C�o���̂Ƃ��B
		if (m_gameMode->GetGameMode() == GameMode::enGameModeSurvival) {
			Attack();				//�U���B
		}
	}
	KnockBack();	//�m�b�N�o�b�N�����B
	Death();		//���S����
}

//�U�������B
void Zombie::Attack()
{
	if (m_enemyState == enEnemy_attack) {	//�U����Ԃ̂Ƃ��B
		//todo �A�j���[�V�����̃t���[�������擾������ or�@�A�j���[�V�����C�x���g�̍쐬�B
		m_damageTimer += 1;		//�_���[�W����p�^�C�}�[�����Z�B
		if (m_damageTimer == 7) {		//�_���[�W����p�̃t���[���ɓ��B�������B
			SuicideObj::CCollisionObj* attackCol = NewGO<SuicideObj::CCollisionObj>();
			attackCol->CreateBox(GetDamageColPos(), m_rot, Block::WIDTH / 4);
			attackCol->SetTimer(0);		//�����P�t���[���B
			attackCol->SetCallback([&](SuicideObj::CCollisionObj::SCallbackParam& param) {
				if (param.EqualName(L"CPlayer")) {			//���O�����B
					m_player->TakenDamage(m_attackPow);		//�v���C���[�Ƀ_���[�W��^����B
				}
			});
		}
		if (m_damageTimer == 9){
			m_damageTimer = 0;			//�^�C�}�[���Z�b�c�B
		}
	}	
	else {			//�U����ԂłȂ��Ƃ��B
		m_damageTimer = 0;
	}
}

//�]���r�̃_���[�W����̍��W���擾�B
CVector3 Zombie::GetDamageColPos()
{
	Bone* rightHandBone = m_skinModelRender->FindBone(L"Bone009");		//�E��̍��B
	Bone* leftHandBone = m_skinModelRender->FindBone(L"Bone006");		//����̍��B

	const CVector3 rightHandBonePos = rightHandBone->GetPosition();		//�E��̍��̍��W�B
	const CVector3 leftHandBonePos = leftHandBone->GetPosition();		//����̍��̍��W�B

	CVector3 DamageColPos = (rightHandBonePos + leftHandBonePos) / 2;	//�_���[�W����̍��W�B

	return DamageColPos;
}