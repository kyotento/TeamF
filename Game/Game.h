#pragma once
#include "World.h"
#include "GameMode.h"

class Player;
class GameCamera;
class Zombie;
class PlayerParameter;
class Title;
class ZombieGenerator;
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
	GameMode* m_gameMode = nullptr;			//ゲームモード。
	Title* m_title = nullptr;				//タイトル。
	PlayerParameter* m_playerParameter = nullptr;		//プレイヤーのパラメーター。
	ZombieGenerator* m_zombieGenerator = nullptr;			//ゾンビツクール。
};

