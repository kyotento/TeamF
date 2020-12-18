#include "stdafx.h"
#include "ItemDisplay.h"
#include "GameCamera.h"
#include "Player.h"
#include "BlockFactory.h"
#include "ItemStack.h"

static const wchar_t* FILE_PATH_ARRAY[enCube_Num]{};

namespace {
	int DownPosY = 0;
	int DownPosZ = 0;

	float UpPosY = 25;

	const float handMullFront = 45.0f;
	const float blockMullFront = 45.0f;
	const float toolMullFront = 30.0f;

	const float handMullCross = 45.0f;
	const float BlockMullCross= 45.0f;
	const float toolMullCross = 30.0f;

	bool swich_flag = false;
	bool initItem_flag = false;
}

ItemDisplay::ItemDisplay()
{
}

ItemDisplay::~ItemDisplay()
{
	DeleteGO(m_skinModelRender);
}

bool ItemDisplay::Start()
{
	//���f������
	InitModel();
	return true;
}

void ItemDisplay::Update()
{
	if (m_gameCamera == nullptr) {
		m_gameCamera = FindGO<GameCamera>();
		return;
	}
	if (m_player == nullptr) {
		m_player = FindGO<Player>();
		return;
	}

	CameraModeChangeToDisplay();
	//�Ǐ]�����B
	Follow();

	//��]
	Rotation();

	//�؂�ւ��̏����B
	Switching();
	m_skinModelRender->SetScale(m_scale);
}

