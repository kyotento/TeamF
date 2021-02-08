#pragma once

class Game;
//class Title;
class Config : public IGameObject
{
public:
	Config();
	~Config();

	bool Start();
	void Update();

	void PostRender() override;

	void GetScPos();		//スクリーン空間上の画像の範囲取得。

	void ChangeColor();		//選択された画像の色を変更

	//このクラスが消されたときに再生成されるクラス。
	enum enBackClass {
		enMenu,		//メニュー。
		enTitle,	//タイトル。
		enNum
	};

	/// <summary>
	/// このクラスが消されたときに再生成されるクラスの設定。
	/// </summary>
	/// <param name="a">再生成されるクラスの番号</param>
	void SetBackClass(enBackClass a) {
		m_backclass = a;
	}

	/// <summary>
	/// クリックしたときの処理。
	/// </summary>
	void ClickProcess();

	//タイトルを設定する。
	//void SetTitle(Title* title) {
	//	m_title = title;
	//}

	int Click();		//クリックした画像番号を返す。

private:

	enBackClass m_backclass = enNum;		//再生成されるクラス。

	static const int m_buttonNum = 4;		//生成されるボタンの数。

	int m_chunk = 1;						//Doneのときに反映されるチャンク。

	GameObj::CSpriteRender m_spriteRender[m_buttonNum];		//画像。

	CVector2 m_position[m_buttonNum] = { CVector2::Zero() };		//ボタンの座標。
	CVector2 m_scPos[m_buttonNum] = { CVector2::Zero() };			//スクリーン空間座標左上。
	CVector2 m_scPos2[m_buttonNum] = { CVector2::Zero() };			//スクリーン空間座標右下。
	float m_scale = 2.f;			//ボタンのスケール。		

	Game* m_game = nullptr;			//ゲーム。

	const wchar_t* m_clickName;		//click
//	Title* m_title = nullptr;		//タイトル。

	CFont m_font;					//フォント。
};

