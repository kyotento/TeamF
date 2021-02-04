#include "stdafx.h"
#include "Cow.h"

namespace {
	CVector3 setScale = { 0.85f,0.85f,0.85f };
	const int KomuraNum = 10;
}

Cow::Cow() : Animals(enEntity_Cow)
{
}

Cow::~Cow()
{

}


bool Cow::Start()
{
	//�X�L�����f���@�̏������B
	m_skinModelRender->Init(L"Resource/modelData/Cow.tkm");
	m_skinModelRender->SetPos(m_position);
	m_skinModelRender->SetScale(setScale);
	m_skinModelRender->SetRot(m_rot);
	//���C�g�����f��������
	m_raytraceModel.Init(*m_skinModelRender);

	m_characon.SetPosition(m_position);

	m_hp = KomuraNum;					//�̗͂̐ݒ�B

	m_damageVoice = L"Resource/soundData/enemy/cat1.wav";		//�{�C�X�B
	m_deathVoice = L"Resource/soundData/enemy/cat2.wav";		//�{�C�X�B

	SetDropItemId(enCube_GoldOre);
	SetChanceDropping(100);

	return true;
}

void Cow::Update()
{
	if (m_position.y <= 3.f) {
		return;
	}
	//���݂��Ă���Ƃ��B
	if (m_animalState != enAnimals_death) {
		//�܂��������Ȃ��B
		//���������Ȃ���B
		m_damageCollision->Update();
		CVector3 colPos = { m_position.x, m_position.y + Block::WIDTH, m_position.z };	//�����蔻��̍��W�B
		m_damageCollision->SetPosition(colPos);
		m_skinModelRender->SetPos(m_position);
		m_characon.SetPosition(m_position);
		Jump();
	}	
	m_characonMove = m_direction * m_moveSpeed;
	m_position = m_characon.Execute(m_characonMove);
	KnockBack();	//�m�b�N�o�b�N�B
	Death();		//���S����B
}