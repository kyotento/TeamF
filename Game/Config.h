#pragma once

class Game;
class Config : public IGameObject
{
public:
	Config();
	~Config();

	bool Start();
	void Update();

	void GetScPos();		//スクリーン空間上の画像の範囲取得。

	void ChangeColor();		//選択された画像の色を変更

private:

	static const int m_buttonNum = 3;		//生成されるボタンの数。

	GameObj::CSpriteRender m_spriteRender[m_buttonNum];		//画像。

	CVector2 m_position[m_buttonNum] = { CVector2::Zero() };		//ボタンの座標。
	CVector2 m_scPos[m_buttonNum] = { CVector2::Zero() };			//スクリーン空間座標左上。
	CVector2 m_scPos2[m_buttonNum] = { CVector2::Zero() };			//スクリーン空間座標右下。
	float m_scale = 2.f;			//ボタンのスケール。		

	Game* m_game = nullptr;			//ゲーム。

};

