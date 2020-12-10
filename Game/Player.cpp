#include "stdafx.h"
#include "Player.h"
#include "GameCamera.h"
#define _USE_MATH_DEFINES //M_PI �~�����Ăяo��
#include <math.h> 
#include "Item.h"
#include "GameMode.h"
#include "World.h"
#include "InventoryGUI.h"
#include "BlockFactory.h"
#include "ItemDisplay.h"

namespace {
	const float turnMult = 20.0f;			//�v���C���[�̉�]���x�B
	const float maxDegreeXZ = 70.0f;		//XZ���̉�]�̍ő�l�B
	const float minDegreeXZ = -50.0f;		//XZ���̉�]�̍ŏ��l�B
	const float moveMult = 8.0f;			//�v���C���[�̈ړ����x�B
	const float move = 1.0f;				//�ړ����x(��{�I�ɂ͐G��Ȃ�)�B
	const float gravitationalAcceleration = 0.3f;		//todo ���ꑽ�������� �d�͉����x�B
	const float doubleClickRug = 0.2f;		//�_�u���N���b�N����ɂȂ�ԍ����B

	CVector3 stickL = CVector3::Zero();		//WSAD�L�[�ɂ��ړ���
	CVector3 moveSpeed = CVector3::Zero();		//�v���C���[�̈ړ����x(��������)�B
	CVector3 itemDisplayPos = CVector3::Zero();	//�A�C�e���i�E�蕔���j�̈ʒu�B
}

Player::Player() : m_inventory(36)
{
	//�A�j���[�V�����̐ݒ�B
	m_animationClip[enAnimationClip_Idle].Load(L"Resource/animData/player_idle.tka");
	m_animationClip[enAnimationClip_Idle].SetLoopFlag(true);
	m_animationClip[enAnimationClip_move].Load(L"Resource/animData/player_move.tka");
	m_animationClip[enAnimationClip_move].SetLoopFlag(true);
	m_animationClip[enAnimationClip_excavate].Load(L"Resource/animData/player_Excavate.tka");
	m_animationClip[enAnimationClip_excavate].SetLoopFlag(true);
}

Player::~Player()
{
	DeleteGO(m_skinModelRender);
}

bool Player::Start()
{
	//�v���C���[�N���X�̏������B
	m_skinModelRender = NewGO<GameObj::CSkinModelRender>();
	m_skinModelRender->Init(L"Resource/modelData/player.tkm", m_animationClip, enAnimationClip_Num);
	m_skinModelRender->SetPos(m_position);
	m_skinModelRender->SetRot(m_rotation);

	//�L�����R���̏������B
	m_characon.Init(m_characonRadius, m_characonHeight, m_position);

	//��e����p�R���W�����B
	m_damageCollision = std::make_unique<SuicideObj::CCollisionObj>();
	CVector3 colPos = (m_position.x, m_position.y + Block::WIDTH, m_position.z);		//�R���W�������W�B
	m_damageCollision->CreateCapsule(colPos, m_rotation, m_characonRadius, m_characonHeight);
	m_damageCollision->SetTimer(enNoTimer);				//���������B
	m_damageCollision->SetName(L"CPlayer");
	m_damageCollision->SetClass(this);					//�N���X�̃|�C���^���擾�B
	m_damageCollision->SetIsHurtCollision(true);		//�������画����Ƃ�Ȃ��B

	//�C���x���g���\�̏������B
	for (int i = 0; i < inventryWidth; i++) {
		//m_inventoryList[i] = new Inventory();
		//m_inventoryList[i]->s_item = GetItemData().GetItem(enCube_None);
	}

	//�E��\����class�ɂイ���[
	m_rightHandDisplay = NewGO<ItemDisplay>();
	m_rightHandDisplay->SetName(L"ItemDisplay");
	m_rightHandDisplay->SetPos(m_position);
	return true;
}

void Player::Update()
{
	if (m_gameCamera == nullptr) {
		m_gameCamera = FindGO<GameCamera>();
		return;
	}
	if (m_gameMode == nullptr) {
		m_gameMode = FindGO<GameMode>(L"gamemode");
	}
	//todo Debug��p�B
	if( GetKeyDown( 'C' ) ){			//�}�E�X�J�[�\�����Œ�(����)�B
		static bool lock = true;
		MouseCursor().SetLockMouseCursor( lock = !lock );
	}

	//���̍����擾�B
	m_headBone = m_skinModelRender->FindBone(L"Bone002");

	//�ړ������B
	Move();
	//��]�����B
	Turn();
	//�C���x���g�����J���B
	OpenInventory();
	//�v���C���[�̏�ԊǗ��B
	StateManagement();
	//�O����Ray���΂��B
	FlyTheRay();

	Test();
	//�E��̍X�V�����B
	ItemDisplayUpdate();
}

