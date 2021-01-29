#include "stdafx.h"
#include "ItemDisplay.h"
#include "GameCamera.h"
#include "Player.h"
#include "BlockFactory.h"
#include "ItemStack.h"

static const wchar_t* FILE_PATH_ARRAY[enCube_Num]{};

namespace {
	const int endBlockNum = 11;					//�{�b�N�X�n�̍Ō�̔ԍ��B
	const int startToolNum = 16;					//�c�[���̏��߂̔ԍ��B

	int DownPosY = 0;
	int DownPosZ = 0;

	int minDownPos = -50;
	int maxDownPos = 0;

	int Limit = 0;

	float UpPosY = 25;

	const float handMullFront = 45.0f;
	const float blockMullFront = 45.0f;
	const float toolMullFront = 30.0f;
	const float itemMullFront = 15.0f;

	const float handMullCross = 45.0f;
	const float BlockMullCross= 45.0f;
	const float toolMullCross = 30.0f;
	const float itemMullCross = 15.0f;

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

	if (!m_player->GetIsPlayerDead()) {
		CameraModeChangeToDisplay();
		//�Ǐ]�����B
		Follow();

		//��]
		Rotation();

		//�؂�ւ��̏����B
		Switching();

		//�N���b�N�����Ƃ��̃��[�V���������B
		LeftClickMouseToMoveHand();

		m_skinModelRender->SetScale(m_scale);
	}
	else if (m_player->GetIsPlayerDead())
	{
		m_skinModelRender->SetIsDraw(false);
	}
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

	SwitchItemType();		//���A�C�e���̏��̏����B

	//�؂�ւ�
	if (m_isItemChangeFlag && !swich_flag)
	{
		DownPosY = minDownPos;
		initItem_flag = true;
		swich_flag = true;
	}
	else if (m_isItemChangeFlag)
	{
		DownPosY = minDownPos;
		initItem_flag = true;
		swich_flag = true;
	}
	else if (swich_flag)
	{
		DownPosY++;
		BuildAgain();
		//���̈ʒu�ցB
		m_isItemChangeFlag = false;
		if (DownPosY >= maxDownPos)
		{
			DownPosY = maxDownPos;
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
	case ItemDisplay::enItem:
		ItemRotation();
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
	//���������ĂȂ����B
	if (initItem_flag && type == enHand)
	{
		DeleteGO(m_skinModelRender);
		m_skinModelRender = NewGO<GameObj::CSkinModelRender>();
		m_skinModelRender->Init(L"Resource/modelData/playerhand.tkm");
		initItem_flag = false;
	}
	//�u���b�N�n�̃A�C�e���B
	else if (initItem_flag && type == enBlock)
	{
		DeleteGO(m_skinModelRender);
		m_skinModelRender = NewGO<GameObj::CSkinModelRender>();
		m_skinModelRender->Init(m_modelPath.wstring().c_str());
		initItem_flag = false;
	}
	//�c�[���n�̃A�C�e���B
	else if (initItem_flag && type == enTool)
	{
		DeleteGO(m_skinModelRender);
		m_skinModelRender = NewGO<GameObj::CSkinModelRender>();
		m_skinModelRender->Init(m_modelPath.wstring().c_str());
		initItem_flag = false;
	}
	//����ȊO�̃A�C�e���B
	else if (initItem_flag && type == enItem)
	{
		DeleteGO(m_skinModelRender);
		m_skinModelRender = NewGO<GameObj::CSkinModelRender>();
		m_skinModelRender->Init(m_modelPath.wstring().c_str());
		initItem_flag = false;
	}
}
//��̉�]�B
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
//�u���b�N�n�̉�]�B
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
//�c�[���n�̉�]�B
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
//�؂̖_��C���S�b�g���̉�]�B
void ItemDisplay::ItemRotation()
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
	UpPosY = -40;
	m_scale = { 0.10f,0.10f,0.10f };
	m_mullFornt = itemMullFront;
	m_mullCrossProduct = itemMullCross;
}
//�C���x���g���ɍ��킹�Đ؂�ւ�(ry�B
void ItemDisplay::SwitchItemType()
{
	//�ȈՏ���
	//�A�C�e���̎Q�ƁB
	auto& item = m_player->GetInventory().GetItem(m_player->GetSelectItemNum()-1);
	if (item == nullptr) {
		type = enHand;
	}
	else if (item != nullptr) {
		if (item->GetID() >=endBlockNum && item->GetID() <= startToolNum) {
			type = enItem;
			m_modelPath = item->GetModelPath();
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

//�N���b�N�ɍ��킹�ē��������B
void ItemDisplay::LeftClickMouseToMoveHand()
{
	const int minDown = -75;
	const int plus = 5;
	if (m_player->GetIsBlockDestruction() && Limit >= maxDownPos)
	{
		Limit = minDown;
	}
	if (Limit <= maxDownPos)
	{
		Limit += plus;
	}
	CQuaternion m_rotationX;
	m_rotationX.SetRotationDeg(CVector3::AxisX(), Limit);
	m_rotation.Multiply(m_rotationX);
	m_skinModelRender->SetRot(m_rotation);
}