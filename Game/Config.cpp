#include "stdafx.h"
#include "Config.h"
#include "Game.h"

Config::Config()
{
}


Config::~Config()
{
}

bool Config::Start()
{
	m_game = FindGO<Game>();

	m_spriteRender[0].Init(L"Resource/spriteData/ReturnToTitleButton.dds");
	m_spriteRender[1].Init(L"Resource/spriteData/GameEndButton.dds");
	m_spriteRender[2].Init(L"Resource/spriteData/GameEndButton.dds");

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

void Config::Update()
{
	GetScPos();
	ChangeColor();
}


//�X�N���[����ԏ�̉摜�͈͎̔擾�B
void Config::GetScPos()
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
void Config::ChangeColor()
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

//�摜���N���b�N�����Ƃ��̏����B
void Config::ClickProcess()
{
	if (GetKeyDown(VK_LBUTTON)) {		//���N���b�N�������B
		if (Click() == 0) {

		}
		if (Click() == 2) {
			if (m_backclass == enMenu) {
				m_game->NewEscMenu();
			}
			if (m_backclass == enTitle) {
				//�^�C�g���̃{�^���𐶐�����B
			}
		}

	}
}

//�}�E�X���N���b�N�����Ƃ��̔���B
int Config::Click()
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