void Player::SetWorld(World* world, bool recursive) {
	m_world = world;
	if (recursive)
		world->SetPlayer(this, false);
}

//�L�[�{�[�h�̓��͏��Ǘ��B
void Player::KeyBoardInput()
{
	Dash();		//���鏈���B

	//�e�L�[���͂ɂ��ړ��ʂ��v�Z
	//if (!m_doubleCilckFlag) {
	if (GetKeyInput('W')) {
		stickL.y = -move;
	}
	else if (GetKeyInput('S')) {
		stickL.y = move;
	}
	if (GetKeyInput('A')) {
		stickL.x = -move;
	}
	else if (GetKeyInput('D')) {
		stickL.x = move;
	}

	if (m_gameMode->GetGameMode() == GameMode::enGameModeCreative) {		//�N���G�C�e�B�u�̂Ƃ��B

		ChangeMovemontC();

		if (m_flyingMode) {			//��s���[�h�̂Ƃ��B
			if (GetKeyInput(VK_SHIFT)) {
				stickL.z = -move;
			}
			else if (GetKeyInput(VK_SPACE)) {
				stickL.z = move;
			}
		}
	}
}

//�X�y�[�X���_�u���N���b�N�������ǂ����B
bool Player::SpaceDoubleClick()
{
	bool doubleClickeFlag = false;
	if (!m_doubleClickFlagC) {
		if (GetKeyUp(VK_SPACE)) {			//�X�y�[�X�L�[�𗣂����Ƃ��B
			m_doubleClickFlagC = true;
		}
	}
	if (m_doubleClickFlagC) {
		if (m_doubleClickTimerC <= doubleClickRug) {
			m_doubleClickTimerC += GetEngine().GetRealDeltaTimeSec();
			if (GetKeyDown(VK_SPACE)) {
				doubleClickeFlag = true;
			}
		}
		else {
			m_doubleClickFlagC = false;
			m_doubleClickTimerC = 0.f;
			doubleClickeFlag = false;
		}
	}	
	return doubleClickeFlag;
}

// �ړ����@�̐؂�ւ�(�N���G�C�e�B�u)�B
void Player::ChangeMovemontC()
{
	if (SpaceDoubleClick()) {		//�_�u���N���b�N������B
		if (!m_flyingMode){
			m_flyingMode = true;	//��s���[�h�ɁB
			return;
		}
		else {						//�_�u���N���b�N���Ă��Ȃ���΁B
			m_flyingMode = false;	//���s���[�h�ɁB
		}
	}
}

//���鏈���B
void Player::Dash()
{
	//W�_�u���N���b�N�B
	if (GetKeyUp('W')) {
		m_doubleCilckFlag = true;
	}
	if (m_doubleCilckFlag) {
		if (m_doubleClickTimer <= doubleClickRug && m_playerState != enPlayerState_run) {
			m_doubleClickTimer += GetEngine().GetRealDeltaTimeSec();	//�^�C�}�[��i�߂�B
		}
		if (m_doubleClickTimer <= doubleClickRug) {
			if (GetKeyDown('W')) {		//���点��B
				m_playerState = enPlayerState_run;
				m_runFlag = true;
			}

			if (GetKeyUp('W') && m_runFlag) {
				m_playerState = enPlayerState_idle;		//����̂���߂�B
				m_doubleClickTimer = 0.f;
				m_doubleCilckFlag = false;
				m_runFlag = false;
			}
		}
		else {			//��莞�Ԍo�߂������B
			m_playerState = enPlayerState_idle;
			m_doubleClickTimer = 0.f;
			m_doubleCilckFlag = false;
		}
	}

	//Ctrl+W�B
	if (GetKeyInput('W') && GetKeyInput(VK_CONTROL)) {
		m_playerState = enPlayerState_run;
	}
	if (m_playerState == enPlayerState_run) {
		if (GetKeyUp('W') || GetKeyUp(VK_CONTROL)) {
			m_playerState = enPlayerState_move;
		}
	}

}

