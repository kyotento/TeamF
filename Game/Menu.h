#pragma once
class Game;
class Menu : public IGameObject
{
public:
	Menu();
	~Menu();

	bool Start();
	void Update();

	void SetGame(Game* game) {
		m_game = game;
	}

	void GetScPos();		//�X�N���[����ԏ�̉摜�͈͎̔擾�B

	void ChangeColor();		//�I�����ꂽ�摜�̐F��ύX�B

	/// <summary>
	/// �N���b�N�����Ƃ��̏����B
	/// </summary>
	void ClickProcess();

	/// <returns>�N���b�N�����{�^���ԍ�</returns>
	int Click();

private:

	enum enMenuButton
	{
		enMenu_ReturnToTitle,
		enMenu_GameEnd,
		enMenu_Config,
		enMenu_Num,
	};

	enMenuButton m_menuButtion = enMenu_Num;

	static const int m_buttonNum = 3;		//���������{�^���̐��B

	GameObj::CSpriteRender m_spriteRender[m_buttonNum];		//�摜�B

	CVector2 m_position[m_buttonNum] = { CVector2::Zero() };		//�{�^���̍��W�B
	CVector2 m_scPos[m_buttonNum] = { CVector2::Zero() };			//�X�N���[����ԍ��W����B
	CVector2 m_scPos2[m_buttonNum] = { CVector2::Zero() };			//�X�N���[����ԍ��W�E���B
	float m_scale = 2.f;			//�{�^���̃X�P�[���B		
	
	Game* m_game = nullptr;			//�Q�[���B
};

