#include "stdafx.h"
#include "Boss.h"

Boss::Boss() : Enemy(enEntity_Zombie)
{
}

Boss::~Boss()
{
}

bool Boss::Start()
{
	//�A�j���[�V�����̏������B
	m_animationClips[enAnimationClip_idle].Load(L"Resource/animData/zombie_idle.tka", true);
	m_animationClips[enAnimationClip_move].Load(L"Resource/animData/zombie_move.tka", true);
	m_animationClips[enAnimationClip_tracking].Load(L"Resource/animData/zombie_tracking.tka", true);
	m_animationClips[enAnimationClip_attack].Load(L"Resource/animData/zombie_attack.tka", true);
	m_animationClips[enAniamtionClip_fan].Load(L"Resource/animData/zombie_fan.tka", true);

	m_scale *= 1.25f;

	//�X�L�����f���@�̏������B
	m_skinModelRender->Init(L"Resource/modelData/zombie.tkm", m_animationClips, enAnimationClip_num);
	m_skinModelRender->SetPos(m_position);
	m_skinModelRender->SetScale(m_scale);
	m_skinModelRender->SetRot(m_rot);
	//���C�g�����f��������
	m_raytraceModel.Init(*m_skinModelRender);

	m_characon.SetPosition(m_position);

	m_hp = 20;					//�̗͂̐ݒ�B
	m_attackPow = 10;			//�U���͂̐ݒ�B
	m_exp = 7.0f;				//�擾�o���l�ʁB
	m_knockBack = 1.7f;			//�m�b�N�o�b�N�{���B
	m_moveSpeed *= 7.0f;		//�ړ����x
	m_attackRenge *= 2.0f;

	m_damageVoice = L"Resource/soundData/enemy/zombiedamage.wav";
	m_deathVoice = L"Resource/soundData/enemy/zombiedeath.wav";

	//�A�C�e���h���b�v
	SetDropItemId(enCube_WoGBlock);
	SetChanceDropping(100);
	//���A�h���b�v
	SetDropItemId(enCube_WoGBlock, true);
	SetChanceDropping(5, true);

	return true;
}

void Boss::Update()
{
	//EscMenu���J���Ă���Ƃ��X�V���~�߂�B
	if (m_game->GetIsEscMenu()) {
		return;
	}

	//�G�l�~�[�����݂��Ă���Ƃ��̂݁B
	if (m_enemyState != enEnemy_death) {

		if (m_stateTimer > 0.0f) {
			m_stateTimer -= GetDeltaTimeSec();
		}

		if (m_stateTimer < FLT_EPSILON) {
			if (m_enemyState == enEnemy_fan) {
				m_enemyState = enEnemy_attack;
				m_stateTimer = 0.10f;//�U������
			}
		}

		Move();

		//�T�o�C�o���̂Ƃ��B
		if (m_gameMode->GetGameMode() == GameMode::enGameModeSurvival) {
			Attack();				//�U���B
		}

		StateManagement();		//��ԊǗ��B
	}
	KnockBack();	//�m�b�N�o�b�N�����B
	Death();		//���S����
	//AttackSun();	//���z�ɂ��_���[�W�B
}

//�U�������B
void Boss::Attack()
{
	if (m_enemyState == enEnemy_attack) {	//�U����Ԃ̂Ƃ��B
		m_damageTimer += 1;		//�_���[�W����p�^�C�}�[�����Z�B
		if (m_damageTimer == 7) {		//�_���[�W����p�̃t���[���ɓ��B�������B
			SuicideObj::CCollisionObj* attackCol = NewGO<SuicideObj::CCollisionObj>();
			attackCol->CreateBox(GetDamageColPos(), m_rot, Block::WIDTH / 4);
			attackCol->SetTimer(0);		//�����P�t���[���B
			attackCol->SetCallback([&](SuicideObj::CCollisionObj::SCallbackParam& param) {
				if (param.EqualName(L"CPlayer")) {			//���O�����B
					CVector3 direction = GetDamageColPos() - GetPos();
					m_player->TakenDamage(m_attackPow, direction, true);		//�v���C���[�Ƀ_���[�W��^����B
				}
			});
		}
		if (m_damageTimer == 9) {
			m_damageTimer = 0;			//�^�C�}�[���Z�b�c�B
		}
	}
	else {			//�U����ԂłȂ��Ƃ��B
		m_damageTimer = 0;
	}
}

//�]���r�̃_���[�W����̍��W���擾�B
CVector3 Boss::GetDamageColPos()
{
	Bone* rightHandBone = m_skinModelRender->FindBone(L"Bone009");		//�E��̍��B
	Bone* leftHandBone = m_skinModelRender->FindBone(L"Bone006");		//����̍��B

	const CVector3 rightHandBonePos = rightHandBone->GetPosition();		//�E��̍��̍��W�B
	const CVector3 leftHandBonePos = leftHandBone->GetPosition();		//����̍��̍��W�B

	CVector3 DamageColPos = (rightHandBonePos + leftHandBonePos) / 2;	//�_���[�W����̍��W�B

	return DamageColPos;
}

void Boss::TakenDamage(int attackDamage)
{
	//if (m_enemyState == enEnemy_attack) {
	//	Enemy::TakenDamage(attackDamage);
	//}
	//else {
		m_enemyState = enEnemy_idle;
		m_stateTimer = 0.5f;

		m_isTakenDamage = true;			//�_���[�W�t���O��Ԃ��B
	//}
}

void Boss::Move()
{
	//�Ǐ]�����B
	m_direction = (m_player->GetPos() - m_position);
	m_oldDirection = m_direction;		//���K������O�̒l���i�[�B

	if (m_stateTimer < FLT_EPSILON) {
		m_direction.Normalize();
		m_direction.y = 0.f;

		//�W�����v�����B
		if (m_characon.IsContactWall() && m_characon.IsOnGround())
		{
			m_jumpFlag = true;
		}
		if (m_jumpFlag) {
			m_direction.y += m_jmpInitialVelocity;
			m_jmpInitialVelocity -= m_gravity;

			if (m_characon.IsOnGround() && m_jmpInitialVelocity < m_gravity) {
				m_jumpFlag = false;
				m_jmpInitialVelocity = 13.f;
			}
		}
	}
	else {
		m_direction = CVector3::Zero();
	}

	//����
	Fall();

	//�v���C���[�ƈ�苗������Ă��āA�m�b�N�o�b�N���Ă��Ȃ��Ƃ��B
	if (!m_isTakenDamage) {
		if (m_oldDirection.Length() >= m_attackRenge) {
			m_enemyState = enEnemy_tracking;			//�ǐՏ�ԂɁB
		}
		else {
			m_direction.x = m_direction.z = 0.0f;
			//m_enemyState = enEnemy_fan;				//�U����ԂɁB
			//m_stateTimer = 0.8f;
		}
	}

	//���f���A�L�����R���A�����蔻��p�̍��W���X�V�B
	m_characonMove = m_direction * m_moveSpeed;
	m_position = m_characon.Execute(m_characonMove);
	m_skinModelRender->SetPos(m_position);
	CVector3 colPos = { m_position.x, m_position.y + Block::WIDTH, m_position.z };	//�����蔻��̍��W�B
	m_damageCollision->SetPosition(colPos);

	//�v���C���[�̕��������������B
	m_rot.SetRotation(CVector3::AxisY(), atan2f(m_direction.x, m_direction.z));
	m_skinModelRender->SetRot(m_rot);
}