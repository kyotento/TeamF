#include "stdafx.h"
#include "Enemy.h"

Enemy::Enemy(World* world, EnEntity enEntity) : Entity(world, enEntity)
{
	//�X�L�����f���𐶐��B
	m_skinModelRender = NewGO<GameObj::CSkinModelRender>();
	m_characonPos = m_position;
	//�L�����R���̏������B
	m_characon.Init(m_characonRadius, m_characonHeight, m_characonPos);
	//��e����p�R���W�����B
	m_damageCollision = std::make_unique<SuicideObj::CCollisionObj>();
	CVector3 colPos = (m_position.x, m_position.y + Block::WIDTH, m_position.z);		//�R���W�������W�B
	m_damageCollision->CreateCapsule(colPos, m_rot, m_characonRadius, m_characonHeight);
	m_damageCollision->SetTimer(enNoTimer);				//���������B
	m_damageCollision->SetName(L"CEnemy");
	m_damageCollision->SetClass(this);					//�N���X�̃|�C���^���擾�B
	m_damageCollision->SetIsHurtCollision(true);		//�������画����Ƃ�Ȃ��B
	//�v���C���[�̃C���X�^���X���擾�B
	if (m_player == nullptr) {			
		m_player = FindGO<Player>(L"player");		
	}
	//�Q�[�����[�h�̃C���X�^���X���擾�B
	if (m_gameMode == nullptr) {
		m_gameMode = FindGO<GameMode>(L"gamemode");
	}
}

Enemy::~Enemy()
{
	DeleteGO(m_skinModelRender);
}

//������ǐՂ��鏈���B
void Enemy::Tracking()
{
	//�Ǐ]�����B
	m_direction = (m_player->GetPos() - m_position);
	m_oldDirection = m_direction;		//���K������O�̒l���i�[�B
	m_direction.Normalize();
	m_direction.y = 0.f;

	Jump();				//�W�����v�����B

	if (m_oldDirection.Length() >= 1.01f * Block::WIDTH && !m_isTakenDamage) {		//�v���C���[�ƈ�苗������Ă��āA�m�b�N�o�b�N���Ă��Ȃ��Ƃ��B

		//���f���A�L�����R���A�����蔻��p�̍��W���X�V�B
		m_characonMove = m_direction * m_moveSpeed;
		m_position = m_characon.Execute(m_characonMove);
		m_skinModelRender->SetPos(m_position);
		CVector3 colPos = { m_position.x, m_position.y + Block::WIDTH, m_position.z };	//�����蔻��̍��W�B
		m_damageCollision->SetPosition(colPos);

		m_enemyState = enEnemy_tracking;			//�ǐՏ�ԂɁB
	}
	else {
		m_enemyState = enEnemy_attack;				//�U����ԂɁB
	}

	//�v���C���[�̕��������������B
	m_rot.SetRotation(CVector3::AxisY(), atan2f(m_direction.x, m_direction.z));
	m_skinModelRender->SetRot(m_rot);
}

//���������B
void Enemy::Fall()    
{
	//���R�����B
	if (!m_characon.IsOnGround()) {
		m_fallSpeed += 0.5f;
	}
	else
	{
		m_fallSpeed = 1.f;
	}

	m_direction.y -= m_fallSpeed;
}

//�W�����v�B
void Enemy::Jump()
{
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
	else {
		Fall();
	}
}

//��_���[�W�����B
void Enemy::TakenDamage(int attackDamage)
{
	if (m_hp > 0 && !m_isInvincibleTime) {		//HP������Ƃ��A�����G���ԂłȂ��Ƃ��B
		m_hp -= attackDamage;
		m_isTakenDamage = true;			//�_���[�W�t���O��Ԃ��B
		m_isInvincibleTime = true;		//���G���Ԃɂ���B

		DamageVoice();

		//�̗͂�0�����ɂ��Ȃ��B
		if (m_hp <= 0) {
			m_hp = 0;
			m_enemyState = enEnemy_death;		//���S��ԂɁB
		}
	}
}

//��_�����̃_���[�W���B
void Enemy::DamageVoice()
{
	SuicideObj::CSE* voice;
	if (m_hp <= 0) {		//���S���B
		voice = NewGO<SuicideObj::CSE>(m_deathVoice);
	}
	else {			//����łȂ��Ƃ��B
		voice = NewGO<SuicideObj::CSE>(m_damageVoice);
	}
	voice->Play();
}