//���f�������B
void ItemDisplay::InitModel()
{
	m_skinModelRender = NewGO<GameObj::CSkinModelRender>();
	m_skinModelRender->Init(L"Resource/modelData/playerhand.tkm");
	type = enHand;
}
//���f���̒Ǐ]�B
void ItemDisplay::Follow()
{	
	const float blockPulsPosY = 60.0;				//Y�͕ʂő����Ă��炵�܂��B


	//���Ƀv���C���[�̐��ʎ擾�B
	m_forward = m_player->GetFront();
	m_forward *= m_mullFornt;

	//�E�����ɂ��炷�B
	CVector3 right = m_player->GetRight();
	m_forward += (right * m_mullCrossProduct);
	
	//�������ɂ��炷�B
	m_forward.y -= blockPulsPosY + UpPosY;

	//�㉺�����ɉ�]������B
	CQuaternion upDownRot;
	upDownRot.SetRotation(right, m_player->GetRadianXZ());
	upDownRot.Multiply(m_forward);

	//�J�����̍��W�擾�B
	m_position = m_gameCamera->GetPos();
	m_position += m_forward;

	m_position.y += DownPosY;
	////���W�����f���ցB
	m_skinModelRender->SetPos(m_position);
}
//�؂�ւ�
void ItemDisplay::Switching()
{
	int oldID = m_player->GetSelectItemNum();
	SwitchItemType();
	int minDownPos = -50;
	int maxDownPos = 0;

	//�؂�ւ�(���if����ς��܂��B)
	if (m_isItemChangeFlag && !swich_flag)
	{
		//���ɗ��Ƃ���B
		DownPosY--;
		DownPosZ--;
		if (DownPosY >= minDownPos)
		{
			DownPosY = minDownPos-1;
			DownPosZ = minDownPos-1;
			initItem_flag = true;
			swich_flag = true;
		}
	}
	//�؂�ւ����[�V�������ɐ؂�ւ����Ƃ��p�B
	else if (m_isItemChangeFlag) {
		//���ɗ��Ƃ���B
		DownPosY--;
		DownPosZ--;
		if (DownPosY >= minDownPos)
		{
			DownPosY = minDownPos - 1;
			DownPosZ = minDownPos - 1;
			initItem_flag = true;
			swich_flag = true;
		}
	}
	else if (swich_flag)
	{
		DownPosY++;
		DownPosZ++;
		BuildAgain();
		//���̈ʒu�ցB
		m_isItemChangeFlag = false;
		if (DownPosY >= maxDownPos)
		{
			DownPosY = maxDownPos;
			DownPosZ = maxDownPos;
			swich_flag = false;
		}
	}
}
//�r�̉�]����
void ItemDisplay::Rotation()
{
	switch (type)
	{
	case ItemDisplay::enHand:
		HandRotation();
		break;
	case ItemDisplay::enBlock:
		BlockRotation();
		break;
	case ItemDisplay::enTool:
		ToolRotation();
		break;
	}
}
//�J�����̃��[�h�ɍ��킹������
void ItemDisplay::CameraModeChangeToDisplay()
{
	if (m_gameCamera->GetCameraMode() == m_cameraDisplayMode)
	{
		m_skinModelRender->SetIsDraw(true);
	}
	else
	{
		m_skinModelRender->SetIsDraw(false);
	}
}
//�ēx�B
void ItemDisplay::BuildAgain()
{
	//��x�����������Ȃ����B
	//if���͌�ɕύX���܂��B
	if (initItem_flag && type == enHand)
	{
		DeleteGO(m_skinModelRender);
		m_skinModelRender = NewGO<GameObj::CSkinModelRender>();
		m_skinModelRender->Init(L"Resource/modelData/playerhand.tkm");
		initItem_flag = false;
	}
	else if (initItem_flag && type == enBlock)
	{
		DeleteGO(m_skinModelRender);
		m_skinModelRender = NewGO<GameObj::CSkinModelRender>();
		m_skinModelRender->Init(m_modelPath.wstring().c_str());
		initItem_flag = false;
	}
	else if (initItem_flag && type == enTool)
	{
		DeleteGO(m_skinModelRender);
		m_skinModelRender = NewGO<GameObj::CSkinModelRender>();
		m_skinModelRender->Init(m_modelPath.wstring().c_str());
		initItem_flag = false;
	}
}
//��̉�]
void ItemDisplay::HandRotation()
{
	const float m_rotX = 25.0f;						//�c�[���n���������Ƃ��ɑO�ɓ|�����߂̕ϐ��B
	//�v���C���[�̉�]�������Ă���
	m_radianY = m_player->GetRadianY();
	//�����Ŏ΂߂ɂ��炵�܂��B
	m_rotation = m_player->GetRot();
	CQuaternion m_rotationX;
	m_rotationX.SetRotationDeg(CVector3::AxisX(), -m_rotX);
	m_rotation.Multiply(m_rotationX);
	//�����Ńv���C���[�̏㉺�ɂ����Ă��炵�܂��B
	CQuaternion upDownRot;
	upDownRot.SetRotation(CVector3::AxisX(), -m_player->GetRadianXZ());
	m_rotation.Multiply(upDownRot);
	//���f���ցB
	m_skinModelRender->SetRot(m_rotation);
	//�T�C�Y�ƈʒu�����炷�B
	UpPosY = 0;
	m_scale = { 0.40f,0.40f,0.40f };
	m_mullFornt = handMullFront;
	m_mullCrossProduct = BlockMullCross;
}
//�u���b�N�n�̉�]
void ItemDisplay::BlockRotation()
{
	const float m_rotY = 0.0f;						//�c�[���n���������Ƃ��ɑO�ɓ|�����߂̕ϐ��B
	//�v���C���[�̉�]�������Ă���
	m_radianY = m_player->GetRadianY();
	//�����Ŏ΂߂ɂ��炵�܂��B
	m_rotation = m_player->GetRot();
	CQuaternion m_rotationX;
	m_rotationX.SetRotationDeg(CVector3::AxisY(), m_rotY);
	m_rotation.Multiply(m_rotationX);
	//�����Ńv���C���[�̏㉺�ɂ����Ă��炵�܂��B
	CQuaternion upDownRot;
	upDownRot.SetRotation(CVector3::AxisX(), -m_player->GetRadianXZ());
	m_rotation.Multiply(upDownRot);
	//���f���ցB
	m_skinModelRender->SetRot(m_rotation);
	//�T�C�Y�ƈʒu�����炷�B
	m_scale = { 0.25f,0.25f,0.25f };
	m_mullFornt = blockMullFront;
	m_mullCrossProduct = BlockMullCross;
	UpPosY = 0;
}
//�c�[���n�̉�]
void ItemDisplay::ToolRotation()
{
	const float m_rotZ = 10.0f;						//�c�[���n���������Ƃ��ɑO�ɓ|�����߂̕ϐ��B
	//�v���C���[�̉�]�������Ă���
	m_radianY = m_player->GetRadianY();
	//�����Ŏ΂߂ɂ��炵�܂��B
	m_rotation = m_player->GetRot();
	CQuaternion m_rotationX;
	m_rotationX.SetRotationDeg(CVector3::AxisZ(), m_rotZ);
	m_rotation.Multiply(m_rotationX);
	//�����Ńv���C���[�̏㉺�ɂ����Ă��炵�܂��B
	CQuaternion upDownRot;
	upDownRot.SetRotation(CVector3::AxisX(), -m_player->GetRadianXZ());
	m_rotation.Multiply(upDownRot);
	//���f���ցB
	m_skinModelRender->SetRot(m_rotation);
	//�T�C�Y�ƈʒu�����炷�B
	m_scale = { 0.2f,0.225f,0.2f };
	m_mullFornt = toolMullFront;
	m_mullCrossProduct = toolMullCross;
	UpPosY = -20;
}
//�C���x���g���ɍ��킹�Đ؂�ւ�(ry
void ItemDisplay::SwitchItemType()
{
	int endBlockNum = 11;					//�{�b�N�X�n�̍Ō�̔ԍ��B
	int startToolNum = 16;					//�c�[���̏��߂̔ԍ��B
	//�ȈՏ���
	//�A�C�e���̎Q�ƁB
	auto& item = m_player->GetInventory().GetItem(m_player->GetSelectItemNum()-1);
	if (item != nullptr) {
		if (!item->GetIsBlock() && item->GetID() <= startToolNum) {
			type = enHand;
		}
		else if (!item->GetIsBlock() && item->GetID() > startToolNum)
		{
			type = enTool;
			m_modelPath = item->GetModelPath();
		}
		else if (item->GetIsBlock()) {
			type = enBlock;
			m_modelPath = item->GetModelPath();
		}
	}
}