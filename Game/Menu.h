#pragma once
class Game;
class Menu : public IGameObject
{
public:
	Menu();
	~Menu();

	bool Start();
	void Update();

	void SetGame(Game* game) {
		m_game = game;
	}

	void GetScPos();		//スクリーン空間上の画像の範囲取得。

	void ChangeColor();		//選択された画像の色を変更。

	/// <summary>
	/// クリックしたときの処理。
	/// </summary>
	void ClickProcess();

	/// <returns>クリックしたボタン番号</returns>
	int Click();

private:

	enum enMenuButton
	{
		enMenu_ReturnToTitle,
		enMenu_GameEnd,
		enMenu_Config,
		enMenu_Num,
	};

	enMenuButton m_menuButtion = enMenu_Num;

	static const int m_buttonNum = 3;		//生成されるボタンの数。

	GameObj::CSpriteRender m_spriteRender[m_buttonNum];		//画像。

	CVector2 m_position[m_buttonNum] = { CVector2::Zero() };		//ボタンの座標。
	CVector2 m_scPos[m_buttonNum] = { CVector2::Zero() };			//スクリーン空間座標左上。
	CVector2 m_scPos2[m_buttonNum] = { CVector2::Zero() };			//スクリーン空間座標右下。
	float m_scale = 2.f;			//ボタンのスケール。		
	
	Game* m_game = nullptr;			//ゲーム。
};

