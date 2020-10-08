#pragma once
#include "Player.h"
#include "GameCamera.h"
#include "Zombie.h"
class Game :public IGameObject
{
public:
	Game();
	~Game();

	bool Start();
	void Update();


private:

	Player* m_player = nullptr;				//プレイヤー。
	GameCamera* m_gameCamera = nullptr;		//ゲームカメラ。
	Zombie* m_zombie = nullptr;				//ゾンビ。

};

