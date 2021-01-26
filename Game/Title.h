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

private:

	GameObj::CSpriteRender* m_spriteRender = nullptr;

	std::unique_ptr<TitleSelect> m_titleSelect;
};