//�ړ������B
void Player::Move()
{
	KeyBoardInput();		//�L�[�{�[�h�̓��͏��B
	stickL.Normalize();

	//���E���͂̏���
	moveSpeed.z = sin(m_radianY) * stickL.x;
	moveSpeed.x = -cos(m_radianY) * stickL.x;
	//�㉺���͂̏���
	moveSpeed.z += cos(m_radianY) * stickL.y;
	moveSpeed.x += sin(m_radianY) * stickL.y;
	moveSpeed.y = stickL.z;

	Jump();   //�W�����v�����B

	moveSpeed.x *= moveMult * GetEngine().GetStandardFrameRate() * m_runSpeedDouble;
	moveSpeed.y *= moveMult * GetEngine().GetStandardFrameRate();
	moveSpeed.z *= moveMult * GetEngine().GetStandardFrameRate() * m_runSpeedDouble;
	if (m_flyingMode			//�N���G�C�e�B�u�̔�s���[�h�Ȃ�ړ����x���グ��B
		&& m_gameMode->GetGameMode() == GameMode::enGameModeCreative) {			
		moveSpeed *= m_creativeSpeedMag;
	}
	//�L�����R�����ړ�������B
	m_position = m_characon.Execute(moveSpeed);
	m_skinModelRender->SetPos(m_position);
	//�E����ړ�������B
	m_rightHandDisplay->SetPos(m_position);
	//�_���[�W�����蔻��ړ��B
	CVector3 colPos = { m_position.x, m_position.y + Block::WIDTH, m_position.z };	//�����蔻��̍��W�B
	m_damageCollision->SetPosition(colPos);

	//�v���C���[�̏�Ԃ̕ύX�B
	if (m_playerState != enPlayerState_run) {
		if (stickL.Length() > 0.001f) {
			m_playerState = enPlayerState_move;
		}
		else {
			m_playerState = enPlayerState_idle;
		}
	}

	Shift();		//���Ⴊ�݂̏����B

	stickL = CVector3::Zero();
	moveSpeed = CVector3::Zero();
}

//�W�����v�B
void Player::Jump()
{
	if (m_gameMode->GetGameMode() == GameMode::enGameModeSurvival		//�T�o�C�o���̂Ƃ����B
		|| m_flyingMode == false) {										//�N���G�C�e�B�u�̃t���C���[�h�łȂ��Ƃ��B
		if (GetKeyInput(VK_SPACE) && m_characon.IsOnGround()) {			//�X�y�[�X��������Ă�����&&�n�ʂɂ�����B
			m_isJump = true;			//�W�����v�t���O��Ԃ��B
		}
		//�W�����v���̏����B
		if (m_isJump) {					

			moveSpeed.y += m_jmpInitialVelocity;
			m_jmpInitialVelocity -= m_gravity * gravitationalAcceleration;

			if (m_characon.IsOnGround() && m_jmpInitialVelocity < m_gravity * gravitationalAcceleration) {
				m_isJump = false;
				m_jmpInitialVelocity = 3.f;
			}
		}
		else
		{
			//���R�����B
			if (!m_characon.IsOnGround()) {
				m_fallSpeed += 0.1f;
				moveSpeed.y -= m_gravity + m_fallSpeed;		//���R�����B
			}
			else
			{
				m_fallSpeed = 0.5f;
			}
		}
	}
}

//��]�����B
void Player::Turn()
{
	//�}�E�X�̈ړ��ʂ��擾�B
	CVector2 mouseCursorMovePow = MouseCursor().GetMouseMove() * turnMult * GetDeltaTimeSec();
	//��]����
	m_degreeY += mouseCursorMovePow.x;
	m_degreeXZ += mouseCursorMovePow.y;

	//XZ���̉�]�𐧌��B
	if (m_degreeXZ < minDegreeXZ) {
		m_degreeXZ = minDegreeXZ;
	}
	else if (m_degreeXZ > maxDegreeXZ) {
		m_degreeXZ = maxDegreeXZ;
	}

	//�}�E�X�̉�]�ʂ����W�A���ɕϊ��B
	m_radianY = M_PI / 180 * m_degreeY;
	m_radianXZ = M_PI / 180 * m_degreeXZ;

	//��]���v�Z�B
	m_rotation.SetRotationDeg(CVector3::AxisY(), m_degreeY);
	CQuaternion modelRot;
	modelRot.SetRotationDeg(CVector3::AxisY(), m_degreeY + 180.0f);
	m_skinModelRender->SetRot(modelRot);
	Headbang();

	//�E�����Ɛ��ʕ����̃x�N�g���̌v�Z�B
	m_right = { -1.0f,0.0f,0.0f };
	m_rotation.Multiply(m_right);
	m_front = { 0.0f,0.0f,-1.0f };
	m_rotation.Multiply(m_front);
}

