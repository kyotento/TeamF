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

	std::unique_ptr<World> m_world;			//ワールド。
	std::unique_ptr<Player> m_player;		//プレイヤー。
	std::unique_ptr <GameCamera> m_gameCamera;		//ゲームカメラ。
	Zombie* m_zombie = nullptr;				//ゾンビ。
	GameMode* m_gameMode = nullptr;			//ゲームモード。
	Title* m_title = nullptr;				//タイトル。
	Sun* m_sun = nullptr;					//太陽
};

