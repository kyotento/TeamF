#include "stdafx.h"
#include "Config.h"
//#include "Title.h"
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
	//���݂̃`�����N���擾����B
	m_chunk = m_game->GetChunkRange();

	m_spriteRender[0].Init(L"Resource/spriteData/MinusButton.dds");
	m_position[0] = { 0.3f,0.4f };
	m_spriteRender[0].SetPos(m_position[0]);
	m_spriteRender[1].Init(L"Resource/spriteData/ChunkButton.dds");
	m_position[1] = { 0.5f,0.4f };
	m_spriteRender[1].SetPos(m_position[1]);
	m_spriteRender[2].Init(L"Resource/spriteData/PlusButton.dds");
	m_position[2] = { 0.7f,0.4f };
	m_spriteRender[2].SetPos(m_position[2]);
	m_spriteRender[3].Init(L"Resource/spriteData/DoneButton.dds");
	m_position[3] = { 0.5f,0.6125f };
	m_spriteRender[3].SetPos(m_position[3]);

	for (int i = 0; i < m_buttonNum; i++) {
		m_spriteRender[i].SetScale(m_scale);
	}

	MouseCursor().SetLockMouseCursor(false);		//�}�E�X�J�[�\���̔�Œ�B

	return true;
}

void Config::Update()
{
	GetScPos();
	ChangeColor();
	ClickProcess();
}


//�X�N���[����ԏ�̉摜�͈͎̔擾�B
void Config::GetScPos()
{
	CVector2 imageSize[m_buttonNum];
	imageSize[0] = { 28.f,28.f };		//�摜�̃T�C�Y�B
	imageSize[1] = { 200.f,28.f };		//�摜�̃T�C�Y�B
	imageSize[2] = { 28.f,28.f };		//�摜�̃T�C�Y�B
	imageSize[3] = { 292.f,28.f };		//�摜�̃T�C�Y�B

	//todo Pivot�Ή�������B
	for (int i = 0; i < m_buttonNum; i++) {
		m_scPos[i] = { (m_position[i].x) - (imageSize[i].x / 1280.f * 0.5f) * m_scale,			//�摜�̍���B
							(m_position[i].y) + (imageSize[i].y / 720.f * 0.5f) * m_scale };
		m_scPos2[i] = { m_position[i].x + (imageSize[i].x / 1280.f * 0.5f) * m_scale,			//�摜�̉E���B
							(m_position[i].y) - (imageSize[i].y / 720.f * 0.5f) * m_scale };
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
		if (Click() == 0) {			//-.
			m_chunk -= 1;
		}
		if (Click() == 2) {			//+.
			m_chunk += 1;
		}
		if (Click() == 3) {			//Done.
			if (m_backclass == enMenu) {
				m_game->NewEscMenu();
				SetFogDistance((m_chunk * 2 + 1) * 1000);					//�t�H�O�����S�ɂ����肫�鋗���B
				SetFogStartDistance((m_chunk * 2 + 1) - 200);		//�t�H�O���n�܂鋗���B
				m_game->SetChunkRange(m_chunk);
				DeleteGO(this);
			}
			//if (m_backclass == enTitle) {
			//	//�^�C�g���̃{�^���𐶐�����B
			//	m_title->NewTitleSelect();
			//	SetFogDistance((m_chunk * 2 + 1) * 1000);					//�t�H�O�����S�ɂ����肫�鋗���B
			//	SetFogStartDistance((m_chunk * 2 + 1) - 200);		//�t�H�O���n�܂鋗���B
			//	m_game->SetChunkRange(m_chunk);
			//	DeleteGO(this);
			//}
		}
		//�`�拗����傫�����߂���ƁA�d�߂��邽�ߏ����ݒ�B
		if (m_chunk <= 1) {
			m_chunk = 1;
		}
		if (m_chunk >= 5) {
			m_chunk = 5;
		}
	}
}

void Config::PostRender() {

	wchar_t font[256];
	swprintf_s(font, L"%d", m_chunk);
	m_font.DrawScreenPos(font, { 631.f,320.f }, CVector4::White(), { 2.f,2.f },
		CVector2::Zero(),
		0.0f,
		DirectX::SpriteEffects_None,
		0.7f
	);
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