//���Ⴊ�݂̏����B
void Player::Shift()
{
	CQuaternion bodyRot;			//�̂̍��̉�]�B
	CQuaternion rightLegRot;		//�E���̍��̉�]�B
	CQuaternion leftLegRot;			//�����̍��̉�]�B
	Bone* bodyBone = m_skinModelRender->FindBone(L"Bone010");		//���̂̍��B
	Bone* rightLegBone = m_skinModelRender->FindBone(L"Bone015");	//�E���̍��B
	Bone* leftLegBone = m_skinModelRender->FindBone(L"Bone012");	//�����̍��B
	const float shiftDir = 30.f;			//���Ⴊ�ފp�x�B

	//���Ⴊ�݂̏���(Bone�̉�]����)�B
	if (GetKeyInput(VK_SHIFT)) {
		//todo �u���b�N���痎���Ȃ�������ǉ�����B
		bodyRot.SetRotationDeg(CVector3::AxisZ(), shiftDir);
		rightLegRot.SetRotationDeg(CVector3::AxisX(), shiftDir);
		leftLegRot.SetRotationDeg(CVector3::AxisX(), -shiftDir);

		bodyBone->SetRotationOffset(bodyRot);
		rightLegBone->SetRotationOffset(rightLegRot);
		leftLegBone->SetRotationOffset(leftLegRot);
	}
	//���ɖ߂鏈���B
	if (GetKeyUp(VK_SHIFT)) {
		bodyRot.SetRotationDeg(CVector3::AxisZ(), -shiftDir* 0.5f);
		rightLegRot.SetRotationDeg(CVector3::AxisX(), -shiftDir* 0.5f);
		leftLegRot.SetRotationDeg(CVector3::AxisX(), shiftDir* 0.5f);

		bodyBone->SetRotationOffset(bodyRot);
		rightLegBone->SetRotationOffset(rightLegRot);
		leftLegBone->SetRotationOffset(leftLegRot);
	}
}

//���̉�]�����B
void Player::Headbang()
{
	m_headBoneRot.SetRotationDeg(CVector3::AxisZ(), m_degreeXZ);
	m_headBone->SetRotationOffset(m_headBoneRot);
}

//�C���x���g�����J���B
void Player::OpenInventory()
{
	if (GetKeyDown('E')){		//E�{�^�����������Ƃ��B
		//�C���x���g�����J���B
		if (!m_inventoryGUI) {
			m_inventoryGUI = std::make_unique<GUI::InventoryGUI>(m_inventory);
			MouseCursor().SetLockMouseCursor(false);		//�}�E�X�J�[�\���̌Œ���O���B
		}else{
		//�C���x���g�������B
			m_inventoryGUI.reset();
			MouseCursor().SetLockMouseCursor(true);		//�}�E�X�J�[�\�����Œ肷��B
		}
	}
}

//�v���C���[�̏�ԊǗ��B
void Player::StateManagement()
{
	switch (m_playerState)
	{
	case Player::enPlayerState_idle:	//�ҋ@��ԁB

		//�A�j���[�V�����̍Đ��B
		m_skinModelRender->GetAnimCon().Play(enAnimationClip_Idle, 0.3f);
		m_skinModelRender->GetAnimCon().SetSpeed(1.0f);

		break;
	case Player::enPlayerState_move:	//�ړ�(����)�B

		//�A�j���[�V�����̍Đ��B
		m_skinModelRender->GetAnimCon().Play(enAnimationClip_move, 0.3f);
		m_skinModelRender->GetAnimCon().SetSpeed(0.9f);
		m_runSpeedDouble = 1.f;

		break;
	case Player::enPlayerState_run:		//�����Ă���Ƃ��B

		//�A�j���[�V�����̍Đ��B
		m_skinModelRender->GetAnimCon().Play(enAnimationClip_move, 0.3f);
		m_skinModelRender->GetAnimCon().SetSpeed(1.2f);
		m_runSpeedDouble = 2.f;

		break;
	case Player::enPlayerState_excavate:	//�����@��B

		//�A�j���[�V�����̍Đ��B
		m_skinModelRender->GetAnimCon().Play(enAnimationClip_excavate, 0.3f);
		m_skinModelRender->GetAnimCon().SetSpeed(1.0f);
		break;
	default:
		break;
	}
}

