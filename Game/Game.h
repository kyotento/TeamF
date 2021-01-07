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

private:

	std::unique_ptr	<World> m_world;						//���[���h�B
	std::unique_ptr <GameCamera> m_gameCamera;			//�Q�[���J�����B
	std::unique_ptr <GameMode> m_gameMode;				//�Q�[�����[�h�B
	std::unique_ptr <Sun> m_sun;						//���z
	std::unique_ptr <ZombieGenerator> m_zombieGenerator;//�]���r�c�N�[���B
	std::unique_ptr <Menu> m_menu;						//���j���[�B
};

