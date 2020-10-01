#pragma once
#include "Player.h"
#include "GameCamera.h"
class Game :public IGameObject
{
public:
	Game();
	~Game();

	bool Start();
	void Update();


private:

	Player* m_player = nullptr;				//�v���C���[�B
	GameCamera* m_gameCamera = nullptr;		//�Q�[���J�����B

};

