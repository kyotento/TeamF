#pragma once
#include "Node.h"

class TitleSelect : public GUI::Node
{
public:
	TitleSelect();
	~TitleSelect();

//	void Draw(const CVector2& pos, const CVector2& parentScale) override:

	CVector2 GetSize() const override;

	//! @brief ルートノードを追加。
	void AddRoot(Node* root) {
		m_buttons.insert(root);
	}

private:

	static const int m_buttonNum = 3;	//タイトル画面のボタンの数。

	float m_scale = 2.f;				//ボタンのスケール。

	CVector2 m_position[m_buttonNum] = { CVector2::Zero() };

	GameObj::CSpriteRender m_spriteRneder[m_buttonNum];

	//ボタンのノードリスト。
	std::unordered_set<Node*> m_buttons;
};

