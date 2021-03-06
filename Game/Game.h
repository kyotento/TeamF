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
class BossGenerator;
class Config;
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
	/// EscMnuを生成する。
	/// </summary>
	void NewEscMenu();

	/// <summary>
	/// EscMenuを消す処理。
	/// </summary>
	void DeleteEscMenu();

	/// <summary>
	/// Configを取得。
	/// </summary>
	/// <returns>t or f</returns>
	bool GetIsEscMenu()
	{
		return m_isEscMenu;
	}
	/// <summary>
	/// Configを設定。
	/// </summary>
	/// <param name="escMenu">t or f</param>
	void SetIsEscMenu(bool escMenu)
	{
		m_isEscMenu = escMenu;
	}

	//描画距離を指定する。
	void SetChunkRange(int range)
	{
		m_chunkRange = range;
	}
	int GetChunkRange()
	{
		return m_chunkRange;
	}

	//プレイヤー取得
	Player* GetPlayer() {
		return m_world->GetPlayer();
	}

	void GameBGM();

	void StopBGM(bool isStop) {
		m_stopCntBGM += isStop ? 1 : -1;
		m_stopCntBGM = max(0, m_stopCntBGM);
	}
	bool GetIsStopBGM()const {
		return m_stopCntBGM > 0;
	}

private:

	int m_chunkRange = 1;				//描画距離。

	bool m_isEscMenu = false;			//EscMenuが出ているかどうか。
	bool m_isConfig = true;				//コンフィグを開いているか。
	std::unique_ptr	<World> m_world;					//ワールド。
	std::unique_ptr <GameCamera> m_gameCamera;			//ゲームカメラ。
	std::unique_ptr <GameMode> m_gameMode;				//ゲームモード。
	std::unique_ptr <ZombieGenerator> m_zombieGenerator;//ゾンビツクール。
	std::unique_ptr <BossGenerator> m_bossGenerator;	//ボスツクール。
	std::unique_ptr <Menu> m_menu;						//メニュー。
	std::unique_ptr <CowGenerator> m_cowGenerator;

	int m_stopCntBGM = 0;
	SuicideObj::CBGM* m_bgm = nullptr;
	Config* m_config = nullptr;

	const wchar_t* m_bgmName;
	const wchar_t* m_clickName;		//click
};

