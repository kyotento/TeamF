#include "stdafx.h"
#include "Config.h"
#include "Game.h"
#include "Player.h"

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
	//���݂̃}�E�X���x���擾
	m_sensi = m_game->GetPlayer()->GetTurnMult();
	//���݂̃}�E�X���]�ݒ���擾
	m_mouseReverse = m_game->GetPlayer()->GetReverseTurnXZ();

	m_spriteRender[0].Init(L"Resource/spriteData/MinusButton.dds");
	m_position[0] = { 0.3f,0.2f };
	m_spriteRender[0].SetPos(m_position[0]);
	m_spriteRender[1].Init(L"Resource/spriteData/ChunkButton.dds");
	m_position[1] = { 0.5f,0.2f };
	m_spriteRender[1].SetPos(m_position[1]);
	m_spriteRender[2].Init(L"Resource/spriteData/PlusButton.dds");
	m_position[2] = { 0.7f,0.2f };
	m_spriteRender[2].SetPos(m_position[2]);

	m_spriteRender[3].Init(L"Resource/spriteData/DoneButton.dds");
	m_position[3] = { 0.5f,0.6125f };
	m_spriteRender[3].SetPos(m_position[3]);

	m_spriteRender[4].Init(L"Resource/spriteData/MinusButton.dds");
	m_position[4] = { 0.3f,0.3f };
	m_spriteRender[4].SetPos(m_position[4]);
	m_spriteRender[5].Init(L"Resource/spriteData/Button.dds");
	m_position[5] = { 0.5f,0.3f };
	m_spriteRender[5].SetPos(m_position[5]);
	m_spriteRender[6].Init(L"Resource/spriteData/PlusButton.dds");
	m_position[6] = { 0.7f,0.3f };
	m_spriteRender[6].SetPos(m_position[6]);
	
	m_spriteRender[7].Init(L"Resource/spriteData/Button.dds");
	m_position[7] = { 0.5f,0.4f };
	m_spriteRender[7].SetPos(m_position[7]);

	//�摜�̃T�C�Y�B
	m_imageSize[0] = { 28.f,28.f };		
	m_imageSize[1] = { 200.f,28.f };	
	m_imageSize[2] = { 28.f,28.f };		
	m_imageSize[3] = { 292.f,28.f };	
	m_imageSize[4] = { 28.f,28.f };		
	m_imageSize[5] = { 200.f,28.f };	
	m_imageSize[6] = { 28.f,28.f };		
	m_imageSize[7] = { 200.f,28.f };	

	m_clickName = L"Resource/soundData/game/click.wav";

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
	//todo Pivot�Ή�������B
	for (int i = 0; i < m_buttonNum; i++) {
		m_scPos[i] = { (m_position[i].x) - (m_imageSize[i].x / 1280.f * 0.5f) * m_scale,			//�摜�̍���B
							(m_position[i].y) + (m_imageSize[i].y / 720.f * 0.5f) * m_scale };
		m_scPos2[i] = { m_position[i].x + (m_imageSize[i].x / 1280.f * 0.5f) * m_scale,			//�摜�̉E���B
							(m_position[i].y) - (m_imageSize[i].y / 720.f * 0.5f) * m_scale };
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
		SuicideObj::CSE* se = NewGO<SuicideObj::CSE>(m_clickName);
		se->SetVolume(0.1f);
		se->Play();

		int click = Click();
		if (click == 0) {			//-.
			m_chunk -= 1;
		}
		if (click == 2) {			//+.
			m_chunk += 1;
		}
		if (click == 4) {			//-.
			m_sensi -= 5.0f;
		}
		if (click == 6) {			//+.
			m_sensi += 5.0f;
		}
		if (click == 7) {
			m_mouseReverse = !m_mouseReverse;
		}
		if (click == 3) {			//Done.
			if (m_backclass == enMenu) {
				m_game->NewEscMenu();

				SetFogDistance((m_chunk * 2 + 1) * 1000);					//�t�H�O�����S�ɂ����肫�鋗���B
				SetFogStartDistance(((m_chunk * 2 + 1) * 1000) - 200);		//�t�H�O���n�܂鋗���B
				m_game->SetChunkRange(m_chunk);

				m_game->GetPlayer()->SetTurnMult(m_sensi);
				m_game->GetPlayer()->SetReverseTurnXZ(m_mouseReverse);

				DeleteGO(this);
			}
		}

		//�`�拗����傫�����߂���ƁA�d�߂��邽�ߏ����ݒ�B
		if (m_chunk <= 1) {
			m_chunk = 1;
		}
		if (m_chunk >= 5) {
			m_chunk = 5;
		}
		m_sensi = CMath::Clamp(m_sensi, 5.0f, 40.0f);
	}
}

void Config::PostRender() {

	wchar_t font[256];
	swprintf_s(font, L"%d", m_chunk);
	m_font.Draw(font, m_position[1] + CVector2((m_imageSize[1].x / 1280.f)*0.4f, 0.0f) * 0.5f * m_scale,
		CVector4::White(),
		{ 1.2f,1.2f },
		CVector2::One()*0.5f,
		0.0f,
		DirectX::SpriteEffects_None,
		0.4f
	);

	swprintf_s(font, L"Mouse Sensivity'%.0f%%'", m_sensi / 20.0f * 100.0f);
	m_font.Draw(font, m_position[5],
		CVector4::White(),
		CVector2::One() * 0.8f,
		CVector2::One() * 0.5f,
		0.0f,
		DirectX::SpriteEffects_None,
		0.4f
	);

	if (m_mouseReverse) {
		swprintf_s(font, L"Mouse Reverse'On'");
	}
	else {
		swprintf_s(font, L"Mouse Reverse'Off'");
	}
	m_font.Draw(font, m_position[7],
		CVector4::White(),
		CVector2::One() * 0.8f,
		CVector2::One() * 0.5f,
		0.0f,
		DirectX::SpriteEffects_None,
		0.4f
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