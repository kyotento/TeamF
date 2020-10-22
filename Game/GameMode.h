#pragma once

class GameMode : public IGameObject
{
public:
	GameMode();
	~GameMode();

	bool Start();
	void Update();

	void PostRender() override;

	/// <summary>
	/// ゲームモードの状態。
	/// </summary>
	enum enGameMode {
		enGameModeSurvival,		//サバイバル。
		enGameModeCreative,		//クリエイティブ。
		enGameModeNum,			//ゲームモードの数。
	};

	const enGameMode GetGameMode() const
	{
		return m_gameMode;
	}

private:

	enGameMode m_gameMode = enGameModeNum;			//ゲームモード。
	CFont m_font;									//フォント。

};

