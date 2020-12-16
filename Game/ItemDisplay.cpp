#include "stdafx.h"
#include "ItemDisplay.h"
#include "GameCamera.h"
#include "Player.h"
#include "BlockFactory.h"

static const wchar_t* FILE_PATH_ARRAY[enCube_Num]{};

namespace {
	int DownPosY = 0;
	int DownPosZ = 0;
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
	const float mullFornt = 45.0f;					//�O�x�N�g���̐��l��傫������ϐ��B
	const float mullCrossProduct = 45.0f;			//�O�ς̐��l��傫������ϐ�

	//���Ƀv���C���[�̐��ʎ擾�B
	m_forward = m_player->GetFront();
	m_forward *= mullFornt;

	//�E�����ɂ��炷�B
	CVector3 right = m_player->GetRight();
	m_forward += (right * mullCrossProduct);
	
	//�������ɂ��炷�B
	m_forward.y -= blockPulsPosY ;

	//�㉺�����ɉ�]������B
	CQuaternion upDownRot;
	upDownRot.SetRotation(right, m_player->GetRadianXZ());
	upDownRot.Multiply(m_forward);

	//�J�����̍��W�擾�B
	m_position = m_gameCamera->GetPos();
	m_position += m_forward;

	m_position.y += DownPosY;
	m_position.z -= DownPosZ;
	////���W�����f���ցB
	m_skinModelRender->SetPos(m_position);
}
//�؂�ւ�
void ItemDisplay::Switching()
{
	int minDownPos = -50;
	int maxDownPos = 0;
	//�؂�ւ�(���if����ς��܂��B)
	if (GetKeyDown('G'))
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
	else if (swich_flag)
	{
		DownPosY++;
		DownPosZ++;
		BuildAgain();
		//���̈ʒu�ցB
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
		m_skinModelRender->Init(L"Resource/modelData/GrassBlock.tkm");
		initItem_flag = false;
		type = enBlock;
	}
	else if (initItem_flag && type == enBlock)
	{
		DeleteGO(m_skinModelRender);
		m_skinModelRender = NewGO<GameObj::CSkinModelRender>();
		m_skinModelRender->Init(L"Resource/modelData/playerhand.tkm");
		initItem_flag = false;
		type = enHand;
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
	//�T�C�Y�B
	m_scale = { 0.40f,0.40f,0.40f };
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
	//�T�C�Y�B
	m_scale = { 0.25f,0.25f,0.25f };
}
//�c�[���n�̉�]
void ItemDisplay::ToolRotation()
{
	const float m_rotX = 0.0f;						//�c�[���n���������Ƃ��ɑO�ɓ|�����߂̕ϐ��B
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
	//�T�C�Y�B
	m_scale = { 0.40f,0.40f,0.40f };
}