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
	/// �^�C�g���ւ̑J�ځB
	/// </summary>
	void TransToTitle();

	/// <summary>
	/// Escape�ŊJ���C���x���g���B
	/// </summary>
	void EscMenu();

	/// <summary>
	/// EscMnu�𐶐�����B
	/// </summary>
	void NewEscMenu();

	/// <summary>
	/// EscMenu�����������B
	/// </summary>
	void DeleteEscMenu();

	/// <summary>
	/// Config���擾�B
	/// </summary>
	/// <returns>t or f</returns>
	bool GetIsEscMenu()
	{
		return m_isEscMenu;
	}
	/// <summary>
	/// Config��ݒ�B
	/// </summary>
	/// <param name="escMenu">t or f</param>
	void SetIsEscMenu(bool escMenu)
	{
		m_isEscMenu = escMenu;
	}

	//�`�拗�����w�肷��B
	void SetChunkRange(int range)
	{
		m_chunkRange = range;
	}
	int GetChunkRange()
	{
		return m_chunkRange;
	}

private:

	int m_chunkRange = 1;				//�`�拗���B

	bool m_isEscMenu = false;			//EscMenu���o�Ă��邩�ǂ����B

	bool m_isConfig = true;
	std::unique_ptr	<World> m_world;					//���[���h�B
	std::unique_ptr <GameCamera> m_gameCamera;			//�Q�[���J�����B
	std::unique_ptr <GameMode> m_gameMode;				//�Q�[�����[�h�B
	std::unique_ptr <Sun> m_sun;						//���z
	std::unique_ptr <ZombieGenerator> m_zombieGenerator;//�]���r�c�N�[���B
	std::unique_ptr <Menu> m_menu;						//���j���[�B
	std::unique_ptr <CowGenerator> m_cowGenerator;		//���B
};

