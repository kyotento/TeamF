#pragma once
#include "World.h"
#include "GameMode.h"

class Player;
class GameCamera;
class Zombie;
class Title;
class Sun;
class Menu;
class ZombieGenerator;
class CowGenerator;
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

	/// <summary>
	/// Escapeで開くインベントリ。
	/// </summary>
	void EscMenu();

	/// <summary>
	/// Configを取得。
	/// </summary>
	/// <returns>t or f</returns>
	bool GetIsConfig()
	{
		return m_isConfig;
	}
	/// <summary>
	/// Configを設定。
	/// </summary>
	/// <param name="config">t or f</param>
	void SetIsConfig(bool config)
	{
		m_isConfig = config;
	}

private:
	bool m_isConfig = true;
	std::unique_ptr	<World> m_world;					//ワールド。
	std::unique_ptr <GameCamera> m_gameCamera;			//ゲームカメラ。
	std::unique_ptr <GameMode> m_gameMode;				//ゲームモード。
	std::unique_ptr <Sun> m_sun;						//太陽
	std::unique_ptr <ZombieGenerator> m_zombieGenerator;//ゾンビツクール。
	std::unique_ptr <Menu> m_menu;						//メニュー。
	std::unique_ptr <CowGenerator> m_cowGenerator;		//牛。
};

