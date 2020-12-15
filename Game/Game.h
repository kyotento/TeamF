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
	/// �^�C�g���ւ̑J�ځB
	/// </summary>
	void TransToTitle();


private:
	World m_world;
	Player* m_player = nullptr;				//�v���C���[�B
	GameCamera* m_gameCamera = nullptr;		//�Q�[���J�����B
	Zombie* m_zombie = nullptr;				//�]���r�B
	GameMode* m_gameMode = nullptr;			//�Q�[�����[�h�B
	Title* m_title = nullptr;				//�^�C�g���B
	Sun* m_sun = nullptr;					//���z
};

