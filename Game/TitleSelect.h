#pragma once
#include "Node.h"

class TitleSelect : public GUI::Node
{
public:
	TitleSelect();
	~TitleSelect();

//	void Draw(const CVector2& pos, const CVector2& parentScale) override:

	CVector2 GetSize() const override;

	//! @brief ���[�g�m�[�h��ǉ��B
	void AddRoot(Node* root) {
		m_buttons.insert(root);
	}

private:

	static const int m_buttonNum = 3;	//�^�C�g����ʂ̃{�^���̐��B

	float m_scale = 2.f;				//�{�^���̃X�P�[���B

	CVector2 m_position[m_buttonNum] = { CVector2::Zero() };

	GameObj::CSpriteRender m_spriteRneder[m_buttonNum];

	//�{�^���̃m�[�h���X�g�B
	std::unordered_set<Node*> m_buttons;
};

