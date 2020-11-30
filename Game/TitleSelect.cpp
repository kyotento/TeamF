#include "stdafx.h"
#include "TitleSelect.h"
#include "GUIManager.h"


TitleSelect::TitleSelect()
{
	//m_spriteRneder[0].Init(L"Resource/spriteData/GameStartButton.dds");
	//m_spriteRneder[1].Init(L"Resource/spriteData/GameEndButton.dds");
	//m_spriteRneder[2].Init(L"Resource/spriteData/ConfigurationButton.dds");

	//for (int i = 0; i < m_buttonNum; i++) {
	//	m_position[i] = { 0.5f,0.5f };
	//	float posY = 0.1f * i;
	//	m_position[i].y += posY;
	//	m_spriteRneder[i].SetPos(m_position[i]);
	//	m_spriteRneder[i].SetScale(m_scale);
	//}

//	GUI::GUIManager::Instance().AddRoot(this);		//�ŏ�ʂ�Node��o�^�B
}

TitleSelect::~TitleSelect()
{
}

bool TitleSelect::Start()
{
	m_spriteRneder[0].Init(L"Resource/spriteData/GameStartButton.dds");
	m_spriteRneder[1].Init(L"Resource/spriteData/GameEndButton.dds");
	m_spriteRneder[2].Init(L"Resource/spriteData/ConfigurationButton.dds");

	for (int i = 0; i < m_buttonNum; i++) {
		m_position[i] = { 0.5f,0.5f };
		float posY = 0.1f * i;
		m_position[i].y += posY;
		m_spriteRneder[i].SetPos(m_position[i]);
		m_spriteRneder[i].SetScale(m_scale);
	}

	return true;
}

void TitleSelect::Update()
{
	GetScPos();				//�X�N���[����ԏ�̉摜�͈͎̔擾�B
	ChangeColor();			//�I�����ꂽ�摜�̐F��ύX�B
}

//�X�N���[����ԏ�̉摜�͈͎̔擾�B
void TitleSelect::GetScPos()
{
	CVector2 imageSize = { 292.f,28.f };		//�摜�̃T�C�Y�B

	//todo Pivot�Ή�������B
	for (int i = 0; i < m_buttonNum; i++) {
		m_scPos[i] = { (m_position[i].x) - (imageSize.x / 1280.f * 0.5f) * m_scale,			//�摜�̍���B
							(m_position[i].y) + (imageSize.y / 720.f * 0.5f) * m_scale};
		m_scPos2[i] = { m_position[i].x + (imageSize.x / 1280.f * 0.5f) * m_scale,			//�摜�̉E���B
							(m_position[i].y) - (imageSize.y / 720.f * 0.5f) * m_scale};
	}
}

//�I�����ꂽ�摜�̐F��ύX�B
void TitleSelect::ChangeColor()
{
	CVector3 mousePos = MouseCursor().GetMouseCursorPos();			//�}�E�X�J�[�\���̍��W�B

	for (int i = 0; i < m_buttonNum; i++) {
		//�摜���Ƀ}�E�X�J�[�\��������Ƃ��B
		if (mousePos.x >= m_scPos[i].x && mousePos.y <= m_scPos[i].y
			&& mousePos.x <= m_scPos2[i].x && mousePos.y >= m_scPos2[i].y) {
			m_spriteRneder[i].SetColor(CVector4::Green());
		}
		else {		//�Ȃ��Ƃ��B
			m_spriteRneder[i].SetColor(CVector4::White());
		}
	}
}

//�}�E�X���N���b�N�����Ƃ��̔���B
int TitleSelect::Click()
{
	if (GetKeyDown(VK_LBUTTON)) {		//�E�N���b�N�������B

		CVector3 mousePos = MouseCursor().GetMouseCursorPos();			//�}�E�X�J�[�\���̍��W�B

		for (int i = 0; i < m_buttonNum; i++) {
			//�摜���Ƀ}�E�X�J�[�\��������Ƃ��B
			if (mousePos.x >= m_scPos[i].x && mousePos.y <= m_scPos[i].y			
				 && mousePos.x <= m_scPos2[i].x && mousePos.y >= m_scPos2[i].y) {
				return i;
			}
		}
	}

	return m_buttonNum;		//�摜�̔ԍ��O�̐��l�B
}


//CVector2 TitleSelect::GetSize() const 
//{
//	CVector2 scale = { 292.f,28.f };
//	scale *= m_scale;
//	return scale;
//}