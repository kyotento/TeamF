#include "stdafx.h"
#include "ItemDisplay.h"
#include "GameCamera.h"
#include "Player.h"
#include "BlockFactory.h"

namespace {

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
	m_position = {0.0f,175.0f,0.0f};
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

	//MouseFollow();

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
	m_scale = { 0.35f,0.35f,0.35f };
	m_position = { 0.0f,175.0f,0.0f };
}

//���f���̒Ǐ]�B
void ItemDisplay::Follow()
{	
	const float pulsPosY = 40;					//Y�͕ʂő����Ă��炵�܂��B
	const float mullFornt = 15.0f;				//�O�x�N�g���̐��l��傫������ϐ��B
	const float mullCrossProduct = 30.0f;		//�O�ς̐��l��傫������ϐ�

	//���Ƀv���C���[�̐��ʎ擾�B
	m_forward = m_player->GetFront();
	m_forward *= mullFornt;

	//�E�����ɂ��炷�B
	CVector3 right = m_player->GetRight();
	m_forward += (right * mullCrossProduct);
	
	//�������ɂ��炷�B
	m_forward.y -= pulsPosY;

	//�㉺�����ɉ�]������B
	CQuaternion upDownRot;
	upDownRot.SetRotation(right, m_player->GetRadianXZ());
	upDownRot.Multiply(m_forward);

	//�J�����̍��W�擾�B
	m_position = m_gameCamera->GetPos();
	m_position += m_forward;

	////���W�����f���ցB
	m_skinModelRender->SetPos(m_position);
}

//�؂�ւ��i�܂����g���Ȃ��ł��B�j
void ItemDisplay::Switching()
{

}

//�r�̉�]����
void ItemDisplay::Rotation()
{
	const float m_rotX = 25.0f;					//�O�ɓ|�����߂̕ϐ��B

	//�v���C���[�̉�]�������Ă���
	m_radianY = m_player->GetRadianY();

	//�����Ŏ΂߂ɂ��炵�܂��B
	m_rotation = m_player->GetRot();
	CQuaternion m_rotationX;
	m_rotationX.SetRotationDeg(CVector3::AxisX(),-m_rotX);
	m_rotation.Multiply(m_rotationX);
	//�����Ńv���C���[�̏㉺�ɂ����Ă��炵�܂��B
	CQuaternion upDownRot;
	upDownRot.SetRotation(CVector3::AxisX(), -m_player->GetRadianXZ());
	m_rotation.Multiply(upDownRot);
	//���f���ցB
	m_skinModelRender->SetRot(m_rotation);
}