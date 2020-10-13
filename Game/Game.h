#pragma once
#include "World.h"

class Player;
class GameCamera;
class Zombie;

class Game :public IGameObject
{
public:
	Game();
	~Game();

	bool Start();
	void Update();


private:
	World m_world;
	Player* m_player = nullptr;				//�v���C���[�B
	GameCamera* m_gameCamera = nullptr;		//�Q�[���J�����B
	Zombie* m_zombie = nullptr;				//�]���r�B

};

