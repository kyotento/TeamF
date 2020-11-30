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
	/// 画像をクリックしたときの処理。
	/// </summary>
	void ClickProcess();

private:

	CSpriteRender* m_spriteRender = nullptr;

//	TitleSelect* m_titleSelect = nullptr;		//タイトルのボタン。
	std::unique_ptr<TitleSelect> m_titleSelect2;
};

