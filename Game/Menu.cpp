#include "stdafx.h"
#include "Menu.h"
#include "Game.h"

Menu::Menu()
{
}


Menu::~Menu()
{
	MouseCursor().SetLockMouseCursor(true);		//�}�E�X�J�[�\���̔�Œ�B
}

bool Menu::Start()
{
	m_spriteRender[0].Init(L"Resource/spriteData/ReturnToTitleButton.dds");
	m_spriteRender[1].Init(L"Resource/spriteData/GameEndButton.dds");
	m_spriteRender[2].Init(L"Resource/spriteData/ConfigurationButton.dds");

	for (int i = 0; i < m_buttonNum; i++) {
		m_position[i] = { 0.5f,0.4f };
		float posY = 0.125f * i;		//Y���W�̉��Z�l�B
		m_position[i].y += posY;
		m_spriteRender[i].SetPos(m_position[i]);
		m_spriteRender[i].SetScale(m_scale);
	}
	MouseCursor().SetLockMouseCursor(false);		//�}�E�X�J�[�\���̔�Œ�B
	return true;
}

void Menu::Update()
{
	GetScPos();				//�X�N���[����ԏ�̉摜�͈͎̔擾�B
	ChangeColor();			//�I�����ꂽ�摜�̐F��ύX�B
	ClickProcess();			// �N���b�N�����Ƃ��̏����B
}

//�X�N���[����ԏ�̉摜�͈͎̔擾�B
void Menu::GetScPos()
{
	CVector2 imageSize = { 292.f,28.f };		//�摜�̃T�C�Y�B

	//todo Pivot�Ή�������B
	for (int i = 0; i < m_buttonNum; i++) {
		m_scPos[i] = { (m_position[i].x) - (imageSize.x / 1280.f * 0.5f) * m_scale,			//�摜�̍���B
							(m_position[i].y) + (imageSize.y / 720.f * 0.5f) * m_scale };
		m_scPos2[i] = { m_position[i].x + (imageSize.x / 1280.f * 0.5f) * m_scale,			//�摜�̉E���B
							(m_position[i].y) - (imageSize.y / 720.f * 0.5f) * m_scale };
	}
}

//�I�����ꂽ�摜�̐F��ύX�B
void Menu::ChangeColor()
{
	CVector3 mousePos = MouseCursor().GetMouseCursorPos();			//�}�E�X�J�[�\���̍��W�B

	for (int i = 0; i < m_buttonNum; i++) {
		//�摜���Ƀ}�E�X�J�[�\��������Ƃ��B
		if (mousePos.x >= m_scPos[i].x && mousePos.y <= m_scPos[i].y
			&& mousePos.x <= m_scPos2[i].x && mousePos.y >= m_scPos2[i].y) {
			m_spriteRender[i].SetColor(CVector4::Green());
		}
		else {		//�Ȃ��Ƃ��B
			m_spriteRender[i].SetColor(CVector4::White());
		}
	}
}

// �N���b�N�����Ƃ��̏����B
void Menu::ClickProcess()
{
	if (GetKeyDown(VK_LBUTTON)) {		//���N���b�N�������B
		if (Click() == enMenu_ReturnToTitle) {
			m_game->TransToTitle();
		}
		else if (Click() == enMenu_GameEnd) {
			GetEngine().BreakGameLoop();
		}
		else if (Click() == enMenu_Config) {
			MessageBox(nullptr, "�t�x�ݖ����Ɏ������܂�", "�܁[�҂�", MB_ICONASTERISK);
		}
	}
}

//�}�E�X���N���b�N�����Ƃ��̔���B
int Menu::Click()
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