#pragma once
#include "Node.h"

class TitleSelect : public IGameObject// GUI::Node
{
public:
	TitleSelect();
	~TitleSelect();

	bool Start()override;
	void Update()override;

	void GetScPos();		//スクリーン空間上の画像の範囲取得。

	void ChangeColor();		//選択された画像の色を変更。

	/// <summary>
	/// マウスをクリックしたときの判定。
	/// </summary>
	int Click();			

	//ボタンのState。
	enum ButtonType {
		en_buttonType_GameStart,	
		en_buttonType_GameEnd,
		en_buttonType_Config,
		en_buttonType_num
	};

	ButtonType m_buttonType = en_buttonType_num;		//ボタンの種類を格納。

//	CVector2 GetSize() const override;

	//! @brief ルートノードを追加。
	//void AddRoot(Node* root) {
	//	m_buttons.insert(root);
	//}

private:

	static const int m_buttonNum = 3;	//タイトル画面のボタンの数。

	float m_scale = 2.f;				//ボタンのスケール。

	CVector2 m_position[m_buttonNum] = { CVector2::Zero() };
	CVector2 m_scPos[m_buttonNum] = {CVector2::Zero()};		//スクリーン空間座標左上。
	CVector2 m_scPos2[m_buttonNum] = {CVector2::Zero()};		//スクリーン空間座標右下。

	GameObj::CSpriteRender m_spriteRneder[m_buttonNum];

};

