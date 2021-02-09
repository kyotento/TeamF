#include "stdafx.h"
#include "PlayerDeath.h"


PlayerDeath::PlayerDeath()
{
}

PlayerDeath::~PlayerDeath()
{
}

bool PlayerDeath::Start()
{
	m_spriteRender[0].Init(L"Resource/spriteData/RespawnButton.dds");
	m_spriteRender[1].Init(L"Resource/spriteData/ReturnToTitleButton.dds");

	for (int i = 0; i < m_buttonNum; i++) {
		m_position[i] = { 0.5f,0.6f };
		float posY = 0.125f * i;		//Y���W�̉��Z�l�B
		m_position[i].y += posY;
		m_spriteRender[i].SetPos(m_position[i]);
		m_spriteRender[i].SetScale(m_scale);
	}
	MouseCursor().SetLockMouseCursor(false);		//�}�E�X�J�[�\���̔�Œ�B
	return true;
}

void PlayerDeath::Update()
{
	GetScPos();				//�X�N���[����ԏ�̉摜�͈͎̔擾�B
	ChangeColor();			//�I�����ꂽ�摜�̐F��ύX�B
}

//�X�N���[����ԏ�̉摜�͈͎̔擾�B
void PlayerDeath::GetScPos()
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
void PlayerDeath::ChangeColor()
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

void PlayerDeath::PostRender() {

	wchar_t font[256];
	swprintf_s(font, L"����ł��܂����I");
	m_font.DrawScreenPos(font, { 300.f,160.f }, CVector4::Red(), { 2.f,2.f },
		CVector2::Zero(),
		0.0f,
		DirectX::SpriteEffects_None,
		0.7f
	);
	wchar_t font2[256];
	swprintf_s(font2, L"�X�R�A::%8.3f", m_exp * 10.f);
	m_font.DrawScreenPos(font2, { 310.f,260.f }, CVector4::White(), { 1.5f,1.5f },
		CVector2::Zero(),
		0.0f,
		DirectX::SpriteEffects_None,
		0.7f
	);
}

//�}�E�X���N���b�N�����Ƃ��̔���B
int PlayerDeath::Click()
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