#pragma once

class TitleSelect;
class Title : public IGameObject
{
public:
	Title();
	~Title();

	bool Start()override;
	void Update()override;

	/// <summary>
	/// �摜���N���b�N�����Ƃ��̏����B
	/// </summary>
	void ClickProcess();

private:

	CSpriteRender* m_spriteRender = nullptr;

//	TitleSelect* m_titleSelect = nullptr;		//�^�C�g���̃{�^���B
	std::unique_ptr<TitleSelect> m_titleSelect2;
};

