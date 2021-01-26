#pragma once

class Game;
class Config : public IGameObject
{
public:
	Config();
	~Config();

	bool Start();
	void Update();

	void GetScPos();		//�X�N���[����ԏ�̉摜�͈͎̔擾�B

	void ChangeColor();		//�I�����ꂽ�摜�̐F��ύX

private:

	static const int m_buttonNum = 3;		//���������{�^���̐��B

	GameObj::CSpriteRender m_spriteRender[m_buttonNum];		//�摜�B

	CVector2 m_position[m_buttonNum] = { CVector2::Zero() };		//�{�^���̍��W�B
	CVector2 m_scPos[m_buttonNum] = { CVector2::Zero() };			//�X�N���[����ԍ��W����B
	CVector2 m_scPos2[m_buttonNum] = { CVector2::Zero() };			//�X�N���[����ԍ��W�E���B
	float m_scale = 2.f;			//�{�^���̃X�P�[���B		

	Game* m_game = nullptr;			//�Q�[���B

};

