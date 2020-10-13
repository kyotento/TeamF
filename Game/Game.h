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
	Player* m_player = nullptr;				//プレイヤー。
	GameCamera* m_gameCamera = nullptr;		//ゲームカメラ。
	Zombie* m_zombie = nullptr;				//ゾンビ。

};

