#pragma once

class Player;
class PlayerDeath : public IGameObject
{
public:
	PlayerDeath();
	~PlayerDeath();

	bool Start()override;
	void Update()override;
	void PostRender()override;

	void GetScPos();		//スクリーン空間上の画像の範囲取得。

	void ChangeColor();		//選択された画像の色を変更。

	/// <summary>
	/// マウスをクリックしたときの判定。
	/// </summary>
	int Click();

	enum enButtonType {
		enButtonResupawn,			//リスポーンボタン。
		enButtonRerturnToTitle,		//タイトルボタン。
		enButtonNum					//ボタンの数。
	};

	enButtonType m_buttonType = enButtonNum;		//ボタン番号。

	//経験値を設定する。
	void SetExp(float exp)
	{
		m_exp = exp;
	}

private:

	static const int m_buttonNum = 2;		//生成されるボタンの数。
	float m_scale = 2.f;					//ボタンのスケール。
	float m_exp = 0.f;						//死亡時の経験値。

	GameObj::CSpriteRender m_spriteRender[m_buttonNum];		//画像。

	CVector2 m_position[m_buttonNum] = { CVector2::Zero() };		//ボタンの座標。
	CVector2 m_scPos[m_buttonNum] = { CVector2::Zero() };			//スクリーン空間座標左上。
	CVector2 m_scPos2[m_buttonNum] = { CVector2::Zero() };			//スクリーン空間座標右下。

	CFont m_font;					//フォント。
	CFont m_font2;					//フォント。

};

