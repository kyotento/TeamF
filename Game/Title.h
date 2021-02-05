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
	/// 画像をクリックしたときの処理。
	/// </summary>
	void ClickProcess();

	/// <summary>
	/// TitleSelectを生成する。
	/// </summary>
	void NewTitleSelect();

	/// <summary>
	/// TilteSelectを消す。
	/// </summary>
	void DeleteTitleSelect();

	void TitleBGM();
private:

	GameObj::CSpriteRender* m_spriteRender = nullptr;

	std::unique_ptr<TitleSelect> m_titleSelect;
	SuicideObj::CSE* m_bgm;

	const wchar_t* m_bgmName;		//BGM
	const wchar_t* m_clickName;		//click
	bool m_isBgmFlag = false;		//一度だけ生成。
};

