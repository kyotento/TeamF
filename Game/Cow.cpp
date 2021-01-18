#include "stdafx.h"
#include "Cow.h"

Cow::~Cow()
{

}


bool Cow::Start()
{
	//�X�L�����f���@�̏������B
	m_skinModelRender->Init(L"Resource/modelData/Cow.tkm");
	m_skinModelRender->SetPos(m_position);
	m_skinModelRender->SetScale(m_scale);
	m_skinModelRender->SetRot(m_rot);
	//���C�g�����f��������
	m_raytraceModel.Init(*m_skinModelRender);
	m_characon.SetPosition(m_position);

	m_hp = 10;					//�̗͂̐ݒ�B

	m_damageVoice = L"Resource/soundData/enemy/cat1.wav";		//�{�C�X�B
	m_deathVoice = L"Resource/soundData/enemy/cat2.wav";		//�{�C�X�B

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
	}
	KnockBack();	//�m�b�N�o�b�N�B
	Death();		//���S����B
}