#pragma once

class Player;
class PlayerDeath : public IGameObject
{
public:
	PlayerDeath();
	~PlayerDeath();

	bool Start()override;
	void Update()override;

	void GetScPos();		//�X�N���[����ԏ�̉摜�͈͎̔擾�B

	void ChangeColor();		//�I�����ꂽ�摜�̐F��ύX�B

	/// <summary>
	/// �}�E�X���N���b�N�����Ƃ��̔���B
	/// </summary>
	int Click();

	enum enButtonType {
		enButtonResupawn,			//���X�|�[���{�^���B
		enButtonRerturnToTitle,		//�^�C�g���{�^���B
		enButtonNum					//�{�^���̐��B
	};

	enButtonType m_buttonType = enButtonNum;		//�{�^���ԍ��B

private:

	static const int m_buttonNum = 2;		//���������{�^���̐��B

	GameObj::CSpriteRender m_spriteRender[m_buttonNum];		//�摜�B

	CVector2 m_position[m_buttonNum] = { CVector2::Zero() };		//�{�^���̍��W�B
	CVector2 m_scPos[m_buttonNum] = { CVector2::Zero() };			//�X�N���[����ԍ��W����B
	CVector2 m_scPos2[m_buttonNum] = { CVector2::Zero() };			//�X�N���[����ԍ��W�E���B
	float m_scale = 2.f;											//�{�^���̃X�P�[���B

};

