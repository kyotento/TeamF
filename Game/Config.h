#pragma once

class Game;
//class Title;
class Config : public IGameObject
{
public:
	Config();
	~Config();

	bool Start();
	void Update();

	void PostRender() override;

	void GetScPos();		//�X�N���[����ԏ�̉摜�͈͎̔擾�B

	void ChangeColor();		//�I�����ꂽ�摜�̐F��ύX

	//���̃N���X�������ꂽ�Ƃ��ɍĐ��������N���X�B
	enum enBackClass {
		enMenu,		//���j���[�B
		enTitle,	//�^�C�g���B
		enNum
	};

	/// <summary>
	/// ���̃N���X�������ꂽ�Ƃ��ɍĐ��������N���X�̐ݒ�B
	/// </summary>
	/// <param name="a">�Đ��������N���X�̔ԍ�</param>
	void SetBackClass(enBackClass a) {
		m_backclass = a;
	}

	/// <summary>
	/// �N���b�N�����Ƃ��̏����B
	/// </summary>
	void ClickProcess();

	//�^�C�g����ݒ肷��B
	//void SetTitle(Title* title) {
	//	m_title = title;
	//}

	int Click();		//�N���b�N�����摜�ԍ���Ԃ��B

private:

	enBackClass m_backclass = enNum;		//�Đ��������N���X�B

	static const int m_buttonNum = 4;		//���������{�^���̐��B

	int m_chunk = 1;						//Done�̂Ƃ��ɔ��f�����`�����N�B

	GameObj::CSpriteRender m_spriteRender[m_buttonNum];		//�摜�B

	CVector2 m_position[m_buttonNum] = { CVector2::Zero() };		//�{�^���̍��W�B
	CVector2 m_scPos[m_buttonNum] = { CVector2::Zero() };			//�X�N���[����ԍ��W����B
	CVector2 m_scPos2[m_buttonNum] = { CVector2::Zero() };			//�X�N���[����ԍ��W�E���B
	float m_scale = 2.f;			//�{�^���̃X�P�[���B		

	Game* m_game = nullptr;			//�Q�[���B

	const wchar_t* m_clickName;		//click
//	Title* m_title = nullptr;		//�^�C�g���B

	CFont m_font;					//�t�H���g�B
};