//�I�u�W�F�N�g�̐ݒu�Ɣj��B
void Player::InstallAndDestruct(btCollisionWorld::ClosestRayResultCallback ray, CVector3 frontRotAdd)
{
	frontRotAdd.Normalize();

	//�ݒu�B
	if (GetKeyDown(VK_RBUTTON)) {
		CVector3 installPos;
		installPos = (ray.m_hitPointWorld - frontRotAdd) / Block::WIDTH;
		m_world->PlaceBlock(installPos, BlockFactory::CreateBlock(enCube_CraftingTable));
	}
	//�j��B
	if (GetKeyDown(VK_LBUTTON)) {
//		m_world->GetBlock((ray.m_hitPointWorld + frontRotAdd) / Block::WIDTH)->GetBlockType();		//�u���b�N�̎�ނ��擾�B
		m_world->DeleteBlock((ray.m_hitPointWorld + frontRotAdd) / Block::WIDTH) ;					//�j��B
	}
}

//���C��O���ɔ�΂��B
void Player::FlyTheRay()
{
	if (GetKeyDown(VK_RBUTTON) || GetKeyDown(VK_LBUTTON)) {
		const int installableBlockNum = 5;		//�ݒu�\����(�u���b�N����)�B
		int reyLength = installableBlockNum * Block::WIDTH;		//���C�̒����B		 
		CVector3 frontAddRot = m_front;			//�v���C���[�̌����B
		CQuaternion rot;						//�v�Z�p�g���̂āB
		rot.SetRotationDeg(m_right, m_degreeXZ);
		rot.Multiply(frontAddRot);

		btVector3 startPoint(m_gameCamera->GetPos());					//���C�̎��_�B
		btVector3 endPoint(startPoint + frontAddRot * reyLength);		//���C�̏I�_�B
		//todo Debug Ray�`��p�B
		CVector3 kariX = m_gameCamera->GetPos() + GetMainCamera()->GetFront() * 100;
		CVector3 kariY = kariX + frontAddRot * reyLength;
		DrawLine3D(kariX, kariY, CVector4::Green());

		btCollisionWorld::ClosestRayResultCallback rayRC(startPoint, endPoint);		//���C���B
		GetEngine().GetPhysicsWorld().GetDynamicWorld()->rayTest(startPoint, endPoint, rayRC);		//���C���΂��B
		if (rayRC.hasHit()) {		//�ՓˁB
			InstallAndDestruct(rayRC , frontAddRot);
		}
	}
}

//��_���|�W�B
void Player::TakenDamage(int AttackPow)
{
	if (m_hp > 0) {			//��e����B
		m_hp -= AttackPow;
	}
	if(m_hp <= 0){			//HP��0�����ɂ��Ȃ��B
		m_hp = 0;
	}
}

//todo Debug��p�B
void Player::Test()
{
	if (GetKeyUp(VK_LEFT) && m_hp > 0) {		//�̗͌����B
		m_hp -= 1;
		if (m_defensePower > 0) {
			m_defensePower -= 1;				//�h��͌����B
		}
	}
	if (GetKeyUp(VK_RIGHT) && m_hp < 20) {		//�̗͏㏸�B
		m_hp += 1;
		if (m_defensePower < 20) {
			m_defensePower += 1;				//�h��͏㏸�B
		}
	}
	if (GetKeyUp(VK_UP) && m_stamina < 20) {	//�X�^�~�i�㏸�B
		m_stamina += 1;
	}
	if (GetKeyUp(VK_DOWN) && m_stamina > 0) {	//�X�^�~�i�����B
		m_stamina -= 1;
	}
	if (GetKeyUp(VK_NUMPAD1) && m_exp > 0) {	//�o���l�����B
		m_exp -= 0.3f;			
	}
	if (GetKeyUp(VK_NUMPAD2)) {					//�o���l�����B
		m_exp += 0.3f;
	}
}

//�E��\���̍X�V�����B
void Player::ItemDisplayUpdate()
{
	//�E��Ɉʒu�Ɖ�]�𑗂��Ă܂��B
	//m_rightHandDisplay->SetPos(m_position);
	//m_rightHandDisplay->SetRot(m_rotation);
}