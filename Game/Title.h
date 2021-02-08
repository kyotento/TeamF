#pragma once

class TitleSelect;
class Config;
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

	/// <summary>
	/// TitleSelect�𐶐�����B
	/// </summary>
	void NewTitleSelect();

	/// <summary>
	/// TilteSelect�������B
	/// </summary>
	void DeleteTitleSelect();

	void TitleBGM();
private:

	GameObj::CSpriteRender* m_spriteRender = nullptr;

	std::unique_ptr<TitleSelect> m_titleSelect;
	SuicideObj::CSE* m_bgm;

	const wchar_t* m_bgmName;		//BGM
	const wchar_t* m_clickName;		//click
	bool m_isBgmFlag = false;		//��x���������B
};

