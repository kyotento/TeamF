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

	Player* m_player = nullptr;				//プレイヤー。
	GameCamera* m_gameCamera = nullptr;		//ゲームカメラ。

};

