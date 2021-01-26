#pragma once
#include "Node.h"

class TitleSelect : public IGameObject// GUI::Node
{
public:
	TitleSelect();
	~TitleSelect();

	bool Start()override;
	void Update()override;

	void GetScPos();		//�X�N���[����ԏ�̉摜�͈͎̔擾�B

	void ChangeColor();		//�I�����ꂽ�摜�̐F��ύX�B

	/// <summary>
	/// �}�E�X���N���b�N�����Ƃ��̔���B
	/// </summary>
	int Click();			

	//�{�^����State�B
	enum ButtonType {
		en_buttonType_GameStart,	
		en_buttonType_GameEnd,
		en_buttonType_Config,
		en_buttonType_num
	};

	ButtonType m_buttonType = en_buttonType_num;		//�{�^���̎�ނ��i�[�B

//	CVector2 GetSize() const override;

	//! @brief ���[�g�m�[�h��ǉ��B
	//void AddRoot(Node* root) {
	//	m_buttons.insert(root);
	//}

private:

	static const int m_buttonNum = 3;	//�^�C�g����ʂ̃{�^���̐��B

	float m_scale = 2.f;				//�{�^���̃X�P�[���B

	CVector2 m_position[m_buttonNum] = { CVector2::Zero() };
	CVector2 m_scPos[m_buttonNum] = {CVector2::Zero()};		//�X�N���[����ԍ��W����B
	CVector2 m_scPos2[m_buttonNum] = {CVector2::Zero()};		//�X�N���[����ԍ��W�E���B

	GameObj::CSpriteRender m_spriteRneder[m_buttonNum];

};

