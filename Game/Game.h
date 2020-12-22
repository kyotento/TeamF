#pragma once
#include "World.h"
#include "GameMode.h"

class Player;
class GameCamera;
class Zombie;
class Title;
class Sun;

class ZombieGenerator;
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

	std::unique_ptr<World> m_world;						//ワールド。
	std::unique_ptr <GameCamera> m_gameCamera;			//ゲームカメラ。
	std::unique_ptr <GameMode> m_gameMode;				//ゲームモード。
	std::unique_ptr <Sun> m_sun;						//太陽
	std::unique_ptr <ZombieGenerator> m_zombieGenerator;//ゾンビツクール。
};