//�m�b�N�o�b�N�����B
void Enemy::KnockBack()
{
	float knockBackFrame = 25.f;			//�m�b�N�o�b�N����t���[����(60FPS)�B

	if (m_isTakenDamage) {
		if (m_knockBackTimer < knockBackFrame) {

			//�m�b�N�o�b�N�����B
			CVector3 direction;
			direction = (m_player->GetPos() - m_position);
			direction.Normalize();
			direction.y = 0.f;
			m_characonMove.x -= direction.x * m_knockBack * Block::WIDTH;
			m_characonMove.z -= direction.z * m_knockBack * Block::WIDTH;

			//�����̏����B
			m_knoceBackY = m_knockBack;
			m_characonMove.y += m_knoceBackY * Block::WIDTH;
			m_knoceBackY -= m_knoceBackY + 0.5 * (1 * m_knockBack) / (knockBackFrame * 2) * (knockBackFrame * 2);	//V0 + 1/2gtt;

			m_position = m_characon.Execute(m_characonMove);
			m_skinModelRender->SetPos(m_position);

			//���f���̐F��Ԃ݂��������悤�ɂ���B
			m_skinModelRender->GetSkinModel().FindMaterialSetting([](MaterialSetting* mat) {
				mat->SetAlbedoScale({ CVector4::Red() });
			});
			m_knockBackTimer += 1;		//�^�C�}�[�����Z�B
		}
		else {		//���Z�b�g�B
			m_isTakenDamage = false;
			m_knockBackTimer = 0.f;
			m_knoceBackY = 1.f;
			m_isInvincibleTime = false;
		}
		
	}
	else if (!m_isTakenDamage) {
		//���f���̐F�����ɖ߂��B
		m_skinModelRender->GetSkinModel().FindMaterialSetting([](MaterialSetting* mat) {
			mat->SetAlbedoScale({ CVector4::White() });
		});
	}
}

//���S���̏����B
void Enemy::Death()
{
	float maxRot = 90.f;							//��]�̏���l�B
	float rotEndTime = 0.5f;						//��]�I���܂łɂ����鎞�ԁB
	float oneFrameRot = maxRot / 60.f/*frameLate*// rotEndTime;			//1�t���[���̉�]�ʁB

	//���񂾂Ƃ��B
	if (m_enemyState == enEnemy_death) {
		//���S���̉�]�����B
		if (m_deathAddRot <= maxRot) {
			CQuaternion deathRot;		//���Z�����]�ʁB
			deathRot.SetRotationDeg(CVector3::AxisZ(), oneFrameRot);
			m_rot.Multiply(deathRot);
			m_skinModelRender->SetRot(m_rot);
			m_deathAddRot += oneFrameRot;
		}
		else {		//��]���I�������G�������B
			DeleteGO(this);
		}

		//���f���̐F��Ԃ݂��������悤�ɂ���B
		m_skinModelRender->GetSkinModel().FindMaterialSetting([](MaterialSetting* mat) {
			mat->SetAlbedoScale({ CVector4::Red() });
		});
	}
}

//�G�l�~�[�̏�ԊǗ��B
void Enemy::StateManagement()
{
	switch (m_enemyState)
	{
	case enEnemy_idle:

		//�A�j���[�V�����̍Đ��B
		m_skinModelRender->GetAnimCon().Play(enAnimationClip_idle, m_interpolateTimeSec);
		m_skinModelRender->GetAnimCon().SetSpeed(m_animSpeed);

		break;
	case enEnemy_move:

		//�A�j���[�V�����̍Đ��B
		m_skinModelRender->GetAnimCon().Play(enAnimationClip_move, m_interpolateTimeSec);
		m_skinModelRender->GetAnimCon().SetSpeed(m_animSpeed);

		break;
	case enEnemy_tracking:

		//�A�j���[�V�����̍Đ��B
		m_skinModelRender->GetAnimCon().Play(enAnimationClip_tracking, m_interpolateTimeSec);
		m_skinModelRender->GetAnimCon().SetSpeed(m_animSpeed);

		break;
	case enEnemy_attack:

		//�A�j���[�V�����̍Đ��B
		m_skinModelRender->GetAnimCon().Play(enAnimationClip_attack, m_interpolateTimeSec);
		m_skinModelRender->GetAnimCon().SetSpeed(m_animSpeed);

		break;
	case enEnemy_fan:

		//�A�j���[�V�����̍Đ��B
		m_skinModelRender->GetAnimCon().Play(enAniamtionClip_fan, m_interpolateTimeSec);
		m_skinModelRender->GetAnimCon().SetSpeed(m_animSpeed);

		break;
	case enEnemy_death:
		//�A�j���[�V�����̍Đ��B
		m_skinModelRender->GetAnimCon().Play(enAnimationClip_idle, m_interpolateTimeSec);
		m_skinModelRender->GetAnimCon().SetSpeed(m_animSpeed);
		break;
	default:

		break;
	}
}