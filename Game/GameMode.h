#pragma once

class GameMode : public IGameObject
{
public:
	GameMode();
	~GameMode();

	bool Start();
	void Update();

	/// <summary>
	/// �Q�[�����[�h�̏�ԁB
	/// </summary>
	enum enGameMode {
		enGameModeSurvival,		//�T�o�C�o���B
		enGameModeCreative,		//�N���G�C�e�B�u�B
		enGameModeNum,			//�Q�[�����[�h�̐��B
	};

	const enGameMode GetGameMode() const
	{
		return m_gameMode;
	}

private:

	enGameMode m_gameMode = enGameModeNum;			//�Q�[�����[�h�B

};

