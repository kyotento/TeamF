#pragma once
#include "World.h"
#include "GameMode.h"

class Player;
class GameCamera;
class Zombie;
class Title;
class Sun;

class Game :public IGameObject
{
public:
	Game();
	~Game();

	bool Start();
	void Update();

	/// <summary>
	/// タイトルへの遷移。
	/// </summary>
	void TransToTitle();


private:
	World m_world;
	Player* m_player = nullptr;				//プレイヤー。
	GameCamera* m_gameCamera = nullptr;		//ゲームカメラ。
	Zombie* m_zombie = nullptr;				//ゾンビ。
	GameMode* m_gameMode = nullptr;			//ゲームモード。
	Title* m_title = nullptr;				//タイトル。
	Sun* m_sun = nullptr;					//太陽
};

